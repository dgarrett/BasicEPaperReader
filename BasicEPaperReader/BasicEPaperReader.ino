#include "Keys.h"
#include "Certs.h"

#include <Arduino.h>

// Graphics library
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

// Wifi Libraries
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

// JSON library
#include <ArduinoJson.h>

// select the display class to use, only one
#include <GxGDEP015OC1/GxGDEP015OC1.h>    // 1.54" b/w
//#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>


#if 0
SPIClass s(HSPI);
GxIO_Class io(s, /*CS=5*/ 15, /*DC=*/ 5, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4

SPIClass s(VSPI);
GxIO_Class io(s, /*CS=5*/ 15, /*DC=*/ 33, /*RST=*/ 27); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 27, /*BUSY=*/ 32); // arbitrary selection of (16), 4

#endif

// Set pins for EPaper Display
SPIClass s(VSPI); // VSPI or HSPI
GxIO_Class io(s, /*CS=*/ 15, /*DC=*/ 33, /*RST=*/ 27);
GxEPD_Class display(io, /*RST=*/ 27, /*BUSY=*/ 32);

// Button pins
#define UP 14
#define DOWN 21
#define SELECT 23
#define BACK 22


WiFiMulti wifiMulti;

const char*  pocket_server = "getpocket.com";
const char*  summarizer_server = "api.aylien.com";

const GFXfont* font = &FreeSerif9pt7b;

unsigned int current_article = 0;

struct Article {
  String title;
  String url;
};

Article articles[3];

void refreshScreen(bool partial = false)
{
  partial = false;
  if (partial)
  {
    display.updateWindow(0, 0, display.width(), display.height());
  }
  else
  {
    display.update();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");

  display.init(115200); // enable diagnostic output on Serial
  //display.setRotation(3);

  wifiMulti.addAP(WIFI_AP_NAME, WIFI_PASSWORD);

  // Use internal pullups for buttons
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(SELECT, INPUT_PULLUP);
  pinMode(BACK, INPUT_PULLUP);
  
  refreshScreen();
  
  loadingScreen();
  updateArticles();
  displayList();

  Serial.println("setup done");
}

void loop() {
  resetScreen();
  if (digitalRead(DOWN) == LOW)
  {
    Serial.println("Down Clicked!");
    
    current_article++;
    current_article %= 3;
    displayList();
  }
  else if (digitalRead(UP) == LOW)
  {
    Serial.println("Up Clicked!");
    
    current_article--;
    current_article %= 3;
    displayList();
  }
  else if (digitalRead(BACK) == LOW)
  {
    Serial.println("Back Clicked!");
    loadingScreen();
    updateArticles();
    displayList();
  }
  else if (digitalRead(SELECT) == LOW)
  {
    Serial.println("Select Clicked!");
    
    loadingScreen();
    displayArticleSummary(articles[current_article].url.c_str());
  }
  delay(5);

}

void resetScreen()
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(font);
  display.setCursor(0, 0);
  display.println();
}

void loadingScreen()
{
  resetScreen();
  display.println("Loading...");
  refreshScreen();
  resetScreen();
}

void updateArticles()
{
  while (wifiMulti.run() != WL_CONNECTED){}
  if((wifiMulti.run() == WL_CONNECTED)) {
    
    WiFiClientSecure client;
    client.setCACert(amazon_root_ca);
  
    Serial.println("\nStarting connection to server...");
    if (!client.connect(pocket_server, 443)) {
      Serial.println("Connection failed!");
      char err[1024];
      client.lastError(err, 1024);
      Serial.println(err);
    } else {
      Serial.println("Connected to server!");
      String content = "consumer_key=" POCKET_CONSUMER_KEY "&access_token=" POCKET_ACCESS_KEY "&count=3&sort=newest&contentType=article";
      // Make a HTTP request:
      client.println("POST /v3/get HTTP/1.1");
      client.println("Host: getpocket.com");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println("X-Accept: application/x-www-form-urlencoded");
      client.println(String("Content-Length: ") + content.length());
      client.println();
      client.println(content);
  
      char status[32] = {0};
      client.readBytesUntil('\r', status, sizeof(status));
      if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
        Serial.print(F("Unexpected response: "));
        Serial.println(status);
        return;
      }
    
      // Skip HTTP headers
      char endOfHeaders[] = "\r\n\r\n";
      if (!client.find(endOfHeaders)) {
        Serial.println(F("Invalid response"));
        return;
      }
    
      // Allocate the JSON document
      // Use arduinojson.org/assistant to compute the capacity.
      const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
      DynamicJsonBuffer jsonBuffer(capacity);

      // Parse JSON object
      JsonObject& root = jsonBuffer.parseObject(client);
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }
      
      // Extract values
      Serial.println(F("Response:"));
      Serial.println(root["status"].as<int>());

      int i = 0;
      for (JsonPair& p : root["list"].as<JsonObject>()) {
        articles[i].title = p.value.as<JsonObject>()["given_title"].as<char*>();
        articles[i].url = p.value.as<JsonObject>()["given_url"].as<char*>();
        i++;
      }
      
      client.stop();
    }
  }
}

void displayList()
{
  int i = 0;
  for (auto const& a : articles) {
    if (i++ == current_article)
    {
      display.print(">>");
    }
    else
    {
      display.print("--");
    }
    display.println(a.title);
  }
  refreshScreen(true);
}

void displayArticleSummary(const char* url)
{
  while (wifiMulti.run() != WL_CONNECTED){}
  if((wifiMulti.run() == WL_CONNECTED)) {
    
    WiFiClientSecure client;
    client.setCACert(comodo_root_ca);
  
    Serial.println("\nStarting connection to server...");
    if (!client.connect(summarizer_server, 443)) {
      Serial.println("Connection failed!");
      char err[1024];
      client.lastError(err, 1024);
      Serial.println(err);
    } else {
      Serial.println("Connected to server!");
      // Make a HTTP request:
      client.println(String("GET /api/v1/extract?url=") + url + " HTTP/1.1");
      client.println("Host: api.aylien.com");
      client.println("Accept: application/json");
      client.println("X-AYLIEN-TextAPI-Application-Key: " AYLIEN_APPLICATION_KEY);
      client.println("X-AYLIEN-TextAPI-Application-ID: " AYLIEN_APPLICATION_ID);
      client.println();
  
      char status[32] = {0};
      client.readBytesUntil('\r', status, sizeof(status));
    
      // Skip HTTP headers
      char endOfHeaders[] = "\r\n\r\n";
      if (!client.find(endOfHeaders)) {
        Serial.println(F("Invalid response"));
        return;
      }
    
      // Allocate the JSON document
      // Use arduinojson.org/assistant to compute the capacity.
      const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
      DynamicJsonBuffer jsonBuffer(capacity);

      // Parse JSON object
      JsonObject& root = jsonBuffer.parseObject(client);
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }
      
      // Extract value
      display.println(root["article"].as<char*>());
      refreshScreen();
      
      client.stop();
    }
  }
}
