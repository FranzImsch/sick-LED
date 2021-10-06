#include "USB-PD.h"
#include <Arduino.h>
#include "globals.h"

#include "Wire.h"
#include <SparkFun_STUSB4500.h>

STUSB4500 usb;

void USBPDinit()
{
    Wire.setSCL(SCL);
    Wire.setSDA(SDA);
    Wire.begin();

    DEBUG_PRINTLN();

    if (!usb.begin())
    {
        DEBUG_PRINTLN("Cannot connect to STUSB4500.");
    }

    // write_PD_settings(); // uncomment to write settings to chip

    usb.read();

    // Read the Power Data Objects (PDO) highest priority (1-3) 
    DEBUG_PRINT("PDO Number: ");
    DEBUG_PRINTLN(usb.getPdoNumber());

    // PDO 1
    DEBUG_PRINTLN();
    DEBUG_PRINT("Voltage 1: ");
    DEBUG_PRINTLN(usb.getVoltage(1));
    DEBUG_PRINT("Current 1: ");
    DEBUG_PRINTLN(usb.getCurrent(1));
    DEBUG_PRINTLN();

    // PDO 2
    DEBUG_PRINT("Voltage 2: ");
    DEBUG_PRINTLN(usb.getVoltage(2));
    DEBUG_PRINT("Current 2: ");
    DEBUG_PRINTLN(usb.getCurrent(2));
    DEBUG_PRINTLN();

    // PDO 3
    DEBUG_PRINT("Voltage 3: ");
    DEBUG_PRINTLN(usb.getVoltage(3));
    DEBUG_PRINT("Current 3: ");
    DEBUG_PRINTLN(usb.getCurrent(3));
    DEBUG_PRINTLN();

    DEBUG_PRINT("External Power: ");
    DEBUG_PRINTLN(usb.getExternalPower());

    DEBUG_PRINT("USB Communication Capable: ");
    DEBUG_PRINTLN(usb.getUsbCommCapable());

    DEBUG_PRINT("Configuration OK GPIO: ");
    DEBUG_PRINTLN(usb.getConfigOkGpio());

    DEBUG_PRINT("GPIO Control: ");
    DEBUG_PRINTLN(usb.getGpioCtrl());

    DEBUG_PRINT("Enable Power Only Above 5V: ");
    DEBUG_PRINTLN(usb.getPowerAbove5vOnly());

    DEBUG_PRINT("Request Source Current: ");
    DEBUG_PRINTLN(usb.getReqSrcCurrent());

    Wire.end();
    DEBUG_PRINTLN("Disabled i2c");
    DEBUG_PRINTLN();
    DEBUG_PRINTLN();
}

void write_PD_settings()
{
    //// General settings ////
    usb.setUsbCommCapable(false);
    usb.setPowerAbove5vOnly(true); // No need for 5V power

    /// PDOs ////
    usb.setPdoNumber(3); // 3 PDO

    // PDO1 (fixed 5V, fixed under voltage lock out 3.3V)
    usb.setCurrent(1, 0.5); //
    usb.setUpperVoltageLimit(1, 20);

    // PDO2 15V ± 10% 2.5A
    usb.setVoltage(2, USB_Voltage_2);
    usb.setCurrent(2, USB_Current_2);
    usb.setLowerVoltageLimit(2, 10);
    usb.setUpperVoltageLimit(2, 10);

    // PDO3 20V ± 10% 2.0A
    usb.setVoltage(3, USB_Voltage_1);
    usb.setCurrent(3, USB_Current_1);
    usb.setLowerVoltageLimit(3, 10);
    usb.setUpperVoltageLimit(3, 10);

    //// Write to chip ////
    usb.write();

    DEBUG_PRINTLN("Updated USB-PD settings");
}