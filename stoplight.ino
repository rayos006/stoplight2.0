#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
bool CYCLE = false;

void setup() {
  Serial.begin(9600);
  // Prepare PINS
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  
  // WIFI Info and connect
  WiFi.begin("", "");
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.println("Waiting to connect…");
      
  }
  
  // IP Addresses
  Serial.print("MY IP address: ");
  Serial.println(WiFi.localIP());

  // Start the HTTP Server and define the endpoints
  server.on("/",home);
  server.on("/red", red);
  server.on("/yellow", yellow);
  server.on("/green", green);
  server.on("/cycle", cycle);
  server.begin();
  Serial.println("Server listening");

}

void loop() {
  server.handleClient();
}

void off(){
  CYCLE = false;
  digitalWrite(D2, false);
  digitalWrite(D3, false);
  digitalWrite(D1, false);
  Serial.println("ALL OFF");
  server.send(200, "text/plain", "ALL off");
}

void home(){
    server.send(200, "text/plain", "Welcome");
}

void red(){
  CYCLE = false;
  digitalWrite(D2, false);
  digitalWrite(D3, false);
  digitalWrite(D1, true);
  Serial.println("RED ON");
  server.send(200, "text/plain", "RED on");
}

void yellow(){
  CYCLE = false;
  digitalWrite(D1, false);
  digitalWrite(D3, false);
  digitalWrite(D2, true);
  Serial.println("YELLOW ON");
  server.send(200, "text/plain", "YELLOW on");
}

void green(){
  CYCLE = false;
  digitalWrite(D1, false);
  digitalWrite(D2, false);
  digitalWrite(D3, true);
  Serial.println("GREEN ON");
  server.send(200, "text/plain", "GREEN on");
}

void cycle(){
  while(1){
    red();
    delay(5000);
    if(!CYCLE){
      break;
    }
    green();
    delay(5000);
    if(!CYCLE){
      break;
    }
    yellow();
    delay(2000);
    if(!CYCLE){
      break;
    }
  }
}
