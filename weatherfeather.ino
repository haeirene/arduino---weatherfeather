#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define DELAY 1000

const char* ssid = "";
const char* password = "";

const int bluePin = 13;
const int redPin = 15;
const int greenPin = 12;

//Huidige location
const String LOCATION = "Mechelen,be";

//Dummy data
//const String LOCATION = "Budapest,hu";
//const String LOCATION = "Tete,mz";
//const String LOCATION = "Bolzano,it";

//Vaste URL
const String URL = "http://api.openweathermap.org/data/2.5/weather?q=";
const String API_KEY_QUERY = "&appid=49f088e9926973eae06381752434ac70";
const String OPTIONS = "&mode=json&units=metric";

const String FULL_URL = URL + LOCATION + API_KEY_QUERY + OPTIONS;

void setup() 
{
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  Serial.begin(115200);
  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin(FULL_URL); //Specify the URL and certificate.
    int httpCode = http.GET();
    Serial.print("Searching for API.");

    /*
      < 0 there is a connection error
      > 0 api found
    */
    if (httpCode > 0) 
    {
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      String jsonString = http.getString();

      // FIND FIELDS IN JSON TREE
      JsonObject& root = jsonBuffer.parseObject(jsonString);
      JsonObject& main = root["main"];
      JsonObject& weather = root["weather"][0];
      if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
      }

      //Getting and displaying the temperature
      float temp = main["temp"];
      Serial.print("The temperature is: ");
      Serial.println(temp);

      //Getting and displaying the weather description
      String desc = weather["main"];
      displayDescription(desc);
    }
    else{
      Serial.print("Error in HTTP request");
    }
    http.end(); //Close connection
  }
  /*A new request is send every 60000 milliseconds.*/
  delay(60000);
}

void displayDescription(String weather){
  //API description gives a Capitalized string back.
  weather.toLowerCase();
  if(weather.equals("rain")){
    //blauw
    setColor(0, 0, 255);
    Serial.println("The led displays blue which means the weather is: ");
    Serial.print(weather);
  }
  else if(weather.equals("cloudy") || weather.equals("clouds")){
    //paars
    setColor(80, 0, 80);
    Serial.println("The led displays purple which means the weather is: ");
    Serial.print(weather);
  }
  else if(weather.equals("snow")){
    //aqua
    setColor(0, 255, 255);
    Serial.println("The led displays aqua which means the weather is: ");
    Serial.print(weather);
  }
  else if(weather.equals("hail")){
    //rood
    setColor(255, 0, 0);
    Serial.println("The led displays red which means the weather is: ");
    Serial.print(weather);
  }
  else if(weather.equals("clear")){
    //geel
    setColor(255, 255, 0);
    Serial.println("The led displays yellow which means the weather is: ");
    Serial.print(weather);
  }
  //Atmosphere
  else if(weather.equals("mist") || weather.equals("smoke") || weather.equals("haze") ||
  weather.equals("dust") || weather.equals("fog") || weather.equals("sand") || weather.equals("ash") ||
  weather.equals("squall") || weather.equals("tornado")){
    //orange
     setColor(255, 255, 255),
     Serial.println("The led displays orange which means the weather is: ");
     Serial.print(weather);
  }
  //Drizzle & thunderstorm
  else{
    //wit
    setColor(255, 255, 255),
    Serial.println("The led displays white which means the weather is: ");
    Serial.print(weather);
  }
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

