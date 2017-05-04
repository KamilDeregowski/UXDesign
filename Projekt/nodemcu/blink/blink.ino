#include <ESP8266WiFi.h>

WiFiServer server(80);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  WiFi.begin("Carat Wszechpolski", "QACC7S57");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(333);
  }

  Serial.println("Starting server");
  server.begin();

  Serial.print("Server running on ");
  Serial.println(WiFi.localIP());
}

String prepareHtmlPage()
{
  String htmlPage = 
      String("<!doctype html>") +
      "<html>" +
      "<h1>Hello there</h1>" +
      "<h3 id=\"message\"></h3>" + 
      "<button onclick=\"onToggle()\">Toggle LED</button>" +
      "<script>" +
      "function onToggle() {" +
      "  fetch('/toggle').then(r => {" +
      "    if (r.ok) document.getElementById('message').innerHTML = 'LED toggled';" +
      "    else document.getElementById('message').innerHTML = 'error occurred';" +
      "  })" +
      "}" +
      "</script>" +
      "</html>";
  return htmlPage;
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("Incoming connection");
  while (!client.available()) {
    delay(10);
  }

  String message = client.readStringUntil('\n');
  Serial.println(message);
  client.flush();

  if (message.indexOf("/toggle") != -1) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(prepareHtmlPage());
  client.println();
}
