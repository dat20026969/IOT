#include <Wire.h>
#include <WiFi.h>
#include "pitch.h"
#include "PubSubClient.h"
#include "ThingSpeak.h"

int melody1[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
int melody2[] = {NOTE_G3,NOTE_C4,NOTE_G3,NOTE_C4,NOTE_G3,NOTE_C4,NOTE_G3,NOTE_C4};
int noteDurations[] = { 4, 8, 8, 4,4,4,4,4 };

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.mqtt-dashboard.com";
int port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long counterChannelNumber	= 1995829;         // Channel ID
const char * myCounterReadAPIKey 	= "AMFF5V6HG4GEFMNX"; 	// Read API Key
const int FieldNumber1 				= 1;  				// The field you wish to read

double statusCode;

void wifiConnect(){
  WiFi.begin(ssid, password);
  while (WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  ThingSpeak.begin(espClient);
}

void mqttReconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    if(client.connect("19122012")){
      Serial.println(" connected!");
      client.subscribe("topicName/gas");
    }
    else{
      Serial.println(" try again in 10 seconds.");
      delay(10000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length){
  Serial.println(topic);
  String stMessage;
  for (int i=0;i<length;i++){
    stMessage += (char)message[i];
  }
  Serial.println(stMessage);
}

void readData(){
  float gas1	= ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
  statusCode	= ThingSpeak.getLastReadStatus();
  if (statusCode == 200){
     Serial.print("Gas: ");
     Serial.print(gas1);
     Serial.print("°");
     Serial.print("\n");
  }
  else{
    Serial.println("Unable to read channel / No internet connection");
  }
  delay(50);  
}

void setup() {
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(15, INPUT);
  pinMode(5, INPUT);
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(4, melody1[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);    
    noTone(4);
  }
  Serial.begin(115200); 
  Serial.print("Connecting to WiFi...");

  wifiConnect();

  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, port);
  client.setCallback(callback);
}

void loop() {
  readData();
  if(!client.connected()){
    mqttReconnect();
  }
  client.loop();
  digitalWrite(2, digitalRead(15));
  int gas = random(0, 100);
  char buffer[50];
  sprintf(buffer, "%d", gas);
  client.publish("topicName/gas", buffer);
  delay(500);
  if (digitalRead(15)>=100){
    for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(4, melody2[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);    
    noTone(4);
    }
  }
  delay(500);
}
