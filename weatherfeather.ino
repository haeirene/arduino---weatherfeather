#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/*
const char* ssid = SCHOOL_SSID;
const char* password = SCHOOL_PASSWORD;
*/

const char* ssid = "";
const char* password = "";


void setup() 
{
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
	http.begin("http://api.openweathermap.org/data/2.5/weather?q=London,uk&appid=49f088e9926973eae06381752434ac70&mode=json&units=metric"); //Specify the URL and certificate.
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
	  if (!root.success()) {
		Serial.println("parseObject() failed");
		return;
	  }

	  float temp = main["temp"];
	  Serial.println(temp);


	  //const char* mainTemperature = parsed["main"];
	  //const char* currentTemperature = mainTemperature["temp"];

	  //Serial.println(mainTemperature);
	  //Serial.println(currentTemperature);
	}
	else{
	  Serial.print("Error in HTTP request");
	}
	http.end(); //Close connection
  }
  /*A new request is send every 60000 milliseconds.*/
  delay(60000);
}