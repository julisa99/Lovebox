#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <EEPROM.h>

#include <Wire.h> 
#include <SSD1306.h> // oled display
#include "credentials.h"
#include <Servo.h>

int pos = 90;
int increment = -1;
String line;
String modus;
char idSaved;
bool Read;

Servo myservo;  // servo object
SSD1306  display(0x3c, D2, D1);

// network credentials
const char* ssid = _ssid;
const char* password = _password;

void initDisplay() {
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10); 
}

void drawMessage(const String& message) {
  display.clear();

  // differentiate between text and image
  if(modus[0] == 't'){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawStringMaxWidth(0, 0, 128, message);
  } 
  else {
    display.setColor(WHITE);
    for(int i = 0; i <= message.length(); i++){
      int x = i % 129;
      int y = i / 129;
    
      if(message[i] == '1'){
        display.setPixel(x, y);
      }
    } 
  } 
     
  display.display();
}

void wifiConnect() {
  // connect to wifi
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
  
    // wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}

void getMessage() {
  const int httpsPort = 443;
  const char* host = "gist.githubusercontent.com";
  
  WiFiClientSecure client;
  if (!client.connect(host, httpsPort)) {
    return; // connection failed
  }
  
  const String url = _url;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  while (client.connected()) {
    String temp = client.readStringUntil('\n');
    if (temp == "\r") {
      break; // headers received
    }
  }
  String id = client.readStringUntil('\n'); 
  if(id[0] != idSaved){
    Read = 0;
    idSaved = id[0];
    EEPROM.write(142, idSaved);
    EEPROM.write(144, Read);
    EEPROM.commit(); // new message received
  }
  modus = client.readStringUntil('\n');
  line = client.readStringUntil(0);
}

void spinServo(){
    myservo.attach(16);       // attaches D0 to the servo object
    myservo.write(pos);       // servo goes to actual 'pos'
    delay(50);                // waits 50ms to reach the position

    if(pos == 75 || pos == 105){
      increment *= -1;
    }
    pos += increment;
}

void setup() {
  Serial.begin(9600);

  initDisplay();

  display.clear();
  display.drawString(30, 30, "<3 LOVEBOX <3");
  display.display();
  
  drawMessage("Verbindung zum Internet wird aufgebaut..");
  wifiConnect();

  EEPROM.begin(512);
  idSaved = EEPROM.get(142, idSaved);
  Read = EEPROM.get(144, Read);
}

void loop() {
  getMessage(); 
  drawMessage(line);
  
  int lightValue = analogRead(0);
  while(!Read){   
    spinServo();
    lightValue = analogRead(0);      // update sensor
    if(lightValue < 800) { 
      Read = 1;
      EEPROM.write(144, Read);
      EEPROM.commit();
    }
  }
}

