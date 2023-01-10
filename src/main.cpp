#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <WiFiManager.h>    

#define OLED_SDA 18
#define OLED_SCL 19

Adafruit_SH1106 display(OLED_SDA, OLED_SCL);
char WLAN_IP_STR[25];

void setup()   {                
  Serial.begin(19200);
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
  
}
void loop() { 
  display.setFont();
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0,57);
  display.print(WLAN_IP_STR);
  display.display();
  
  // Start with Racoon Stuff :)
  display.fillRect(0,0,128,15,BLACK);
  display.setCursor(0,0);
  display.println("Hallo Waschbaer!");
  display.display();
  delay(3000);
  display.fillRect(0,0,128,15,BLACK);
  display.setCursor(0,0);
  display.println("Du liebst Katzen!");
  display.display();
  delay(3000);
  display.fillRect(0,0,128,15,BLACK);
  display.setCursor(0,0);
  display.println("Dein Lulu... ");
  display.println("...  wird frieren :)");
  display.display();
  delay(3000);
}