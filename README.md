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

=== License
This project is released under a Public Domain License.

=== Contributing
Please do.

=== BOM Amazon Part Numbers

Pressure Sensor| ASIN:B074QPD5RT

Node MCU 2.0V ASIN:B010O1G1ES

Relay: ASIN:B0798CZDR9


## Change log

### V1.4
- Added Start/Stop/Alarm setpoint code to turn the pump on and off.

### Recent cleanup
- Made the ESP8266 sketch more reliable by removing the blocking 5-second delay from the main loop.
- Added a WiFi connection timeout so the board won’t hang forever if the network is unavailable.
- Added SPIFFS startup checking so file-system problems are easier to spot.
- Cleaned up relay control and sensor timing logic.
- Fixed type usage in the sketch so sensor and threshold values are handled more consistently.
- Cleaned up the HTML template so it has valid IDs, better alarm/pump messaging, and safer template-variable handling.
- Fixed invalid CSS and HTML structure issues in the web page.

## What changed and why

The recent cleanup was focused on making the project easier to maintain and less likely to misbehave in normal use.

- The web server now stays responsive because sensor reading no longer blocks the loop.
- Startup is safer because WiFi and SPIFFS failures are reported instead of being ignored.
- The web page is cleaner and more reliable because it no longer depends on duplicate element IDs or missing targets.
- The tank level display is still based on the same template variables, so the existing behavior remains familiar while the implementation is more robust.
