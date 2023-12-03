#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "||Shanmukh||";
const char* password = "123456789";
const String apiKey = "P1ESU6ZAVP7BUMTE";
const String server = "api.thingspeak.com";
const String url = "/update";

int irPin1 = D4; // Pin for IR sensor 1 (D4)
int irPin2 = D3; // Pin for IR sensor 2 (D3)

float Entering = 0; // Counter for IR sensor 1
float Leaving = 0; // Counter for IR sensor 2
float TotalEntered = 0;
float TotalLeaved = 0;
boolean state1 = true; // State flag for IR sensor 1
boolean state2 = true; // State flag for IR sensor 2

WiFiClient client;

void setup() {
  Serial.begin(9600);

  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void updateThingSpeak() {
  String data = "/update?api_key=" + apiKey +
                "&field1=" + String(Entering) +
                "&field2=" + String(TotalEntered) +
                "&field3=" + String(Leaving) +
                "&field4=" + String(Entering);

  if (client.connect(server, 80)) {
    client.println("GET " + data + " HTTP/1.1");
    client.println("Host: " + server);
    client.println("Connection: close");
    client.println();
    client.stop();
  } else {
    Serial.println("Failed to connect to ThingSpeak");
  }
}

void loop() {
  // Check for sensor 1
  if (!digitalRead(irPin1) && state1) {
    Entering++;
    TotalEntered++;
    state1 = false;
    Serial.print("ENTERING (Sensor 1): ");
    Serial.println(Entering);
    Serial.print("Total People Entered (Sensor 1): ");
    Serial.println(TotalEntered);

    // Update ThingSpeak with all field values
    updateThingSpeak();

    delay(100);
  } else {
    // Reset state flag for sensor 1 if it is not triggered
    state1 = true;
  }

  // Check for sensor 2
  if (Entering != 0 && !digitalRead(irPin2) && state2) {
    Entering--;
    Leaving++;
    state2 = false;
    Serial.print("LEAVING (Sensor 2): ");
    Serial.println(Leaving);
    Serial.print("PEOPLE IN ROOM: ");
    Serial.println(Entering);
    Serial.print("PEOPLE LEAVED ROOM(Sensor 2): ");
    Serial.println(Leaving);

    // Update ThingSpeak with all field values
    updateThingSpeak();

    delay(100);
  } else {
    // Reset state flag for sensor 2 if it is not triggered
    state2 = true;
  }
}
