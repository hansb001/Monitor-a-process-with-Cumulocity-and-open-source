#include <ESP8266WiFi.h>
#include <PubSubClient.h>
int WET= 16; // Wet Indicator at Digital PIN D0
int DRY= 2;  // Dry Indicator at Digital PIN D4
int sense_Pin= 0; // Soil Sensor input at Analog PIN A0
int value= 0;
// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "TopWifi";
const char* password = "Rexisleuk2020!";
// Change the variable to your Gateway (Raspberry Pi) IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.68.111";
WiFiClient espClient;
PubSubClient client(espClient);

void setup_moisture() {
   Serial.begin(9600);
   pinMode(WET, OUTPUT);
   pinMode(DRY, OUTPUT);
   delay(50000);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
 }
// This functions reconnects your ESP8266 to your MQTT broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      client.subscribe("smart/factory/hum/message");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
 Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
     Serial.print("MOISTURE LEVEL : ");
   value= analogRead(sense_Pin);
   value= value/10;
   Serial.println(value);
   Serial.println("A");
   client.publish("smart/factory/hum/message", "60");
   Serial.println("message sent");
   delay(50000);
     if(value<60)
   {
      digitalWrite(WET, HIGH);
      Serial.println(WET);
      delay(50000);
      Serial.println("B");
      client.publish("smart/factory/hum/message", "100");
      delay(50000);
   }
     else
   {
      digitalWrite(DRY,HIGH);
      Serial.println(DRY);
      Serial.println("C");
      client.publish("smart/factory/hum/message", "50");
      Serial.println("message 50 sent");
      delay(50000);
   }
{
   if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");
  }
}
