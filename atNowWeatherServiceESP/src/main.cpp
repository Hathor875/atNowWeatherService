#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "atNowSensor";
const char *password = "12345678";

IPAddress apIP(192, 168, 1, 1);
IPAddress netMsk(255, 255, 255, 0);

int ledPin = 2;

ESP8266WebServer server(80);

struct wifi_network
{
  String name;
  unsigned short channel;
  short quality;
};

const int MAX_NETWORKS = 255;

void timerISR() {}

void ledBlink(int speed)
{
  analogWrite(ledPin, 0);
  delay(100);
  analogWrite(ledPin, 255);
  delay(speed / 2);
  analogWrite(ledPin, 0);
  delay(speed / 2);
}

wifi_network *scanWiFiNetworks()
{
  ledBlink(100);
  int numNetworks = WiFi.scanNetworks();
  wifi_network *networks = new wifi_network[numNetworks];
  ledBlink(10);
  for (int i = 0; i < numNetworks; ++i)
  {
    String ssid = WiFi.SSID(i);
    int channel = WiFi.channel(i);
    int quality = WiFi.RSSI(i);

    networks[i].name = ssid;
    networks[i].channel = channel;
    networks[i].quality = quality;
  }
  ledBlink(100);
  return networks;
}

wifi_network *sortWiFiNetworks(wifi_network networks[], int count)
{
  std::sort(networks, networks + count, [](const wifi_network &a, const wifi_network &b)
            { return a.quality > b.quality; });

  return networks;
}

int dBmToPercentage(int dBm)
{
  const int RSSI_MIN = -100;
  const int RSSI_MAX = -50;

  int quality;

  if (dBm <= RSSI_MIN)
  {
    quality = 0;
  }
  else if (dBm >= RSSI_MAX)
  {
    quality = 100;
  }
  else
  {
    quality = map(dBm, RSSI_MIN, RSSI_MAX, 0, 100);
  }

  return quality;
}

void wifiConnect()
{
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid, password);
}

void homePage()
{
  wifi_network *networks = scanWiFiNetworks();
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

  for (int i = 0; i < WiFi.scanComplete(); ++i)
  {
    page += "<tr>";
    page += "<td><a href=\"/connect?ssid=" + networks[i].name + "\">" + networks[i].name + "</a></td>";
    page += "<td>";
    page += networks[i].channel;
    page += "</td>";
    page += "<td>";

    int quality = dBmToPercentage(networks[i].quality);
    if (quality >= 0 && quality <= 25)
    {
      page += "<span class=\"red\">&#x25CF;</span>";
    }
    else if (quality > 25 && quality <= 50)
    {
      page += "<span class=\"orange\">&#x25CF;</span>";
    }
    else
    {
      page += "<span class=\"green\">&#x25CF;</span>";
    }

    page += " " + String(quality) + "%";
    page += "</td>";
    page += "</tr>";
  }

  page += "</table>";
  page += "</body></html>";

  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", page);

  delete[] networks;
}

void connectToWiFiPage()
{
  String ssid = server.arg("ssid");
  String page = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  page += "<style>";
  page += "body { font-family: Arial, sans-serif; background-color: #f0f0f0; }";
  page += "h2 { color: #333; }";
  page += ".loader { display: none; --s: 25px; --g :5px; width: calc(2*(1.353*var(--s) + var(--g))); aspect-ratio: 1; background: linear-gradient(#ff1818 0 0) left/50% 100% no-repeat, conic-gradient(from -90deg at var(--s) calc(0.353*var(--s)), #fff 135deg,#666 0 270deg,#aaa 0); background-blend-mode: multiply; --_m: linear-gradient(to bottom right, #0000 calc(0.25*var(--s)),#000 0 calc(100% - calc(0.25*var(--s)) - 1.414*var(--g)),#0000 0), conic-gradient(from -90deg at right var(--g) bottom var(--g),#000 90deg,#0000 0); -webkit-mask: var(--_m); mask: var(--_m); background-size: 50% 50%; -webkit-mask-size: 50% 50%; mask-size: 50% 50%; -webkit-mask-composite: source-in; mask-composite: intersect; animation: l9 1.5s infinite; } @keyframes l9 { 0%,12.5% {background-position:0% 0%,0 0} 12.6%,37.5% {background-position:100% 0%,0 0} 37.6%,62.5% {background-position:100% 100%,0 0} 62.6%,87.5% {background-position:0% 100%,0 0} 87.6%,100% {background-position:0% 0%,0 0} }";
  page += "</style>";
  page += "<script>";
  page += "function showLoader() { document.getElementById('loader').style.display = 'block'; }";
  page += "</script>";
  page += "</head><body>";
  page += "<h2>Connect to WiFi Network: " + ssid + "</h2>";
  page += "<div id=\"loader\" class=\"loader\"></div>";
  page += "<form id=\"connectForm\" action=\"/connect\" method=\"POST\" onsubmit=\"showLoader()\">";
  page += "SSID: <input type=\"text\" name=\"ssid\" value=\"" + ssid + "\" readonly><br>";
  page += "Password: <input type=\"password\" name=\"password\"><br>";
  page += "<input type=\"submit\" value=\"Connect\">";
  page += "</form>";
  page += "<br><a href=\"/\">Rescan WiFi Networks</a>";
  page += "</body></html>";

  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", page);
}

void handleConnect()
{
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (millis() - startTime > 10000) {
      server.send(500, "text/plain", "Connection timeout!");
      return;
    }
  }

  String connectedPage = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\">";
  connectedPage += "<style>";
  connectedPage += "body { font-family: Arial, sans-serif; background-color: #f0f0f0; }";
  connectedPage += "h2 { color: #333; }";
  connectedPage += "</style>";
  connectedPage += "</head><body>";
  connectedPage += "<h2>Connection successful!</h2>";
  connectedPage += "<p>The device has successfully connected to the selected WiFi network.</p>";
  connectedPage += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
  connectedPage += "<p>You can <a href=\"/\">rescan WiFi networks</a> or close this window.</p>";
  connectedPage += "</body></html>";

  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", connectedPage);

  WiFi.softAPdisconnect(true);

  digitalWrite(ledPin, LOW);
}

void rescanWiFi()
{
  WiFi.scanNetworks(true);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid, password);
  server.on("/", homePage);
  server.on("/connect", HTTP_GET, connectToWiFiPage);
  server.on("/connect", HTTP_POST, handleConnect);
  server.on("/rescan", rescanWiFi);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop()
{
  server.handleClient();
}
