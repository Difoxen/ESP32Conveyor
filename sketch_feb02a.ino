#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "Conveyor";  
const char* password = "Conveyor";  
IPAddress local_ip(1,1,1,1);
IPAddress gateway(1,1,1,1);
IPAddress subnet(255,255,255,0);
WebServer server(80);
const int s0 = 19;  
const int s1 = 18;
const int out = 15;  
const int s17 = 17;  
const int s3 = 4;  
int red,blue,green,white;
uint8_t LED1pin = 2;
bool LED1status = LOW;
void setup() {
  Serial.begin(115200);
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s17, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);  
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH);  
  pinMode(LED1pin, OUTPUT);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  color();
  delay(500);
  Serial.println();
  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
}
void color()  
{    
  //red
  digitalWrite(s17, LOW);  
  digitalWrite(s3, LOW);   
  delay(50); 
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  //blue
  digitalWrite(s17, LOW);
  digitalWrite(s3, HIGH); 
  delay(50);  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  //green
  digitalWrite(s17, HIGH);
  digitalWrite(s3, HIGH);  
  delay(50);  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  //white
  digitalWrite(s17, HIGH);
  digitalWrite(s3, LOW);  
  delay(50);  
  white = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  
  //color_recognition
  if(red<blue && red<green)
  Serial.println("RED");
  else if(blue<red && blue<green)
  Serial.println("BLUE");
  else if(green<red && green<blue)
  Serial.println("GREEN");
  else if(white<10)
  Serial.println("WHITE");
  else
  Serial.println("color: Not Defined");

}
void handle_OnConnect() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status)); 
}
void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true)); 
}
void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(uint8_t led1stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Управление светодиодом</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Веб сервер</h1>\n";
    ptr +="<h3>Режим точка доступа WiFi (AP)</h3>\n";
   if(led1stat)
  {ptr +="<p>Состояние LED1: ВКЛ.</p><a class=\"button button-off\" href=\"/led1off\">ВЫКЛ.</a>\n";}
  else
  {ptr +="<p>Состояние LED1: ВЫКЛ.</p><a class=\"button button-on\" href=\"/led1on\">ВКЛ.</a>\n";}
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
