#include <Arduino.h>
#include <Wire.h>

void setup() {
    Wire.begin();
    Serial.begin(9600);
    Wire.beginTransmission(0x20);
    Wire.write(0b00000110);
    Wire.write(0b00000000);
    Wire.write(0b00000000);
    Wire.endTransmission();
    while (!Serial);  // Leonardo: wait for serial monitor
}

void loop() {
    Wire.beginTransmission(0x20);
    Wire.write(0b00000010);
    Wire.write(0b11111111);
    Wire.write(0b11111111);
    Wire.endTransmission();
    delay(500);  // wait 5 seconds for next scan
    Wire.beginTransmission(0x20);
    Wire.write(0b00000010);
    Wire.write(0b00000000);
    Wire.write(0b00000000);
    Wire.endTransmission();
    delay(500);
}