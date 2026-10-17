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

ESP8266WebServer server(80);
EspHtmlTemplateProcessor templateProcessor(&server);

// Start/Stop/Alarm SetPoints
const int ON_PIN = 5; //Output Pin From ESP8266 to Relay D1
float TANK_LEVEL_ON = 3.0; // Feet ajustable =< Threshold PUMP ON
float TANK_LEVEL_OFF = 5.0; // Feet ajustable => Threshold PUMP OFF
float ALARM_LEVEL = 2.0; // Alarm Level sends alarm to web page

const int pressureInput = A0; //select the analog input pin for the pressure transducer
const float pressureZero = 0.0; //analog reading of pressure transducer at 0psi
const float pressureMax = 1024.0; //analog reading of pressure transducer at 100psi
const float pressuretransducermaxPSI = 100.0; //psi value of transducer being used
const int baudRate = 115200; //constant integer to set the baud rate for serial monitor
const unsigned long sensorreadDelay = 5000; //constant integer to set the sensor read delay in milliseconds 5000=5 seconds

float voltage = 0;
float tank_height = 0;
float pressureValue = 0; //variable to store the value coming from the pressure transducer
int adcValue = 0;

unsigned long lastSensorRead = 0;

void setup() //setup routine, runs once when system turned on or reset
{
  Serial.begin(baudRate); //initializes serial communication at set baud rate bits per second

  pinMode(ON_PIN, OUTPUT);
  digitalWrite(ON_PIN, LOW);

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS mount failed");
  }

  WiFi.begin(ssid, password);

  Serial.println("Connecting to wifi ");

  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 30000)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection failed");
  } else {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() //loop routine runs over and over again forever
{
  server.handleClient();

  unsigned long now = millis();
  if (now - lastSensorRead >= sensorreadDelay) {
    lastSensorRead = now;

    adcValue = analogRead(pressureInput);
    pressureValue = ((adcValue - pressureZero) * pressuretransducermaxPSI) / (pressureMax - pressureZero); //conversion equation to convert analog reading to psi
    voltage = adcValue * (3.3 / 1023.0); //Voltage used 3.3VDC
    tank_height = pressureValue; // 0.433; PSI=1'WC

    Serial.println("    "); // Space
    Serial.print(pressureValue, 1); //prints value from previous line to serial
    Serial.println(" psi"); //prints label to serial
    Serial.println("    "); //Space
    Serial.println("****Tank Level****"); //prints label to serial
    Serial.print(tank_height, 1);
    Serial.println(" Feet_H20"); //prints label to serial
    Serial.print("ADC Value: ");
    Serial.println(adcValue);

    // Relay Control with simple hysteresis
    if (tank_height <= TANK_LEVEL_ON) {
      digitalWrite(ON_PIN, HIGH); // Turn on Relay
    } else if (tank_height >= TANK_LEVEL_OFF) {
      digitalWrite(ON_PIN, LOW);  // turn off Relay
    }
  }

  yield();
}

//Web Script Data Useing index1.html in data folder
String indexKeyProcessor(const String& key)
{
  if (key == "VAR1") return " "+String(ALARM_LEVEL)+" "; // Alarm Level
  if (key == "VAR2") return " "+String(pressureValue, 1)+" "; //PSI STRING
  if (key == "VAR3") return " "+String(tank_height, 1)+" "; // FEET STRING
  if (key == "VAR4") return " "+String(adcValue)+" "; // ADC 0-1024

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
