#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"

#include "http_client.h"
#include "IoT_DevKit_HW.h"
#include "SystemVersion.h"
#include "SystemTickCounter.h"
#include "telemetry.h"

//#include "config.h"
//#include "utility.h"


static bool hasWifi = false;
static bool hasIoTHub = false;

void setup() {
  // put your setup code here, to run once:
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
}

void loop() {

  //float temp = readTemperature();

  // put your main code here, to run repeatedly:
  //Screen.print(1, "In loop()");
  //delay(500);

  //sprintf_P
  
  //int temp = getDevKitTemperatureValue(0);
  //char buf[10];


  //showHumidTempSensor();


  //sprintf_P(buf, "%d", temp);
  //const char * readTemp = ("", temp, "");
 // Screen.print(1, buf);

  // delay(250);

  // if (!hasIoTHub) {
  //   Screen.print(1, "hasIoTHub false");
  //   delay(250);
  // } else {
  //   Screen.print(1, "hasIoTHub");
  //   delay(250);
  // }

  // if (!hasWifi) {
  //   Screen.print(1, "hasWifi false");
  //   delay(250);
  // } else {
  //   Screen.print(1, "hasWifi");
  //   delay(250);
  // }

  if (hasIoTHub && hasWifi)
  {
    char buff[128];

    //Get data from temp sensor
    
    //Screen.print(1, "Before send");

    // replace the following line with your data sent to Azure IoTHub

    int temp = getDevKitTemperatureValue(0);
    const char * a = ("{\"temp\": \"", temp, ":\"}");

    //snprintf(buff, 128, "{\"topic\":\"iot\"}");
    snprintf_P(buff, 128, a);
    
    //if (DevKitMQTTClient_SendEvent(buff))
    if (DevKitMQTTClient_SendEvent(buff))
    {
      Screen.print(1, "Sending...");
    }
    else
    {
      Screen.print(1, "Failure...");
    }
    delay(2000);
  }
}
