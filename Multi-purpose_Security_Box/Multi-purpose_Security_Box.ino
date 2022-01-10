#include <SPI.h>
#include <ezTime.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

#include "arduino_secrets.h"

int ledPin = 9; // choose the pin for the LED
int Buzzer = 10; // choose the pin for the Buzzer
int inputPin = 7; // choose the input pin (for PIR sensor)
int pirState = LOW; // we start, assuming no motion detected
int val = 0; // variable for reading the pin status
int count = 0;
uint8_t h = hour(); // initial hour for resetCount() function
int thres = 400; // set the threshould to activate the system


// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqttuser = SECRET_MQTTUSER;
const char* mqttpass = SECRET_MQTTPASS;

const char* mqtt_server = "mqtt.cetools.org";

WiFiClient espClient;
PubSubClient client(espClient);

char msg[50];

Timezone GB;

void setup() {

   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  ///lcd.print("hello, world!");
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  startWifi();
  syncDate();
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);


  pinMode(ledPin, OUTPUT); // declare LED as output
  pinMode(Buzzer, OUTPUT); // declare Buzzer as output
  pinMode(inputPin, INPUT); // declare sensor as input
  
  syncDate();

  // start MQTT server
  client.setServer(mqtt_server, 1884);
  client.setCallback(callback);


}

void loop(){
  
  resetCount();
  val = digitalRead(inputPin); // read input value
  int value_ldr = analogRead(A0); // read LDR value
  Serial.println(value_ldr);
  Serial.println(count);
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(value_ldr);
  lcd.print(" ");
  lcd.setCursor(7, 0);
  lcd.print("Alarm:");
  if((value_ldr < thres)){
    lcd.print("ON ");
    if ((value_ldr < thres) && (val == HIGH)){
    if (val == HIGH) { 
      // check if the input is HIGH
      digitalWrite(ledPin, HIGH); // turn LED ON
      tone(Buzzer, 272); // turn Buzzer ON
      delay(2000);
      count += 1;
      if (pirState == LOW) {
        // we have just turned on
        Serial.println("Motion detected!");
        // We only want to print on the output change, not state
        pirState = HIGH;
      }
    } 
    }else {
      digitalWrite(ledPin, LOW); // turn LED OFF
      noTone(Buzzer); // turn Buzzer OFF
      if (pirState == HIGH){
        // we have just turned of
        Serial.println("Motion ended!");
        // We only want to print on the output change, not state
        pirState = LOW;
      }
    }
  }else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    noTone(Buzzer); // turn Buzzer OFF
    lcd.print("OFF");
  }
  lcd.setCursor(0, 1);
  lcd.print("Motion:");
  lcd.print(count);

  if (minuteChanged()) {
      sendMQTT();
      Serial.println(GB.dateTime("H:i:s")); // UTC.dateTime("l, d-M-y H:i:s.v T")
  }
    client.loop();
}

void resetCount(){
  if (h != hour()){
    count = 0;  // reset count if hour change to statistic motion per hour.
    h = hour(); // update hour
  }
}

void startWifi(){
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // check to see if connected and wait until you are
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// This function is used to make sure the arduino is connected
// to an MQTT broker before it tries to send a message and to 
// keep alive subscriptions on the broker (ie listens for inTopic)


void syncDate() {
  // get real date and time
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  GB.setLocation("Europe/London");
  Serial.println("London time: " + GB.dateTime());

}

void sendMQTT() {
 // try to reconnect if not
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  snprintf (msg, 50, "%.0i", count);
  Serial.print("Publish message for count: ");
  Serial.println(msg);
  client.publish("student/CASA0016/ucfnzc1/detect_motion", msg);
 

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {    // while not (!) connected....
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ArduinoUnoWiFiRev2Client-";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttuser, mqttpass)) {
      Serial.println("connected");
      // ... and subscribe to messages on broker
      client.subscribe("student/CASA0016/inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// Execute code when a message is recieved from the MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();


}
