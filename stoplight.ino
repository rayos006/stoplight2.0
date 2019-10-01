#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string.h>

ESP8266WebServer server(80);
bool CYCLE = false;
unsigned long PREV_TIME = 0;
const long LONG_PERIOD = 5000;
const long SHORT_PERIOD = 1000;
char LED[] = "off";

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
  server.on("/red", red_call);
  server.on("/yellow", yellow_call);
  server.on("/green", green_call);
  server.on("/cycle", cycle);
  server.on("/status", status_check);
  server.begin();
  Serial.println("Server listening");
}

void loop(){
  unsigned long curr_time = millis();
  server.handleClient();
  if(CYCLE){
    //Check if light is off or yellow
    if(strcmp(LED,"off") == 0 || strcmp(LED,"yellow") == 0){
      if (curr_time - PREV_TIME >= SHORT_PERIOD) {
        red();
        // save the "current" time
        PREV_TIME = curr_time;
      }
    }
    // Check if light is Red
    else if(strcmp(LED,"red") == 0){
      if (curr_time - PREV_TIME >= LONG_PERIOD) {
        green();
        // save the "current" time
        PREV_TIME = curr_time;
      }
    }
    //Check if light is green
    else if(strcmp(LED,"green") == 0){
      
      if (curr_time - PREV_TIME >= LONG_PERIOD) {
        yellow();
        // save the "current" time
        PREV_TIME = curr_time;
      }
    }
  }
}
// Turns off the cycle and all leds
void off()
{
  CYCLE = false;
  digitalWrite(D2, false);
  digitalWrite(D3, false);
  digitalWrite(D1, false);
  Serial.println("ALL OFF");
  strcpy(LED, "off");
  return_html();
}
// Turns on the red LED
void red()
{
  digitalWrite(D2, false);
  digitalWrite(D3, false);
  digitalWrite(D1, true);
  Serial.println("RED ON");
  strcpy(LED, "red");
}
// Turns on the yellow LED
void yellow()
{
  digitalWrite(D1, false);
  digitalWrite(D3, false);
  digitalWrite(D2, true);
  Serial.println("YELLOW ON");
  strcpy(LED, "yellow");
}
// Turns on the green LED
void green(){
  digitalWrite(D1, false);
  digitalWrite(D2, false);
  digitalWrite(D3, true);
  Serial.println("GREEN ON");
  strcpy(LED, "green");
}

// Status check for front end
void status_check(){
  server.send(200, "text/plain", LED);
}

// Start cycle
void cycle(){
  CYCLE = true;
  return_html();
}

// Turns off cycle and turns on the green light
void green_call(){
  CYCLE = false;
  green();
  return_html();
}
// Turns off cycle and turns on the red light
void red_call(){
  CYCLE = false;
  red();
  return_html();
}
// Turns off cycle and turns on the yellow light
void yellow_call(){
  CYCLE = false;
  yellow();
  return_html();
}
// returns the HTML page
void return_html(){
  server.send(200, "text/html", "<!DOCTYPE html><html><head><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script></head><header><title>Stoplight</title></header><style>.dot {height: 25px;width: 25px;border-radius: 50%;display: inline-block;}.red {background-color: rgb(169, 9, 9);}.yellow {background-color: rgb(244, 236, 15);}.green {background-color: rgb(10, 121, 20);}.off {background-color: rgb(255, 255, 255);}</style><body><h3>Welcome to the Stoplight!</h3><br /><!-- Buttons to change state manually --><a href='/off'><button>Off</button></a><br /><a href='/red'><button>Red</button></a><br /><a href='/yellow'><button>Yellow</button></a><br /><a href='/green'><button>Green</button></a><br /><!-- Button to start cycle of lights --><a href='/cycle'><button>Run Light</button></a><br /><br /><!-- Current state of light --><script>setInterval(function() {$.ajax({type: 'get',url: '/status',datatype: 'json',success: function(data) {console.log(data);$('#dot').removeClass('green red yellow').addClass(data);}});}, 1000); //time in milliseconds</script><span id='dot' class='dot'></span></body></html>");
}
