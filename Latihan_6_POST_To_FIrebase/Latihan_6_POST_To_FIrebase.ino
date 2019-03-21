#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>
#include <NewPing.h>

#define TRIGGER_PIN  D2  
#define ECHO_PIN     D3  
#define MAX_DISTANCE 50 

NewPing cm(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 


// Set these to run example.
#define FIREBASE_HOST "iwanduinobot9.firebaseio.com"
#define FIREBASE_AUTH "bWU63XRZT55sryuJaJZDeEzdOxzEHj60aFL9gmcn"
#define WIFISSID "DILo Bogor" // Assign your WiFi SSID
#define PASSWORD  "DILo Bogor" // Assign your WiFi password

#define DHTPIN D1   
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

const int relay1 = D5;
const int relay2 = D6;
const int relay3 = D7;
const int sensor = A0;
/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(sensor, INPUT);

  WiFi.begin(WIFISSID, PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("relay1", 0);
  Firebase.set("relay2", 0);
  Firebase.set("relay3", 0);


  Firebase.set("cahaya", 0);
  Firebase.set("Jarak", 0);
  
}

void loop() {
  digitalWrite(relay1, Firebase.getInt("relay1"));
  digitalWrite(relay2, Firebase.getInt("relay2"));
  digitalWrite(relay3, Firebase.getInt("relay3"));
  
  int cahaya=analogRead(sensor);
  int jarak=cm.ping_cm();
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();
  
  //POST DATA SERIAL
  Serial.print("SUHU :");Serial.print(suhu);Serial.print(" | ");
  Serial.print("KELEMBABAN :");Serial.print(kelembaban);Serial.print(" | ");
  Serial.print("CAHAYA :");Serial.print(cahaya);Serial.print(" | ");
  Serial.print("JARAK :");Serial.print(jarak);Serial.print(" | ");
  Serial.print("RELAY 1 :");Serial.print(Firebase.getInt("relay1"));Serial.print(" | ");
  Serial.print("RELAY 2 :");Serial.print(Firebase.getInt("relay2"));Serial.print(" | ");
  Serial.print("RELAY 3 :");Serial.print(Firebase.getInt("relay3"));
  Serial.println();
  //POST DATA CLOUD
  Firebase.setFloat("suhu", suhu);
  Firebase.setFloat("kelembaban", kelembaban);
  Firebase.setFloat("cahaya", cahaya);
  Firebase.setFloat("Jarak", jarak);
 // delay(500);
}
