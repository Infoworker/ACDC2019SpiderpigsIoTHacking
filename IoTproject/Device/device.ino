#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"

#include "http_client.h"
#include "IoT_DevKit_HW.h"
#include "SystemVersion.h"
#include "SystemTickCounter.h"
#include "telemetry.h"

#include "HTS221Sensor.h"
//#include "LPS22HBSensor.h"

DevI2C *i2c;
HTS221Sensor *sensor;

unsigned char id;
float temperature = 0;
static bool hasWifi = false;
static bool hasIoTHub = false;

void setup() {
  if (WiFi.begin() == WL_CONNECTED)
  {
    hasWifi = true;
    Screen.print(1, "Running...");

    if (!DevKitMQTTClient_Init())
    {
      hasIoTHub = false;
      return;
    }
    hasIoTHub = true;
  }
  else
  {
    hasWifi = false;
    Screen.print(1, "No Wi-Fi");
  }


  i2c = new DevI2C(D14, D15);
  sensor = new HTS221Sensor(*i2c);
  //init the sensor
  sensor -> init(NULL);
}

void loop() {

  sensor -> enable();
  sensor -> readId(&id);
  sensor -> getTemperature(&temperature);



  if (hasIoTHub && hasWifi)
  {
    char buff[128];

    char res[8];
  
    dtostrf(temperature, 4, 2, res);

    char toSend[128];
    strcpy(toSend, "{\"temperature\": \"");
    strcat(toSend, res);
    strcat(toSend, "\"}");

    const char * a = ("{\"temperature\": \"", res, "\"}");
    //const char * a =     

    Screen.print(1, res);
    delay(500);
    Screen.print(1, toSend);
    delay(500);
    
    //if (DevKitMQTTClient_SendEvent(buff))
    if (DevKitMQTTClient_SendEvent(toSend))
    {
      Screen.print(1, "Sending...");
    }
    else
    {
      Screen.print(1, "Failure...");
    }
    delay(120000);
  }
}
