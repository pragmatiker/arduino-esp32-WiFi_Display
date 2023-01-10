#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <WiFiManager.h>    
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define OLED_SDA 18
#define OLED_SCL 19
#define JSON_CONFIG_FILE "/test_config.json"

bool loadConfigFile();

Adafruit_SH1106 display(OLED_SDA, OLED_SCL);
char WLAN_IP_STR[25];
String display_lines[128]= {""};

void setup()   {                
  Serial.begin(115200);
  /* initialize OLED with I2C address 0x3C */
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  //wm.resetSettings();
  bool res;
  res = wm.autoConnect("AutoConnectAP","password");
  if(!res) {
      Serial.println("Failed to connect");
      ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
      sprintf(WLAN_IP_STR,"IP: %s", WiFi.localIP().toString().c_str());
      Serial.println(WLAN_IP_STR); 
  }
  loadConfigFile();
}
void loop() { 
  display.setFont();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.drawRect(0,53,128,11,WHITE);
  display.setCursor(1,55);
  display.println(WLAN_IP_STR);
  display.display();
  
  // Start with Racoon Stuff :)
  for (int i=0; i < 128; i++)
  {
    if (!display_lines[i].isEmpty())
    {
      display.setTextColor(WHITE,BLACK);
      display.fillRect(0,0,128,16,BLACK);
      display.setCursor(0,0);
      display.println(display_lines[i]);
      display.display();
      delay(3000);
    } else {
      Serial.print("Empty line: ");
      Serial.println(i);
    }
  }
}

bool loadConfigFile()
{
  Serial.println("Mounting File System");
  if (SPIFFS.begin(false) || SPIFFS.begin(true))
  {
    Serial.println("Mounted file system");
    if (SPIFFS.exists(JSON_CONFIG_FILE))
    {
      // The file exists, reading and loading
      Serial.println("Reading config file");
      File configFile = SPIFFS.open(JSON_CONFIG_FILE,"r");
      if (configFile)
      {
        Serial.println("Opened config file");
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        //deserializeJson(json, Serial);
        if (!error)
        {
          Serial.println("Parsing JSON");
          JsonArray display_lines_json = json["display_lines"];
          copyArray(display_lines_json, display_lines);
          Serial.println("Parsed JSON");
          return true;
        }
        else
        {
          Serial.println("Failed loading json config");
          Serial.println(error.c_str());
          return false;
        }
      }
    }
  }
}