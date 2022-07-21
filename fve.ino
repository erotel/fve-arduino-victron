#include <PubSubClient.h>
#include <SPI.h>
#include <Ethernet.h>
#include <avr/wdt.h>
#include <BlockNot.h>



BlockNot L1timer(120, SECONDS);
BlockNot L2timer(120, SECONDS);
BlockNot L3timer(120, SECONDS);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
String mess;
int st = 0;
int l1out = 2;
int l2out = 3;
int l3out = 5;
int stp = 0;
int pov = 128;

int stp1 = 0;
int stp2 = 0;
int stp3 = 0;

float soc;
float pin;
float l1;
float l2;
float l3;
float bv;
float bc;

//ip adresa + port victronu
const char* mqttServer = "192.168.2.5";
const int mqttPort = 1883;

byte* buffer;
 
EthernetClient ethClient;
PubSubClient client(ethClient);
 
void callback(char* topic, byte* payload, unsigned int length) {
  int i=0;
  for (int i = 0; i < length; i++) {
    }
  payload[length] = '\0';
  mess = String((char*) payload);
  mess.replace("{\"value\":","");
  mess.replace("}","");
 
 
  
  if (String(topic) == "N/48e7da85d8c3/system/0/Dc/Battery/Soc") {
      Serial.print("Soc:");
      soc = mess.toFloat(); 
      Serial.println(soc);

      if ( soc >= 98 ){
          st = 1;
      } 
       
      if ( soc <= 96 ){
          st = 0; 
      }
  }
      
  if (String(topic) == "N/48e7da85d8c3/vebus/276/Ac/ActiveIn/P") {
      Serial.print("Pin:");
      pin = mess.toFloat(); 
      Serial.println(pin);
    
  }
  if (String(topic) == "N/48e7da85d8c3/vebus/276/Ac/Out/L1/P") {
      Serial.print("L1out:");
      l1 = mess.toFloat(); 
      Serial.println(l1);
     
  }
 if (String(topic) == "N/48e7da85d8c3/vebus/276/Ac/Out/L2/P") {
      Serial.print("L2out:");
      l2 = mess.toFloat(); 
      Serial.println(l2);
     
  }
  if (String(topic) == "N/48e7da85d8c3/vebus/276/Ac/Out/L3/P") {
      Serial.print("L3out:");
      l3 = mess.toFloat(); 
      Serial.println(l3);
    
  }
  if (String(topic) == "N/48e7da85d8c3/battery/512/Dc/0/Voltage") {
      Serial.print("Bat V:");
      bv = mess.toFloat(); 
      Serial.println(bv);
     
  }
  if (String(topic) == "N/48e7da85d8c3/battery/512/Dc/0/Current") {
      Serial.print("Bat C:");
      bc = mess.toFloat(); 
      Serial.println(bc);

      if ( bc > 5 && bc < 20 ){
        pov = pov + 2;
      }

      if ( bc > 20 ){
        pov = pov + 10;
      }

      if ( bc < -5 && bc > -20 ){
        pov = pov - 2;
      }

      if ( bc < -20 ){
        pov = pov - 20;
      }
             
      if ( pov < 0 ){
        pov = 0;
      }

      if ( pov > 255 ){
        pov = 255;      
      }

      if ( soc >= 100 ){
        pov = 255;
      }
      Serial.print("Power:");
      Serial.println(pov);
    
  }
  
  Serial.println("-----------------------");
}

void pub() {
  
  client.subscribe("N/48e7da85d8c3/system/0/Dc/Battery/Soc");
  client.subscribe("N/48e7da85d8c3/vebus/276/Ac/ActiveIn/P");
  client.subscribe("N/48e7da85d8c3/vebus/276/Ac/Out/L1/P");
  client.subscribe("N/48e7da85d8c3/vebus/276/Ac/Out/L2/P");
  client.subscribe("N/48e7da85d8c3/vebus/276/Ac/Out/L3/P");
  client.subscribe("N/48e7da85d8c3/battery/512/Dc/0/Current");
  client.subscribe("N/48e7da85d8c3/battery/512/Dc/0/Voltage");

  
 
}

void setup() {
    pinMode(l1out, OUTPUT);
    pinMode(l2out, OUTPUT);
    pinMode(l3out, OUTPUT);
    
   analogWrite(l1out, 0);
   analogWrite(l1out, 0);
   analogWrite(l3out, 0);

  Serial.begin(115200);
 
  Ethernet.begin(mac);

  Serial.print(F("Connected! IP address: "));
  Serial.println(Ethernet.localIP());  
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client")) {
 
      Serial.println("connected"); 

       
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  
  pub();

}

void reconnect() {
    analogWrite(l1out, 0);
    analogWrite(l1out, 0);
    analogWrite(l3out, 0);
    
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
     
      pub();
 
      } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
 if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if ( st == 1 ) {
    boj();
  } else {
    analogWrite(l1out, 0);
    analogWrite(l1out, 0);
    analogWrite(l3out, 0);
  }
}


void boj() {

   if ( stp1 == 0 ){
    
      if ( l1 < 3000 ) {
          analogWrite(l1out, pov);
      } else {
          stp1 = 1;
          analogWrite(l1out, 0);
          L1timer.reset();
      }
  } else {
      if ( l1 > 2000 ) {
          L1timer.reset();
      }
      if ( L1timer.TRIGGERED ) {
              stp1 = 0; 
      }     
  }

 if ( stp2 == 0 ){
    
      if ( l2 < 3000 ) {
          analogWrite(l2out, pov);
      } else {
          stp2 = 1;
          analogWrite(l2out, 0);
          L2timer.reset();
      }
  } else {
      if ( l2 > 2000 ) {
          L2timer.reset();
      }
      if ( L2timer.TRIGGERED ) {
              stp2 = 0; 
      }     
  }

 if ( stp3 == 0 ){
    
      if ( l3 < 3000 ) {
          analogWrite(l3out, pov);
      } else {
          stp3 = 1;
          analogWrite(l3out, 0);
          L3timer.reset();
      }
  } else {
      if ( l3 > 2000 ) {
          L3timer.reset();
      }
      if ( L3timer.TRIGGERED ) {
              stp3 = 0; 
      }     
  }
}
