#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include <Servo.h>
#include "SSD1306Wire.h"

#include "credentials.h"
const char* ssid = _ssid;
const char* password = _password;
const String url = _url;

SSD1306Wire oled(0x3C, D2, D1);
Servo myservo; 
int pos = 90;
int increment = -1;
int lightValue;
String line;
String messageMode;
char idSaved; 
bool wasRead;  

void drawMessage(const String& message) {
  oled.clear();

  // differentiat between 't'ext and image message
  if(messageMode[0] == 't'){
    oled.drawStringMaxWidth(0, 0, 128, message);    
  } 
  else {
    for(int i = 0; i <= message.length(); i++){
      int x = i % 129;
      int y = i / 129;
    
      if(message[i] == '1'){
        oled.setPixel(x, y);
      }
    } 
  }    
  oled.display();
}

void wifiConnect() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);

    // connecting to WiFi
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}

void getGistMessage() {
  const int httpsPort = 443;
  const char* host = "gist.githubusercontent.com";
  const char fingerprint[] = "70 94 DE DD E6 C4 69 48 3A 92 70 A1 48 56 78 2D 18 64 E0 B7";
  
  WiFiClientSecure client;
  client.setFingerprint(fingerprint);
  if (!client.connect(host, httpsPort)) {
    return; // failed to connect
  }
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String temp = client.readStringUntil('\n');
    if (temp == "\r") {
      break;
    }
  }
  String id = client.readStringUntil('\n'); 
  if(id[0] != idSaved){ // new message
    messageMode = client.readStringUntil('\n');
    if (messageMode[0] == 't'){
      line = client.readStringUntil(0);
    } else {
      // binary image is corrupted if readStringUntil() takes too long
      // fix: read string line by line
      line = "";
      for (int i = 0; i < 64; i++)     
      {
        line += client.readStringUntil('\n');
        line += "\n";
      }
      if (line.length() != 8256)
      {
        getGistMessage();
      }
    }
    wasRead = 0;
    idSaved = id[0];
    EEPROM.write(142, idSaved);
    EEPROM.write(144, wasRead);
    EEPROM.commit(); 
    drawMessage(line);
  }
}

void spinServo(){
    myservo.write(pos);      
    delay(50);    // wait 50ms to turn servo

    if(pos == 75 || pos == 105){ // 75°-105° range
      increment *= -1;
    }
    pos += increment;
}

void setup() {
  myservo.attach(16);       // Servo on D0
  
  oled.init();
  oled.flipScreenVertically();
  oled.setColor(WHITE);
  oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oled.setFont(ArialMT_Plain_10);
     
  oled.clear();
  oled.drawString(30, 30, "<3 LOVEBOX <3");
  oled.display();
  
  wifiConnect();

  EEPROM.begin(512);
  idSaved = EEPROM.get(142, idSaved);
  wasRead = EEPROM.get(144, wasRead);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnect();
  }
  
  if(wasRead){
    getGistMessage();   
  }
  
  while(!wasRead){   
    yield();
    spinServo();    // turn heart
    lightValue = analogRead(0);      // read light value
    if(lightValue > 300) { 
      wasRead = 1;
      EEPROM.write(144, wasRead);
      EEPROM.commit();
    }
  }
  
  delay(60000); // wait a minute before request gist again
}
