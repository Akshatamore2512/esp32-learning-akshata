#include <WiFi.h>
#include <DHT.h>
 
// WiFi network credentials
const char* ssid     = "yourssid";
const char* password = "yourpasswd";
 
// Define sensor and LED pins
#define LED_PIN 5
#define DHTPIN 4
#define DHTTYPE DHT22  // Use DHT11 if that's your sensor type
 
DHT dht(DHTPIN, DHTTYPE);
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
 
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  // Start the server
  server.begin();
}
 
void loop() {
  WiFiClient client = server.available();
 
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
 
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
 
        if (c == '\n') {
          if (currentLine.length() == 0) {
            float humidity = dht.readHumidity();
            float temperature = dht.readTemperature();
 
            // Send HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
 
            client.println("<!DOCTYPE html><html><head><meta charset='utf-8'><title>ESP32 Temp & Humidity</title></head><body>");
            client.println("<h1>ESP32 Temperature and Humidity</h1>");
 
            if (isnan(humidity) || isnan(temperature)) {
              client.println("<p>Failed to read from DHT sensor!</p>");
            } else {
              client.print("<p>Temperature: ");
              client.print(temperature);
              client.println(" &deg;C</p>");
              client.print("<p>Humidity: ");
              client.print(humidity);
              client.println(" %</p>");
            }
 
            client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
 
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
