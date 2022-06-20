//////////// Initiering ///////////

//inkluderer nødvendige bibloteker
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

#define hivemq_host "d442f1008e514e098cb3126d115cb356.s1.eu.hivemq.cloud"
#define hivemq_user "krypto"
#define hivemq_pass "hivemq1234"
#define hivemq_port 8883

#define cloudmqtt_host "hairdresser.cloudmqtt.com"
#define cloudmqtt_user "jnyhgkwt"
#define cloudmqtt_pass "Zwu25yVhkqBN"
#define cloudmqtt_port 15687

#define maqia_host "maqiatto.com"
#define maqia_user "s203773@student.dtu.dk"
#define maqia_pass "alex123"
#define maqia_port 1883

struct mqttBroker {
  char *host;
  char *user;
  char *pass;
  int   port;
};

const mqttBroker cloudmqtt = { cloudmqtt_host, cloudmqtt_user, cloudmqtt_pass, cloudmqtt_port };
const mqttBroker hivemq    = { hivemq_host, hivemq_user, hivemq_pass, hivemq_port };
const mqttBroker maqiatto  = { maqia_host, maqia_user, maqia_pass, maqia_port };

// Change this to change broker
const mqttBroker MQTT = maqiatto;
// set true for hiveMQ
const bool SSL_ENABLED = false;

//////////// HiveMQ TSL ///////////
const char* root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "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\n" \
     "nLRbwHOoq7hHwg==\n" \
     "-----END CERTIFICATE-----\n";



//////////////////////////////////

//Setting servo name
Servo servo;

//Defining pins for pump
int motorPin1 = 12;
int motorPin2 = 14;

// Definerer id og password til netværksforbindelse som NodeMCU anvender
const char* ssid = "OnePlus 7"; //Indsæt navnet på jeres netværk her
const char* password = "55012bceb395"; //Indsæt password her

String payload; // Definerer variablen 'payload' i det globale scope (payload er navnet på besked-variablen)

//Defining pins for Sensor
const int inRange_trigPin = 27;
const int inRange_echoPin = 26;



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
WiFiClientSecure espClient; // Initialiserer wifi bibloteket ESP8266Wifi, som er inkluderet under "nødvendige bibloteker"

if(SSL_ENABLED) {
  espClient.setCACert(root_ca);
}

// Opretter forbindelse til mqtt klienten:
PubSubClient client(MQTT.host, MQTT.port, callback, espClient); // Initialiserer bibloteket for at kunne modtage og sende beskeder til mqtt. Den henter fra definerede mqtt server og port. Den henter fra topic og beskeden payload

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
  doc2["status"] = doc["status"];

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
        client.publish("s203773@student.dtu.dk/node-red", buffer);
        Serial.print(b);            
        }
      
      if ((doc["type"]) == "flour") {
        // Definerer funktion ved indput Flour
        delay(2000);  
        doc2["type"] = "flour";
        doc2["response"] = "OK";
        for (int j = 0; j < doc["reps"]; j++) {
          servo.write(0);
          delay(200);
          delay(2000);
          servo.write(90);
          delay(200);
          delay(2000);
          servo.write(0);
          delay(doc["freq"]);
        }
        char buffer[256];
        int b=serializeJson(doc2, buffer);
        client.publish("s203773@student.dtu.dk/node-red", buffer);
        Serial.print(b);   
      }
        
      if ((doc["type"])== "water") {
        doc2["type"] = "water";
        doc2["response"] = "OK";
        // Definerer funktion ved indput water
        delay(1000);
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
        client.publish("s203773@student.dtu.dk/node-red", buffer);
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

    if (client.connect("krypto", MQTT.user, MQTT.pass)) { // Forbinder til klient med mqtt bruger og password
      Serial.println("connected");
      // Derudover subsribes til topic "Test" hvor NodeMCU modtager payload beskeder fra
      client.subscribe("s203773@student.dtu.dk/MCU");
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
  servo.attach(25);
  // Set the pin modes of the motor pins to OUTPUT
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  // Set the pin modes of the trig and ecco pins to Output and input
  pinMode(inRange_trigPin, OUTPUT);
  pinMode(inRange_echoPin, INPUT);
  setup_wifi(); // Kører WiFi loopet og forbinder herved.
  client.setServer(MQTT.host, MQTT.port); // Forbinder til mqtt serveren (defineret længere oppe)
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
