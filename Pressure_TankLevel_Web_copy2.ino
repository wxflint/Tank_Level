/* Tank Control With Pressure Transducer and web access ESP8266
 * V1.4 Readme.adoc for change log
 * Modded By: Patrick Sauer
 * Contact wxflint@gmail.com
 * vftechserv.com
 * 
 */
//#include <AsyncElegantOTA.h>
#include "Arduino.h"
#include "settings.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WebServer.h>

#include "EspHtmlTemplateProcessor.h"
#include "Wire.h" //allows communication over i2c devices if used


void handleRoot();

const char* ssid = "wssid";// Your WIFI SSID
const char* password = "wpassword";// Your WIFI Password

String page  = "";

ESP8266WebServer server;
EspHtmlTemplateProcessor templateProcessor(&server);
// Start/Stop/Alarm SetPoints
const int ON_PIN = 5; //Output Pin From ESP8266 to Relay D1
int TANK_LEVEL_ON = 3.0; // Feet ajustable =< Threshold PUMP ON
int TANK_LEVEL_OFF = 5.0; // Feet ajustable => Threshold PUMP OFF
int ALARM_LEVEL = 2.0; // Alarm Level sends alarm to web page

const int pressureInput = A0; //select the analog input pin for the pressure transducer
const int pressureZero = 0.0; //analog reading of pressure transducer at 0psi
const int pressureMax = 1024.0; //analog reading of pressure transducer at 100psi
const int pressuretransducermaxPSI = 100; //psi value of transducer being used
const int baudRate = 115200; //constant integer to set the baud rate for serial monitor
const int sensorreadDelay = 5000; //constant integer to set the sensor read delay in milliseconds 5000=5 seconds
int voltage;
int tank_height;
int data;
 
int a = analogRead(A0);
int adcValue = (a);
float pressureValue = 0; //variable to store the value coming from the pressure transducer


void setup() //setup routine, runs once when system turned on or reset
{
    // Start ElegantOTA

  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second


  SPIFFS.begin();

  WiFi.begin(ssid, password);

  Serial.println("Connecting to wifi ");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 pinMode(ON_PIN, OUTPUT);
}

void loop() //loop routine runs over and over again forever
{
  
  pressureValue = analogRead(pressureInput); //reads value from input pin and assigns to variable
  pressureValue = ((pressureValue-pressureZero)*pressuretransducermaxPSI)/(pressureMax-pressureZero); //conversion equation to convert analog reading to psi
  voltage = (pressureValue) * ((3.3 / 1023.0)); //Voltage used 3.3VDC
  tank_height = (pressureValue); // 0.433; PSI=1'WC
  //Serial.print(voltage);


 Serial.println("    "); // Space
  Serial.print(pressureValue, 1); //prints value from previous line to serial
  Serial.println(" psi"); //prints label to serial
  Serial.println("    "); //Space
  Serial.println ("****Tank Level****"); //prints label to serial
  Serial.print(tank_height);
  Serial.println( " Feet_H20"); //prints label to serial
//Serial.print (a); // For Testing
//Serial.println( " RTV"); // For Testing
Serial.print("ADC Value: ");
Serial.println(analogRead(A0));
  
             // Relay Control
  if(tank_height <= TANK_LEVEL_ON) // Turn on Relay (IF TANK LEVEL IS LESS THAN "TANK_LEVEL_ON" SET POINT TURN PUMP ON)
    digitalWrite(ON_PIN, HIGH); // Turn on Relay (IF TANK LEVEL IS LESS THAN "TANK_LEVEL_ON" SET POINT TURN PUMP ON)
   else if
  (tank_height >= TANK_LEVEL_OFF)// Turn Relay Off (IF TANK LEVEL IS LESS THAN "TANK_LEVEL_OFF" SET POINT TURN PUMP OFF  && distance <15)
    digitalWrite(ON_PIN, LOW);  // turn off Relay
    
  delay(sensorreadDelay); //delay in milliseconds between read values
  server.handleClient();
}

//Web Script Data Useing index1.html in data folder
String indexKeyProcessor(const String& key)
{
 // if (key == "TITLE") return "Hello Test!";
if (key == "VAR1") return " "+String(ALARM_LEVEL)+" "; // Alarm Level
if (key == "VAR2") return " "+String(pressureValue)+" "; //PSI STRING
if (key == "VAR3") return " "+String(tank_height)+" "; // FEET STRING
if (key == "VAR4") return " "+String(analogRead(A0))+" "; // Voltage 0-1024 0=0%__1024=100%

//if(VAR3 >= 25) // TEST NOT USED
//if (key == "VAR3" && distance < 1) return "Full Tank"; //TEST NOT USED
return "Key not found";
}



void handleRoot()
{
  templateProcessor.processAndSend("/index1.html", indexKeyProcessor);
}

/*
 * How many feet are in a psi? Therefore, one pound of pressure per square inch (PSI) is equal to 2.31 feet of head.
 * What is the pressure of water per foot? You can work this out with a simple rule: 1 foot of water creates 0.433 psi of pressure, and it takes 2.31 feet of water to create 1 psi of pressure.
 * 
 */