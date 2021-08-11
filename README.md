# Sick LED Driver

**Work in progress**

Six channel LED driver board that is (quite literally) built around the [LZ7-04-M2PD](https://www.osram.com/ecat/LED%20ENGIN%20LuxiGen%20LZ7-04M2PD/com/en/class_pim_web_catalog_103489/prd_pim_device_13582430/) on MCPCB from LED Engin.

![rendering](Docu/rendering-top.png)
 It's a 60W LED with six colours, which can be used to do some adjustable white experiments due to the relatively full spectrum of the LED (at least compared to a normal RGB-LED): 

![relative spectrum](Docu/spectrum.png)
Source: [LED Datasheet](https://dammedia.osram.info/media/resource/hires/osram-dam-14744640/LZ7-04M2PD_EN.pdf)

The project's objective is not to build an LED that is as bright as possible but to create a dev board that makes it easy to study RGBALC LEDs. Thus, the board is powered via USB-PD and programmed with Arduino.

## Components
 * LED Driver: **BP1808**
 * MCU: **STM32F103C8**
 * to be continued…