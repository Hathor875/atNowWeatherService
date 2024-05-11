#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "x";
const char* password = "x";
int ledPin = 2;
int ledBrightness = 0; 

ESP8266WebServer server(80);

struct wifi_network {
  String name;
  unsigned short channel;
  short quality;
};

const int MAX_NETWORKS = 10;

wifi_network* scanWiFiNetworks() {
  int numNetworks = WiFi.scanNetworks();
  wifi_network* networks = new wifi_network[numNetworks];

  for (int i = 0; i < numNetworks; ++i) {
    String ssid = WiFi.SSID(i);
    int channel = WiFi.channel(i);
    int quality = WiFi.RSSI(i);

    networks[i].name = ssid;
    networks[i].channel = channel;
    networks[i].quality = quality;
  }

  return networks;
}

wifi_network* sortWiFiNetworks(wifi_network networks[], int count) {
    std::sort(networks, networks + count, [](const wifi_network& a, const wifi_network& b) {
        return a.quality > b.quality;
    });

    return networks;
}

int dBmToPercentage(int dBm) {
  const int RSSI_MIN = -100;
  const int RSSI_MAX = -50;  
  
  int quality;

  if (dBm <= RSSI_MIN) {
    quality = 0;
  } else if (dBm >= RSSI_MAX) {
    quality = 100;
  } else {
    quality = map(dBm, RSSI_MIN, RSSI_MAX, 0, 100);
  }

  return quality;
}

void ledBlink(int step) {
  ledBrightness = (ledBrightness + step) % 256; 
  analogWrite(ledPin, ledBrightness);
}

bool wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\n Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    ledBlink(25);
    Serial.print(".");
    delay(100);
  }

  digitalWrite(ledPin, LOW);

  return true;
}

void homePage() {
  wifi_network* networks = scanWiFiNetworks();
  networks = sortWiFiNetworks(networks, WiFi.scanComplete());
  String page = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  page += "<style>";
  page += "body { font-family: Arial, sans-serif; background-color: #f0f0f0; }";
  page += "h2 { color: #333; }";
  page += "table { border-collapse: collapse; width: 100%; }";
  page += "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }";
  page += "tr:nth-child(even) { background-color: #f2f2f2; }";
  page += ".red { color: red; }";
  page += ".orange { color: orange; }";
  page += ".green { color: green; }";
  page += "</style>";
  page += "</head><body>";
  page += "<h2>Available WiFi Networks</h2>";
  page += "<table>";
  page += "<tr><th>Network Name</th><th>Channel</th><th>Quality (%)</th></tr>";

  for (int i = 0; i < WiFi.scanComplete(); ++i) {
    page += "<tr>";
    page += "<td><a href=\"/connect?ssid=" + networks[i].name + "\">" + networks[i].name + "</a></td>";
    page += "<td>";
    page += networks[i].channel;
    page += "</td>";
    page += "<td>";

    int quality = dBmToPercentage(networks[i].quality);
    if (quality >= 0 && quality <= 25) {
      page += "<span class=\"red\">&#x25CF;</span>";
    } else if (quality > 25 && quality <= 50) {
      page += "<span class=\"orange\">&#x25CF;</span>";
    } else {
      page += "<span class=\"green\">&#x25CF;</span>";
    }

    page += " " + String(quality) + "%";
    page += "</td>";
    page += "</tr>";
  }

  page += "</table>";
  page += "<br><form action=\"/rescan\"><input type=\"submit\" value=\"Rescan\"></form>";
  page += "</body></html>";

  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", page);

  delete[] networks;
}

void connectToWiFiPage() {
  String ssid = server.arg("ssid");
  String page = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  page += "<style>";
  page += "body { font-family: Arial, sans-serif; background-color: #f0f0f0; }";
  page += "h2 { color: #333; }";
  page += "</style>";
  page += "</head><body>";
  page += "<h2>Connect to WiFi Network: " + ssid + "</h2>";
  page += "<form action=\"/connect\" method=\"POST\">";
  page += "SSID: <input type=\"text\" name=\"ssid\" value=\"" + ssid + "\" readonly><br>";
  page += "Password: <input type=\"password\" name=\"password\"><br>";
  page += "<input type=\"submit\" value=\"Connect\">";
  page += "</form>";
  page += "<br><a href=\"/\">Rescan WiFi Networks</a>";
  page += "</body></html>";

  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", page);
}

void handleConnect() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  WiFi.begin(ssid.c_str(), password.c_str());

  server.sendHeader("Location", "/");
  server.send(303);
}

void rescanWiFi() {
  WiFi.scanNetworks(true);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  wifiConnect();
  server.on("/", homePage);
  server.on("/connect", HTTP_GET, connectToWiFiPage);
  server.on("/connect", HTTP_POST, handleConnect);
  server.on("/rescan", rescanWiFi);
  server.begin();
}

void loop() {
  server.handleClient();
}