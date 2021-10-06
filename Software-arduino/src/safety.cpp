#include "safety.h"
#include <Arduino.h>
#include "globals.h"
#include "LED.h"

const float voltage[] = {2.7427, 2.7206, 2.6981, 2.6751, 2.6517, 2.6278, 2.6035, 2.5789, 2.5538, 2.5283,
                         2.5024, 2.4762, 2.4496, 2.4226, 2.3954, 2.3678, 2.3400, 2.3118, 2.2835, 2.2549,
                         2.2261, 2.1971, 2.1680, 2.1387, 2.1092, 2.0796, 2.0498, 2.0200, 1.9901, 1.9601,
                         1.9300, 1.9001, 1.8701, 1.8402, 1.8104, 1.7806, 1.7510, 1.7214, 1.6920, 1.6628,
                         1.6337, 1.6047, 1.5758, 1.5472, 1.5188, 1.4906, 1.4627, 1.4350, 1.4076, 1.3804,
                         1.3536, 1.3271, 1.3008, 1.2749, 1.2493, 1.2240, 1.1991, 1.1745, 1.1502, 1.1263,
                         1.1028, 1.0800, 1.0575, 1.0354, 1.0137, 0.9923, 0.9714, 0.9508, 0.9305, 0.9107,
                         0.8912, 0.8719, 0.8531, 0.8346, 0.8165, 0.7987, 0.7813, 0.7642, 0.7475, 0.7311,
                         0.7150, 0.6994, 0.6840, 0.6690, 0.6543, 0.6399, 0.6259, 0.6121, 0.5986, 0.5855,
                         0.5726, 0.5599, 0.5475, 0.5354, 0.5235, 0.5119, 0.5006, 0.4895, 0.4787, 0.4681,
                         0.4578, 0.4478, 0.4380, 0.4284, 0.4191, 0.4100, 0.4011, 0.3924, 0.3839, 0.3756,
                         0.3674, 0.3595, 0.3518, 0.3442, 0.3368, 0.3296, 0.3225, 0.3157, 0.3090, 0.3024,
                         0.2960};

const int temperature[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                           21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
                           39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
                           57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
                           75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
                           93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
                           109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120};

const unsigned int n = sizeof(voltage);

HardwareTimer *FanTimer = new HardwareTimer(TIM3);
int currentPWM_fan = 0;
int lastPWM_fan[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int get_LED_temperature()
{
  // Vrefint = 1.20V (typ) [datasheet p. @@]
  float readVoltage = 1.20 * analogRead(LED_TEMP) / analogRead(AVREF);

  if (readVoltage >= voltage[0])
  {
    return temperature[0];
  }
  else if (readVoltage <= voltage[n - 1])
  {
    return temperature[n - 1];
  }
  else
  {
    int i = 0;
    while (readVoltage <= voltage[i + 1])
    {
      i++;
    }
    return ((readVoltage - voltage[i]) / (voltage[i + 1] - voltage[i])) < 0.5 ? temperature[i] : temperature[i + 1];
  }
};

void set_fan_speed(int speed)
{
  if (speed != currentPWM_fan)
  {
    // The fan has a pretty linear response, so no correction needed [White paper p. @@]
    FanTimer->pauseChannel(1);
    FanTimer->setCaptureCompare(1, speed, PERCENT_COMPARE_FORMAT);
    FanTimer->resumeChannel(1);

    currentPWM_fan = speed;
  }
}

void safetyRoutine()
{
  float temperature = get_LED_temperature();

  DEBUG_PRINT("LED temperature: ");
  DEBUG_PRINTLN(temperature);

  if (temperature >= overtemperature)
  {
    blackout();
    set_fan_speed(100);
    DEBUG_PRINTLN("Emergency stop. LED overheating!");
  }
  else if (temperature >= fan_on_th)
  {
    int speed = 0;

    lastPWM_fan[10] = map(temperature, fan_on_th, 0.68 * overtemperature, minPWM_fan, 100);;
    for (byte i = 0; i <= 9; i++)
    {
      lastPWM_fan[i] = lastPWM_fan[i+1];
      speed += lastPWM_fan[i];
    }
    speed += lastPWM_fan[10];
    speed /= 11;

    set_fan_speed(speed);
    DEBUG_PRINT("Fan speed set to: ");
    DEBUG_PRINTLN(speed);
  }
  else if (temperature <= fan_off_th && currentPWM_fan != 0)
  {
    set_fan_speed(0);
    for (byte i = 0; i <= 10; i++)
    {
      lastPWM_fan[i] = 0;
    }
  }
}

/* Set pins to output. */
void safetyinit()
{
  pinMode(LED_TEMP, INPUT_ANALOG);

  FanTimer->setOverflow(25000, HERTZ_FORMAT); // 25kHz
  FanTimer->setMode(1, TIMER_OUTPUT_COMPARE_PWM1, FAN1);

  set_fan_speed(0);

  DEBUG_PRINTLN("Safety functions initialised");
}
