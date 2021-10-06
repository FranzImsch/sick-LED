#include "nanoDMX.h"
#include "LED.h"
#include "globals.h"

#include <Arduino.h>

/* Serial.begin() to receive nanoDMX commands via USB. */
void nanoDMXinit()
{
    Serial.begin();
    DEBUG_PRINTLN("nanoDMX initialised");
}

void receiveDMXviaUSB()
{
    if (Serial.available())
    {
        int input = Serial.read();
        if (input == 226) // Channels >256 are not of interest here, so they are ignored.
        {
            int channel = Serial.read();
            int value = Serial.read();

            if (channel >= red && channel <= cyan + 1)
            {
                receiveDMX(channel, value);
            }
            return;
        }
        else if (input == 67) // "C" nanoDMX connection check
        {
            int a = Serial.read();
            if (a == 63) // "?" => Check connection
            {
                Serial.print("G"); // "G" good connection
                DEBUG_PRINTLN("Connected to computer");
            }
        }
        else if (input == 78) // "N" nanoDMX init sequence
        {
            int a = Serial.read();
            int b = Serial.read();
            int c = Serial.read();
            if (a == 53 && b == 49 && c == 49) // "511" => 512 DMX channels
            {
                Serial.print("G");
                DEBUG_PRINTLN("Number of channels transmitted");
            }
        }
    }
}