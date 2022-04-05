
//////////// Initiering ///////////




//inkluderer nødvendige bibloteker
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <analogWrite.h>

// Definerer id og password til netværksforbindelse som NodeMCU anvender
const char* ssid = "OnePlus 7"; //Indsæt navnet på jeres netværk her
const char* password = "55012bceb395"; //Indsæt password her

// Definerer information til mqtt serveren
const char *mqtt_server = "maqiatto.com"; //navn på mqtt-server. Find navnet på cloudmqtt-hjemmesiden
const int mqtt_port = 1883; // Definerer porten
const char *mqtt_user = "s203775@student.dtu.dk"; // Definerer mqtt-brugeren
const char *mqtt_pass = "mqtt1234"; // Definerer koden til mqtt-brugeren
//

String payload; // Definerer variablen 'payload' i det globale scope (payload er navnet på besked-variablen)


/////// INITIERING SLUT //////////

//
//
//
//
//
//

/////// FUNKTIONSOPSÆTNING ////////


// Opretter en placeholder for callback-funktionen til brug senere. Den rigtige funktion ses længere nede.
void callback(char* byteArraytopic, byte* byteArrayPayload, unsigned int length);

// Opretter en klient der kan forbinde til en specifik internet IP adresse.
WiFiClient espClient; // Initialiserer wifi bibloteket ESP8266Wifi, som er inkluderet under "nødvendige bibloteker"

// Opretter forbindelse til mqtt klienten:
PubSubClient client(mqtt_server, mqtt_port, callback, espClient); // Initialiserer bibloteket for at kunne modtage og sende beskeder til mqtt. Den henter fra definerede mqtt server og port. Den henter fra topic og beskeden payload



// Motor control digital output pins defined as global constants
#define controlPin1A 21
#define controlPin2A 19
#define enablePin 22
  
boolean motorDirection = 1;   // 1 -> forward, 0 -> backwards
int motorSpeed = 0;           // Motor speed 0 <> 255
  
int strength = 0;             // get from app
#define strengthMax 5
#define speedMin 0
#define speedMax 255
  
int speedFromStrength(int x) {
  // equation, x in range [a,b]:
  // x(a,b) = (b-a) * ( (x - x[min]) / (x[max] - x[min]) ) + a
  // x[min] assumed 0:
  return (speedMax - speedMin) * ( x / strengthMax ) + speedMin;
}
  
void SetMotorControl()
{
  if (motorDirection == 1)          // forward
  {
    digitalWrite(controlPin1A, HIGH);
    digitalWrite(controlPin2A, LOW);
  }
  else                              // reverse
  {
    digitalWrite(controlPin1A, LOW);
    digitalWrite(controlPin2A, HIGH);
  }
  analogWrite(enablePin, motorSpeed);
}
  
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
void callback(char* byteArraytopic, byte* byteArrayPayload, unsigned int length) {

  // Konverterer indkomne besked (topic) til en string:
  String topic;
  topic = String(byteArraytopic);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  // Konverterer den indkomne besked (payload) fra en array til en string:
  // Topic == Temperaturmaaler, Topic == Kraftsensor
  if (topic == "s203775@student.dtu.dk/esp32_actuator") { // OBS: der subscribes til et topic nede i reconnect-funktionen. I det her tilfælde er der subscribed til "Test". Man kan subscribe til alle topics ved at bruge "#"
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

    if (payload == "LOW")
    {
      int strength=5;
    }
    else if (payload == "STOP") 
    {
      int strength=0;
      //client.publish("s203775@student.dtu.dk/esp32_actuator", "nødstop er sat til, motor er slukket i 50 sekunder");
      delay(50000);
    }
    else
    {
      int strength=0;
      //client.publish("s203775@student.dtu.dk/esp32_actuator", "Motor er slukket");
    }
    }
    
    Serial.println(payload);
    //client.publish("mqtt", String(payload).c_str()); // Publish besked fra MCU til et valgt topic. Husk at subscribe til topic'et i NodeRed.
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

    if (client.connect("GroupNamexMCU", mqtt_user, mqtt_pass)) { // Forbinder til klient med mqtt bruger og password
      Serial.println("connected");
      // Derudover subsribes til topic "Test" hvor NodeMCU modtager payload beskeder fra
      client.subscribe("s203775@student.dtu.dk/esp32_actuator");
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
  setup_wifi(); // Kører WiFi loopet og forbinder herved.
  client.setServer(mqtt_server, mqtt_port); // Forbinder til mqtt serveren (defineret længere oppe)
  client.setCallback(callback); // Ingangsætter den definerede callback funktion hver gang der er en ny besked på den subscribede "cmd"- topic

  // Declare digital output pins:
  pinMode(controlPin1A, OUTPUT);    // 1A
  pinMode(controlPin2A, OUTPUT);    // 2A
  pinMode(enablePin, OUTPUT);       // EN1,2
   
  digitalWrite(enablePin, LOW);     // motor off
}
//////// SETUP SLUT ////////

//
//
//
//
//
//

/////// LOOP /////////

void loop() 
{
  // Hvis der opstår problemer med forbindelsen til mqtt broker oprettes forbindelse igen ved at køre client loop
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  motorDirection = 1;
  motorSpeed = speedFromStrength(strength);
  SetMotorControl();
    
  delay(1000);
}

//////// Loop slut ////////
