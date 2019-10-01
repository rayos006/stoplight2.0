#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
bool CYCLE = false;
unsigned long TIME_NOW = 0;
int LONG_PERIOD = 5000;
int SHORT_PERIOD = 2000;
char STATUS[] = "off";

void setup()
{
  Serial.begin(9600);
  // Prepare PINS
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

  // WIFI Info and connect
  WiFi.begin("", "");
  while (WiFi.status() != WL_CONNECTED)
  {

    delay(500);
    Serial.println("Waiting to connect…");
  }

  // IP Addresses
  Serial.print("MY IP address: ");
  Serial.println(WiFi.localIP());

  // Start the HTTP Server and define the endpoints
  server.on("/off", off);
  server.on("/red", red);
  server.on("/yellow", yellow);
  server.on("/green", green);
  server.on("/cycle", cycle);
  server.on("/status", status_check);
  server.begin();
  Serial.println("Server listening");
}

void loop()
{
  server.handleClient();
  if(CYCLE){
    if(millis() > TIME_NOW + LONG_PERIOD){
      TIME_NOW = millis();
      red();
    }
    if(millis() > TIME_NOW + LONG_PERIOD){
      TIME_NOW = millis();
      green();
    }
    if(millis() > TIME_NOW + SHORT_PERIOD){
      TIME_NOW = millis();
      yellow();
    }
  }
}

void off()
{
  CYCLE = false;
  digitalWrite(D2, false);
  digitalWrite(D3, false);
  digitalWrite(D1, false);
  Serial.println("ALL OFF");
}

void red()
{
  digitalWrite(D2, false);
  digitalWrite(D3, false);
  digitalWrite(D1, true);
  Serial.println("RED ON");
  return_html();
}

void yellow()
{
  digitalWrite(D1, false);
  digitalWrite(D3, false);
  digitalWrite(D2, true);
  Serial.println("YELLOW ON");
  return_html();
}

void green(){
  digitalWrite(D1, false);
  digitalWrite(D2, false);
  digitalWrite(D3, true);
  Serial.println("GREEN ON");
  return_html();
}

void status_check(){
  server.send(200, "text/plain", STATUS);
}

void cycle(){
  CYCLE = true;
}

void return_html(){
  server.send(200, "text/html", "<!DOCTYPE html><html><head><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script></head><header><title>Stoplight</title></header><style>.dot {height: 25px;width: 25px;border-radius: 50%;display: inline-block;}.red {background-color: rgb(169, 9, 9);}.yellow {background-color: rgb(244, 236, 15);}.green {background-color: rgb(10, 121, 20);}.off {background-color: rgb(255, 255, 255);}</style><body><h3>Welcome to the Stoplight!</h3><br /><!-- Buttons to change state manually --><a href='/off'><button>Off</button></a><br /><a href='/red'><button>Red</button></a><br /><a href='/yellow'><button>Yellow</button></a><br /><a href='/green'><button>Green</button></a><br /><!-- Button to start cycle of lights --><a href='/cycle'><button>Run Light</button></a><br /><br /><!-- Current state of light --><script>setInterval(function() {$.ajax({type: 'get',url: '/status',datatype: 'json',success: function(data) {console.log(data);$('#dot').removeClass('green red yellow').addClass(data);}});}, 1000); //time in milliseconds</script><span id='dot' class='dot'></span></body></html>")
}
