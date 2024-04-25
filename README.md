# WiFiPortal

This code can be used to make a captive portal that can make a wifi connection by the name you want 
and then makes a captive portal and would recive the password from victim and shows it in display and serial monitor.

## What you need?

1. esp8266
2. 1.3 display with SH1106(not necessary)

## Getting Started

Follow these steps to set up the WiFi captive portal on your ESP32-S2 Mini:

1. Clone or download this repository to your local machine.

2. Open the project using the Arduino IDE or PlatformIO.

3. Configure the SSID and password for your Wi-Fi network in the code.

```cpp
const char *ssid = "Guest_Wifi";
const char *password = NULL;
```

4. Customize the captive portal HTML page by modifying the `html_code.h` file. This is the web page that users will see when they connect to your network.

5. Flash the code to your ESP32-S2 Mini.

6. Once the code is uploaded, the ESP32-S2 Mini will act as a captive portal ready to catch login attempts.
