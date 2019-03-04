# 1 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino"
# 1 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino"
# 2 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 2
# 3 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 2

# 5 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 2
# 6 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 2
# 7 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 2
# 8 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 2
# 9 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 2

# 11 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 2

DevI2C *i2c;
HTS221Sensor *sensor;



//#include "config.h"
//#include "utility.h"
//#include "SystemTickCounter.h"



unsigned char id;
float temperature = 0;
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


  i2c = new DevI2C(D14, D15);
  sensor = new HTS221Sensor(*i2c);
  //init the sensor
  sensor -> init(
# 52 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino" 3 4
                __null
# 52 "c:\\Dev\\GitHub\\ACDC2019SpiderpigsIoTHacking\\IoTproject\\Device\\device.ino"
                    );
}

void loop() {

  sensor -> enable();
  sensor -> readId(&id);
  sensor -> getTemperature(&temperature);
  //temperature = readTemperature();

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
