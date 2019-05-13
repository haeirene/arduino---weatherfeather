#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/*
const char* ssid = SCHOOL_SSID;
const char* password = SCHOOL_PASSWORD;
*/

int red = 4;
//int blue = 5;
//int green = 2;

void setup() 
{
  pinMode(red, OUTPUT);
  //pinMode(blue, OUTPUT);
  //pinMode(green, OUTPUT);
  
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
  /*digitalWrite(green, LOW);
  delay(10);
  digitalWrite(red, LOW);
  digitalWrite(blue, HIGH);
  delay(10);
  digitalWrite(green, HIGH);
  digitalWrite(blue, LOW);*/
  
  digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, HIGH);
  
  if (WiFi.status() == WL_CONNECTED) 
  {
	HTTPClient http; //Object of class HTTPClient
	http.begin("http://api.openweathermap.org/data/2.5/weather?q=London,uk&appid=49f088e9926973eae06381752434ac70&mode=json&units=metric"); //Specify the URL and certificate.
	int httpCode = http.GET();
	Serial.println("Searching for API.");

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

    //Getting and displaying the temmperature
	  float temp = main["temp"];
    Serial.print("The temperature is: ");
	  Serial.println(temp);

    //Getting and displaying the weather description
    String desc = weather["main"];
    Serial.print("The weather description is: ");
    Serial.println(desc);
    
	}
	else{
	  Serial.print("Error in HTTP request");
	}
	http.end(); //Close connection
  }
  /*A new request is send every 60000 milliseconds.*/
  delay(60000);
}