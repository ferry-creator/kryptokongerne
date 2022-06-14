
//////////// Initiering ///////////

//inkluderer nødvendige bibloteker
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

//Setting servo name
Servo servo;

//Defining pins for pump
int motorPin1 = 12;
int motorPin2 = 14;


// Definerer id og password til netværksforbindelse som NodeMCU anvender
const char* ssid = "OnePlus 7"; //Indsæt navnet på jeres netværk her
const char* password = "55012bceb395"; //Indsæt password her

// Definerer information til mqtt serveren
const char *mqtt_server = "hairdresser.cloudmqtt.com"; //navn på mqtt-server. Find navnet på cloudmqtt-hjemmesiden
const int mqtt_port = 15687; // Definerer porten
const char *mqtt_user = "jnyhgkwt"; // Definerer mqtt-brugeren
const char *mqtt_pass = "Zwu25yVhkqBN"; // Definerer koden til mqtt-brugeren
//

String payload; // Definerer variablen 'payload' i det globale scope (payload er navnet på besked-variablen)

//Defining pins for Sensor
const int inRange_trigPin = 23;
const int inRange_echoPin = 22;



/////// INITIERING SLUT //////////

//
//
//
//
//
//

/////// FUNKTIONSOPSÆTNING ////////


// Opretter en placeholder for callback-funktionen til brug senere. Den rigtige funktion ses længere nede.
void callback(char* topic, byte* Payload, unsigned int length);

// Opretter en klient der kan forbinde til en specifik internet IP adresse.
WiFiClient espClient; // Initialiserer wifi bibloteket ESP8266Wifi, som er inkluderet under "nødvendige bibloteker"

// Opretter forbindelse til mqtt klienten:
PubSubClient client(mqtt_server, mqtt_port, callback, espClient); // Initialiserer bibloteket for at kunne modtage og sende beskeder til mqtt. Den henter fra definerede mqtt server og port. Den henter fra topic og beskeden payload

/////// FUNKTIONSOPSÆTNING SLUT /////////

//
//
//
//
//
//

///////// CALLBACKFUNKTION ////////

// Definerer callback funktionen der modtager beskeder fra mqtt
// OBS: den her funktion kører hver gang MCU'en modtager en besked via mqtt
void callback(char* topic, byte* Payload, unsigned int length) {
  StaticJsonDocument<256> doc;
  deserializeJson(doc, Payload, length);
  StaticJsonDocument<256> doc2;
  doc2["STATUS"] = doc["STATUS"];

  // Konverterer indkomne besked (topic) til en string:
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)Payload[i]);
  }
  Serial.println();

/*  
  // Konverterer den indkomne besked (payload) fra en array til en string:
  // Topic == Temperaturmaaler, Topic == Kraftsensor
  if (topic == "mathiashovmark@gmail.com/Mekatronik21") { // OBS: der subscribes til et topic nede i reconnect-funktionen. I det her tilfælde er der subscribed til "Test". Man kan subscribe til alle topics ved at bruge "#"
    payload = ""; // Nulstil payload variablen så forloopet ikke appender til en allerede eksisterende payload
    for (int i = 0; i < length; i++) {
      payload += (char)byteArrayPayload[i];
      // For-loop: tag hvert tegn i hele længden af den inkomne besked, og konverter denne til en char. Append tegnene 1 efter 1:
      // Eksempel:
      // Besked = Study Abroad
      // Length = 12
      // Loop 1 = "S"
      // Loop 2 = "St" osv.
      // Loop (length) = "Study Abroad"
*/

      if (doc["type"] == "heightSensor") {
        // Definerer funktion
        delay(1000);   
        doc2["type"] = "heightSensor";
        JsonArray data = doc2.createNestedArray("data");        
        for (int k = 0; k < doc["reps"]; k++) {
          digitalWrite(inRange_trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(inRange_trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(inRange_trigPin, LOW);
          int maal = 0.01715 * pulseIn(inRange_echoPin, HIGH);
          data.add(maal);
          Serial.print(maal);
          delay(doc["freq"]);
          }
        char buffer[256];
        int b=serializeJson(doc2, buffer);
        client.publish("node-red", buffer);
        Serial.print(b);            
        }
      
      if ((doc["type"]) == "flour") {
        // Definerer funktion ved indput Flour
        delay(2000);  
        doc2["type"] = "flour";
        doc2["response"] = "OK";
        for (int j = 0; j <= doc["reps"]; j++) {
          servo.write(0);
          delay(500);
          delay(5000);
          servo.write(90);
          delay(200);
          delay(5000);
          servo.write(0);
          delay(doc["freq"]);
        }
        char buffer[256];
        int b=serializeJson(doc2, buffer);
        client.publish("node-red", buffer);
        Serial.print(b);   
      }
        
      if ((doc["type"])== "water") {
        doc2["type"] = "water";
        doc2["response"] = "OK";
        // Definerer funktion ved indput mix
        delay(2000);
        // Turn the motor in one direction
        digitalWrite(motorPin1, LOW); 
        digitalWrite(motorPin2, HIGH); 
        delay(doc["freq"]) ;
        //For brake
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW); 
        delay(1000);    
        char buffer[256];
        int b=serializeJson(doc2, buffer);
        client.publish("node-red", buffer);
        Serial.print(b);   
   
  }


  
}

///////// CALLBACK SLUT /////////

//
//
//
//
//
//

/////// OPSÆTNING AF WIFI-FORBINDELSE  ///////////


// Opretter forbindelse til WiFi
void setup_wifi() {
  // Forbinder til et WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


// Opretter forbindelse til mqtt server. Dette gentages ved manglende forbindelse til WiFi, server osv.
void reconnect() {
  // Fortsætter til forbindelsen er oprettet
  while (!client.connected()) {
    Serial.print("Forsøger at oprette MQTT forbindelse...");

    if (client.connect("krypto", mqtt_user, mqtt_pass)) { // Forbinder til klient med mqtt bruger og password
      Serial.println("connected");
      // Derudover subsribes til topic "Test" hvor NodeMCU modtager payload beskeder fra
      client.subscribe("dispenser");
      // Der kan subscribes til flere specifikke topics
      //client.subscribe("Test1");
      // Eller til samtlige topics ved at bruge '#' (Se Power Point fra d. 18. marts)
      // client.subscribe("#");

      // Hvis forbindelsen fejler køres loopet igen efter 5 sekunder indtil forbindelse er oprettet
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Venter 5 sekunder før den prøver igen
      delay(5000);
    }
  }
}

///////// OPSÆTNING AF WIFI SLUT /////////

//
//
//
//
//
//

///////// SETUP ///////////
void setup() {

  Serial.begin(115200); // Åbner serial porten og sætter data raten til 115200 baud
  delay(1000);
  //setting servo pin to pin 2
  servo.attach(2);
  // Set the pin modes of the motor pins to OUTPUT
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  // Set the pin modes of the trig and ecco pins to Output and input
  pinMode(inRange_trigPin, OUTPUT);
  pinMode(inRange_echoPin, INPUT);
  setup_wifi(); // Kører WiFi loopet og forbinder herved.
  client.setServer(mqtt_server, mqtt_port); // Forbinder til mqtt serveren (defineret længere oppe)
  client.setCallback(callback); // Ingangsætter den definerede callback funktion hver gang der er en ny besked på den subscribede "cmd"- topic


}
//////// SETUP SLUT ////////

//
//
//
//
//
//

/////// LOOP /////////

void loop() {

  // Hvis der opstår problemer med forbindelsen til mqtt broker oprettes forbindelse igen ved at køre client loop
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(1000);
}

//////// Loop slut ////////
