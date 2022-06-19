#include <Arduino.h>
#include <WiFi.h>

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println("Hello World");
    Serial.println(WIFI_NAME);
    Serial.println(WIFI_PASS);
    delay(500);
}