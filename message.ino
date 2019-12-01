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
String modus;
char idSaved; 
bool wasRead;  

void drawMessage(const String& message) {
  oled.clear();

  // Unterscheide zwischen Text und Bild
  if(modus[0] == 't'){
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
  
    // Warte auf Verbindung
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}

void getGistMessage() {
  const int httpsPort = 443;
  const char* host = "gist.githubusercontent.com";
  const char fingerprint[] = "CC AA 48 48 66 46 0E 91 53 2C 9C 7C 23 2A B1 74 4D 29 9D 33";
  
  WiFiClientSecure client;
  client.setFingerprint(fingerprint);
  if (!client.connect(host, httpsPort)) {
    return; // Verbindung fehlgeschlagen
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
  if(id[0] != idSaved){ // Neue Nachricht
    wasRead = 0;
    idSaved = id[0];
    EEPROM.write(142, idSaved);
    EEPROM.write(144, wasRead);
    EEPROM.commit(); 

    modus = client.readStringUntil('\n');
    line = client.readStringUntil(0);
    drawMessage(line);
  }
}

void spinServo(){
    myservo.write(pos);      
    delay(50);    // Warte 50ms um den Servo zu drehen

    if(pos == 75 || pos == 105){ // Drehbereich zwischen 75°-105°
      increment *= -1;
    }
    pos += increment;
}

void setup() {
  myservo.attach(16);       // Servo an D0
  
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
  if(wasRead){
    getGistMessage();   
  }
  
  while(!wasRead){   
    spinServo();    // Drehe Herz
    lightValue = analogRead(0);      // Lese Helligkeitswert
    if(lightValue > 300) { 
      wasRead = 1;
      EEPROM.write(144, wasRead);
      EEPROM.commit();
    }
  }
}