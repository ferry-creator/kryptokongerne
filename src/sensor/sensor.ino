
//////////// Initiering ///////////

#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <analogWrite.h>

// Definerer id og password til netværksforbindelse som NodeMCU anvender
const char* ssid = "OnePlus 7"; //Indsæt navnet på jeres netværk her
const char* password = "55012bceb395"; //Indsæt password her

//  information tilDefinerer mqtt serveren
const char *mqtt_server = "maqiatto.com"; //navn på mqtt-server
const int mqtt_port = 1883; // Definerer porten
const char *mqtt_user = "s203775@student.dtu.dk"; // Definerer mqtt-brugeren
const char *mqtt_pass = "mqtt1234"; // Definerer koden til mqtt-brugeren
//

String payload; // payload er navnet på besked-variablen
String DATA;
const int pin_D = 5;
const int pin_A = 18;

/////// INITIERING SLUT //////////

/////// FUNKTIONSOPSÆTNING ////////

// Opretter en klient der kan forbinde til en specifik internet IP adresse.
WiFiClient espClient; // Initialiserer wifi bibloteket ESP8266Wifi

// Opretter en placeholder for callback-funktionen til brug senere
void callback(char* byteArraytopic, byte* byteArrayPayload, unsigned int length);

// Initialiserer biblioteket ift. mqtt klienten
PubSubClient client(mqtt_server, mqtt_port, callback, espClient);

/////// FUNKTIONSOPSÆTNING SLUT /////////

///////// CALLBACKFUNKTION ////////

// Definerer callback funktionen der modtager beskeder fra mqtt
// Kører hver gang MCU'en modtager en besked via mqtt

// !!!!!!!!! SIMPLIFICER SENERE, VI SKAL FAKTISK IKKE BRUGE DEN !!!!!!!!!!!
void callback(char* byteArraytopic, byte* byteArrayPayload, unsigned int length) {

  // Konverterer indkomne besked (topic) til en string:
  String topic;
  topic = String(byteArraytopic);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  // Konverterer den indkomne besked (payload) fra en array til en string:
  // Topic == Temperaturmaaler, Topic == Kraftsensor
  if (topic == "s203775@student.dtu.dk/esp32_rainSensor") { // Der subscribes til et topic nede i reconnect-funktionen. I det her tilfælde er der subscribed til "Test". Man kan subscribe til alle topics ved at bruge "#"
    payload = ""; // start payload
    for (int i = 0; i < length; i++) {
      // Udfyld payload fra byteArrayPayload
      payload += (char)byteArrayPayload[i]; 

    switch(payload) {
      case "LOW":

    }

    if (payload == "LOW")
    {
      int strength=5;
    }
    else if (payload == "STOP") 
    {
      int strength=0;
      //client.publish("s203775@student.dtu.dk/esp32_actuator", "nødstop er sat til, motor er slukket i 50 sekiunder");
      delay(50000);
    }
    else
    {
      int strength=0;
      //client.publish("s203775@student.dtu.dk/esp32_actuator", "Motor er slukket");
    }
    }
    
    //Serial.println(payload);
    //client.publish("mqtt", String(payload).c_str()); // Publish besked fra MCU til et valgt topic. Husk at subscribe til topic'et i NodeRed.
  }

}

///////// CALLBACK SLUT /////////

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
      client.subscribe("s203775@student.dtu.dk/esp32_rainSensor");
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

///////// ARDUINO EVENT LOOP ///////////
void setup() {
  Serial.begin(115200); // Åbner serial porten
  delay(1000);
  setup_wifi(); // Kører WiFi setup og forbinder herved
  client.setServer(mqtt_server, mqtt_port); // Forbinder til mqtt serveren
  //client.setCallback(callback); // Ingangsætter den definerede callback funktion hver gang der er en ny besked på den subscribede "cmd"- topic
  pinMode(pin_D, INPUT);
  pinMode(pin_A, INPUT);
}

void loop() 
{
  // Hvis der opstår problemer med forbindelsen til mqtt broker oprettes forbindelse igen ved at køre client loop
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(digitalRead(pin_D) == LOW)
  {
    client.publish("s203775@student.dtu.dk/esp32_actuator", "WET");
    Serial.println("Digital value : wet");
    delay(10);
  }
  else
  {
    client.publish("s203775@student.dtu.dk/esp32_actuator", "DRY");
    Serial.println("Digital value : dry");
    delay(10);
  }
  delay(1000);
}

///////// ARDUINO EVENT LOOP SLUT ///////////
