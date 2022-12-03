# Tank_Level
:Revision: version 1.4
ESP8266 Tank Level Start Stop Pumps
Project: Pressure_TankLevel_Web
Takes a Pressure from a tank and converts the PSI in to Feet of H20 can fill a tank via pump has setpoints for start and stop pumps.
Has web page to see tank pressure and Hight of water level.
Can change the Tank height and scale it to your need.

![PRESSURE](https://user-images.githubusercontent.com/1435118/205456002-9ca8f2bc-1539-4d9f-a0b8-7772e2f20a7d.png)


Step 1: Installation
1. Open this file
2. Edit as needed


Step 2: Assemble the circuit

Assemble the circuit, the diagram pressure.png attached to the sketch is for demonstration purposes. Wire yours to reflect your code.

Step 3: Load the code

Upload the code contained in this sketch on to your board

Folder structure

....
Pressure_TankLevel_Web  => Arduino sketch folder
Pressure_TankLevel_Web.ino  => main Arduino file
pressure.png => an image of the required schematics
index1.html => web file script goes in data folder
ReadMe.adoc  => ReadMe
....

=== License
This project is released under a Public Domain License.

=== Contributing
To contribute to this project please contact wxflint https://id.arduino.cc/wxflint

=== BOM Amazon Part Numbers


|===
| ID | Part name      | Part number       | Quantity
| R1 | Pressure Sensor| ASIN:B074QPD5RT   | 1
| L1 | Node MCU 2.0V  | ASIN:B010O1G1ES   | 1
| A1 | Relay          | ASIN:B0798CZDR9   | 1
|===

=== CHANGE LOG ===
V1.4 Added Start/Stop/Alarm SetPoints Code to turn pump on and off

