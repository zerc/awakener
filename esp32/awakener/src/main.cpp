#include <Arduino.h>
#include <WiFi.h>

WiFiServer server(80);
WiFiUDP Udp;

byte msg[102] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98,
    0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5,
    0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32,
    0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11,
    0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00,
    0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71, 0x00, 0x11, 0x32, 0xF5, 0x98, 0x71,
};

IPAddress ip(255, 255, 255, 255);

void connectToWiFi(const char *ssid, const char *pwd);

int PRESSED = LOW;

void setup()
{
    Serial.begin(9600);
    connectToWiFi(WIFI_NAME, WIFI_PASS);

    server.begin();
    Serial.println("Server started");

    pinMode(D4, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    PRESSED = 1 - digitalRead(D4);

    if (PRESSED)
    {
        digitalWrite(LED_BUILTIN, PRESSED);
        Udp.beginPacket(ip, 9);
        Udp.write(msg, 102);
        Udp.endPacket();
    }

    WiFiClient client = server.available();

    if (client)
    {
        Serial.println("New client connected");
        bool lineIsBlank = true;

        while (client.connected())
        {
            char c = client.read();
            Serial.print(c);

            if (c == '\n' && lineIsBlank)
            {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println();
                client.println("<html><body><b>Hello</b></body></html>");
                break;
            }

            if (c == '\n')
            {
                lineIsBlank = true;
            }
            else if (c != '\r')
            {
                lineIsBlank = false;
            }
        }
        delay(1);
        client.stop();
        Serial.println("Client disconnected");
    }
}

void connectToWiFi(const char *ssid, const char *pwd)
{
    int ledState = 0;

    Serial.println("Connecting to WiFi network: " + String(ssid));

    WiFi.begin(ssid, pwd);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}