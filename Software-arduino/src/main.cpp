#include <Arduino.h>

#include "globals.h"
#include "safety.h"
#include "LED.h"
#include "nanoDMX.h"
#include "USB-PD.h"

HardwareSerial Serial2(Serial2_RX, Serial2_TX);

long lastmil = 0;

void setup()
{
#ifdef DEBUG
  Serial2.begin(115200);
#endif
  DEBUG_PRINT("Hello c:");

  USBPDinit(); // i2c / timer issue
  LEDinit();
  nanoDMXinit();
  safetyinit();
}

void loop()
{
  long now = millis();
  if (now - lastmil >= 2500)
  {
    lastmil = now;
    safetyRoutine();
  }

  receiveDMXviaUSB();
}