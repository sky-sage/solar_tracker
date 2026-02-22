#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <ESPmDNS.h>
#include "webpage.h"

/* ================== GLOBAL ================== */
volatile double panelVoltage = 0.0;
volatile int servoAngle = 90;
volatile int ldrValues[4] = {0,0,0,0};
volatile bool manualMode = false;
volatile int manualAngle = 90;

#define HISTORY 60
volatile float voltageHistory[HISTORY];
volatile int historyIndex = 0;

const float assumedCurrent = 0.8;

/* ================== SERVO ================== */
Servo myServo;
int currentServoPos = 90;
const int servoPin = 25;

/* ================== LDR CONFIG ================== */
const int ldrPins[4]   = {34,35,32,33};
const int ldrAngles[4] = {30,70,110,150};
const int ldrMax[4]    = {2000,3000,2700,1000};

/* ================== WIFI ================== */
const char* ssid = "ESP32-Solar";
const char* password = "12345678";
WebServer server(80);

/* ================== API ================== */
void handleRoot(){ server.send_P(200,"text/html",webpage); }

void handleData(){
 String json="{";
 json+="\"voltage\":"+String(panelVoltage,2)+",";
 json+="\"power\":"+String(panelVoltage*assumedCurrent,2)+",";
 json+="\"angle\":"+String(servoAngle)+",";
 json+="\"mode\":\""+String(manualMode?"manual":"auto")+"\",";
 json+="\"ldr\":["+String(ldrValues[0])+","+String(ldrValues[1])+","+String(ldrValues[2])+","+String(ldrValues[3])+"]";
 json+="}";
 server.send(200,"application/json",json);
}

void handleHistory(){
 String j="[";
 for(int i=0;i<HISTORY;i++){
  j+=String(voltageHistory[i],2);
  if(i<HISTORY-1) j+=",";
 }
 j+="]";
 server.send(200,"application/json",j);
}

void handleControl(){
 if(server.hasArg("mode"))
  manualMode = server.arg("mode")=="manual";
 if(server.hasArg("angle"))
  manualAngle = constrain(server.arg("angle").toInt(),0,180);
 server.send(200,"text/plain","OK");
}

/* ================== CORE 0 ================== */
void core0Task(void* pv){
 analogReadResolution(12);
 analogSetAttenuation(ADC_11db);

 myServo.setPeriodHertz(50);
 myServo.attach(servoPin,500,2400);
 myServo.write(currentServoPos);

 while(true){

  float weightedSum=0,totalWeight=0;

  for(int i=0;i<4;i++){
   int raw=analogRead(ldrPins[i]);
   raw=constrain(raw,0,ldrMax[i]);
   ldrValues[i]=raw;
   float weight=(float)raw/ldrMax[i];
   weightedSum+=weight*ldrAngles[i];
   totalWeight+=weight;
  }

  if(!manualMode && totalWeight>0)
   servoAngle=weightedSum/totalWeight;

  if(manualMode)
   servoAngle=manualAngle;

  servoAngle=constrain(servoAngle,0,180);

  if(currentServoPos<servoAngle) currentServoPos++;
  else if(currentServoPos>servoAngle) currentServoPos--;

  myServo.write(currentServoPos);

  int rawADC=analogRead(36);
  double vadc=(rawADC/4095.0)*3.3;
  panelVoltage=constrain(vadc*11.0,0.0,12.0);

  voltageHistory[historyIndex]=panelVoltage;
  historyIndex=(historyIndex+1)%HISTORY;

  vTaskDelay(pdMS_TO_TICKS(20));
 }
}

/* ================== CORE 1 ================== */
void core1Task(void* pv){
 WiFi.softAP(ssid,password);

 if(MDNS.begin("esp32-solar"))
  Serial.println("Open: http://esp32-solar.local");

 server.on("/",handleRoot);
 server.on("/data",handleData);
 server.on("/history",handleHistory);
 server.on("/control",handleControl);
 server.begin();

 while(true){
  server.handleClient();
  vTaskDelay(1);
 }
}

/* ================== SETUP ================== */
void setup(){
 Serial.begin(115200);
 pinMode(36,INPUT);

 xTaskCreatePinnedToCore(core0Task,"Core0",4096,NULL,1,NULL,0);
 xTaskCreatePinnedToCore(core1Task,"Core1",8192,NULL,1,NULL,1);
}

void loop(){}