#include <Arduino.h>
#include <WiFi.h>

void connectToWiFi(const char *ssid, const char *pwd);

void setup()
{
    Serial.begin(9600);
    connectToWiFi(WIFI_NAME, WIFI_PASS);
}

void loop()
{
    Serial.println("Hello World");
    delay(500);
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