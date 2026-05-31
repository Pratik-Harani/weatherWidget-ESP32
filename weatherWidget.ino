#include "secrets.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "connectToWifi.h"
#include <ArduinoJson.h>

//preferences
#define BG_COLOR TFT_WHITE
#define FG_COLOR TFT_BLACK
#define WEATHER_UPDATE_FREQUENCY_MINS 30


int BasePositionX = 120;
int BasePositionY = 55;
int textSize = 4;
 

TFT_eSPI tft = TFT_eSPI();

//setting up weather stuff
char server[] = "api.openweathermap.org";
String apiKey = SECRET_WEATHER_API_KEY;
String latitude = SECRET_LATITUDE; 
String longitude = SECRET_LONGITUDE;



struct WeatherData{
  String temp;
  String main;
  String description;
  String icon;
};



void setup()
{
  Serial.begin(115200);
  while (!Serial){delay(10);}
  connectToWifi();

  tft.init();
  tft.fillScreen(BG_COLOR); 
  tft.setTextSize(textSize);
  tft.setRotation(1);
  tft.setTextColor(FG_COLOR);
  tft.setTextDatum(MC_DATUM);

  
  //DEBUGGING: printing weather in serial monitor
  // Serial.print("Temp: ");
  // Serial.println(myData.temp);

  // Serial.print("Weather: ");
  // Serial.println(myData.main);

  // Serial.print("Description: ");
  // Serial.println(myData.description);
  

}

void loop() {
  WeatherData myData = getWeather();
  tft.fillScreen(BG_COLOR);
  tft.drawString(myData.temp + char(247) + "C", BasePositionX, BasePositionY);
  Serial.println(myData.main);

  int descriptionTextSize = textSize;
  while (tft.textWidth(myData.description) > tft.width()){
      //Serial.print("text width is: ");
      // Serial.println(tft.textWidth(myData.description));
      // Serial.print("screen width is: ");
      // Serial.println(tft.width());
      descriptionTextSize = descriptionTextSize - 1;
      // Serial.print("new text size: ");
      // Serial.println(textSize);
      tft.setTextSize(descriptionTextSize);
  }

  tft.drawString(myData.description, 120, 95);
  tft.setTextSize(textSize);
  delay(WEATHER_UPDATE_FREQUENCY_MINS * 60 * 1000);
}





WeatherData getWeather() {

  WeatherData data;

  //Serial.println("\nConnecting to server..");

  if (!client.connect(server, 80)) {
    Serial.println("Connection failed :(");
    return data;
  }


  //Serial.println("Connected to server :)");

  client.println("GET /data/2.5/weather?lat="+latitude+"&lon="+longitude+"&appid="+apiKey+"&units=metric HTTP/1.1");
  client.println("Host: api.openweathermap.org");
  client.println("Connection: close");
  client.println();
  
  //skipping HTTP headers
  while (!client.available()) {
    delay(10);
  }

  // skip HTTP headers
  while (client.available()) {
    char c = client.peek();
    if (c == '{') break;
    client.read();
  }

  //parsing JSON
  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, client);

  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return data;
  }

  //extracting values
  data.temp = String(doc["main"]["temp"].as<float>(), 0);
  data.main = doc["weather"][0]["main"].as<String>();
  data.description = doc["weather"][0]["description"].as<String>();
  data.icon = doc["weather"][0]["icon"].as<String>();

  client.stop();

  return data;
}







