/*
This example will open a configuration portal for 60 seconds when first powered up. ConfigOnSwitch is a a bettter example for most situations but this has the advantage 
that no pins or buttons are required on the ESP8266 device at the cost of delaying 
the user sketch for the period that the configuration portal is open.
 
Also in this example a password is required to connect to the configuration portal 
network. This is inconvenient but means that only those who know the password or those 
already connected to the target WiFi network can access the configuration portal and 
the WiFi network credentials will be sent from the browser over an encrypted connection and
can not be read by observers.
 
*/
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
 
//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager
#include <FS.h>

// Onboard LED I/O pin on NodeMCU board
const byte PIN_LED = 2; // D4 on NodeMCU and WeMos. Controls the onboard LED.
const byte interruptPin = 5; //D2 on NodeMCU. Push button Interrupt.
volatile byte interruptCounter = 0;
int numberOfInterrupts = 0;
 
void setup() {
  // put your setup code here, to run once:
  // initialize the LED digital pin as an output.
  pinMode(PIN_LED, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  
  Serial.begin(115200);
  Serial.println("\n Starting");
  unsigned long startedAt = millis();
 
  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
  Serial.println("Opening configuration portal");
  digitalWrite(PIN_LED, LOW); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.
  //Local intialization. Once its business is done, there is no need to keep it around
 
  WiFiManager wifiManager;  
  //if(wifiManger.getSSID() == ""){
    //sets timeout in seconds until configuration portal gets turned off.
    //If not specified device will remain in configuration mode until
    //switched off via webserver.
    Serial.println("SSID: ");
    Serial.println(WiFi.SSID());
    if (WiFi.SSID()!="") wifiManager.setConfigPortalTimeout(120); //If no access point name has been previously entered disable timeout.
   
    //it starts an access point 
    //and goes into a blocking loop awaiting configuration
    if (!wifiManager.startConfigPortal("ESP8266SW","password"))  //Delete these two parameters if you do not want a WiFi password on your configuration access point
    {
       Serial.println("Not connected to WiFi but continuing anyway.");
    } 
    else 
    {
       //if you get here you have connected to the WiFi
       Serial.println("connected...yeey :)");
       //Serial.println("SSID: ");
       //Serial.println(wifiManager.getSSID());
       //Serial.println("Password: ");
       //Serial.println(wifiManager.getPassword());
       Serial.println("*************");
       WiFi.softAPdisconnect(true);
    }
      digitalWrite(PIN_LED, HIGH); // Turn led off as we are not in configuration mode.
      // For some unknown reason webserver can only be started once per boot up 
      // so webserver can not be used again in the sketch.
 
  Serial.print("After waiting ");
  int connRes = WiFi.waitForConnectResult();
  float waited = (millis()- startedAt);
  Serial.print(waited/1000);
  Serial.print(" secs in setup() connection result is ");
  Serial.println(connRes);
  if (WiFi.status()!=WL_CONNECTED)
  {
      Serial.println("failed to connect, finishing setup anyway");
  } 
  else
  {
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());
  }
}

void handleInterrupt(){
   Serial.println("Reset WiFi!");
   WiFi.disconnect(true);
   delay(500);
}
 
void loop() {
 delay(10000);
  // put your main code here, to run repeatedly:
 
}
