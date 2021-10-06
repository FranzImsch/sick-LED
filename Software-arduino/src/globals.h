#pragma once

#include <Arduino.h>

///////////////////
// USER SETTINGS //
///////////////////

#define DEBUG

// LED Settings
#define gamma_correction_factor 2.8
const uint8_t Imax_led[] = {255, 255, 255, 255, 255, 255};

// Safety settings
const byte overtemperature = 70;
const byte fan_on_th = 35;
const byte fan_off_th = 30;

// Fan settings
const byte minPWM_fan = 20; // Intel standard recommendation https://noctua.at/pub/media/wysiwyg/Noctua_PWM_specifications_white_paper.pdf

// USB-PD Settings
// Priority 1
const float USB_Voltage_1 = 20.0;
const float USB_Current_1 = 2.0;

// Priority 2
const float USB_Voltage_2 = 15.0;
const float USB_Current_2 = 2.5;

#ifdef DEBUG
#define DEBUG_PRINT(...) Serial2.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial2.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) NULL
#define DEBUG_PRINTLN(...) NULL
#endif

/////////////////////
// PIN DEFINITIONS //
/////////////////////

/*
 LED pins defined in LED.cpp
*/

#define LED_TEMP PA7

#define FAN1 PB4
#define FAN2 PB5 // conflict with i2c

#define Serial2_TX PA2
#define Serial2_RX PA3

#define DMX_OUT PB6
#define DMX_IN PB7
#define DMX_DE PB12

#define BTN1 PB0
#define BTN2 PB1
#define BTN3 PB11

#define SCL PB8
#define SDA PB9

#define MIC_IN PA4
#define MIC_EN PB13

#define PD_RST PB14
#define PD_ALERT PB15

#define Light_Sensor PA5

#define VCC_Voltage_Pin PA6

///////////////////////
// COLOUR ASSIGNMENT //
///////////////////////

#define red 0
#define green 1
#define blue 2
#define lime 3
#define amber 4
#define cyan 5
