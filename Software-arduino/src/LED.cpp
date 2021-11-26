#include "LED.h"
#include "globals.h"
#include <Arduino.h>

int master_brightness = 0;
int valBefore[] = {0, 0, 0, 0, 0, 0};

/*
    PA0     Timer 2 Channel 1
    PB10    Timer 2 Channel 3
    PA1     Timer 2 Channel 2
    PA8     Timer 1 Channel 1
    PA9     Timer 1 Channel 2
    PA10    Timer 1 Channel 3 
*/

HardwareTimer *LEDTimer1 = new HardwareTimer(TIM1);
HardwareTimer *LEDTimer2 = new HardwareTimer(TIM2);

HardwareTimer *c_timer[] = {LEDTimer2, LEDTimer2, LEDTimer2, LEDTimer1, LEDTimer1, LEDTimer1};
const int c_pin[] = {PA0_ALT1, PB10, PA1_ALT1, PA8, PA9, PA10};
const byte c_channel[] = {1, 3, 2, 1, 2, 3};

void blackout() // @@dunno now
{
    for (byte i = red; i <= cyan; i++)
    {
        digitalWrite(i, LOW);
    }
    DEBUG_PRINTLN("blackout");
}

void LEDinit()
{
    LEDTimer1->setOverflow(1000, HERTZ_FORMAT); // 1kHz max. frequency of BP1808
    LEDTimer2->setOverflow(1000, HERTZ_FORMAT);

    for (byte i = red; i <= cyan; i++)
    {
        c_timer[i]->setMode(c_channel[i], TIMER_OUTPUT_COMPARE_PWM1, c_pin[i]);
    }

    DEBUG_PRINTLN("LED functions initialised");
}

uint32_t outputGammaCorrected(byte channel, int value)
{
    float gammacorrected = pow((value / 255.0) * (master_brightness / 255.0) * (Imax_led[channel] / 255.0), gamma_correction_factor) * 65535.0;
    return gammacorrected;
}

void receiveDMX(int channel, int value)
{
    if (channel == 0 && value != master_brightness) // Master brightness = channel 0,
    {
        master_brightness = value;
        for (byte i = red; i <= cyan; i++)
        {
            if (valBefore[i] != 0)
            {
                c_timer[i]->pauseChannel(c_channel[i]);
                c_timer[i]->setCaptureCompare(c_channel[i], outputGammaCorrected(i, valBefore[i]), RESOLUTION_16B_COMPARE_FORMAT);
                c_timer[i]->resumeChannel(c_channel[i]);
            }
        }
    }
    else if (channel >= (red + 1) && channel <= (cyan + 1)) // channel 1 - 6 are the individual colours,
    {
        if (value != valBefore[channel - 1])
        {
            int n = channel - 1;
            c_timer[n]->pauseChannel(c_channel[n]);
            c_timer[n]->setCaptureCompare(c_channel[n], outputGammaCorrected(n, value), RESOLUTION_16B_COMPARE_FORMAT);
            c_timer[n]->resumeChannel(c_channel[n]);
            valBefore[n] = value;
        }
    }
    else // everything else is ignored at the moment.
    {
        return;
    }
}