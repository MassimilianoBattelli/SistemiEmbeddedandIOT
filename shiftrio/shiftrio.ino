#include <ESP8266WiFi.h>
#include <MQTT.h>

const char ssid[] = "yourwifiname";
const char pass[] = "yourwifipassword";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("ESP8266", "iotunibo", "Vq95BxxTQkHlkcJS")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("Topic");
}

void messageReceived(String &topic, String &payload) {
  Serial.println(topic + ": " + payload);
}

void setup() {
  Serial.begin(115200);
  // start wifi and mqtt
  WiFi.begin(ssid, pass);
  client.begin("iotunibo.cloud.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);

  // check if connected
  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    int temperature=random(-10,10);
    char cstr[2];
    itoa(temperature, cstr, 10);
    lastMillis = millis();
    client.publish("/Topic", cstr);
  }
}