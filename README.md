# Integrate your Ampinvt solar controller with you home automation solution via ESPhome.

This project is based on a combination of hardware and software to facilitate obtaining, reading and processing the output from the Ampinvt solar controllers.


# About the controllers

Ampinvt solar products are common in North America due to their low cost, good build quality and availability through Amazon shopping sites. The controllers are at the lower end of the price range compared to the brand name producers and this is likely due to the lack of extended functionality rather than lower build quality. The remote interface is RS485 and utilizes a form of Modbus protocol although the implementation is far from standard.

The manufacturer does produce a commercial solution for remote monitoring of the unit which consists of a hardware RS485 reader and wifi access point which then couples to an app that can be downloaded to a phone. This solution does not meet the requirements for integration into a home automation system so I set out to build one.

## Required Hardware

To read the data on the RS485 bus requires the use of a serial component. Although I started out using an ESP8266 I quickly switched to using an ESP32 for the additional UART component. This code is based on the ESP32 platform. In addition, an RS485 interface is required and although several are available, I recommend obtaining a unit that has an auto flow control capability. I tried 3 different types and had the best success with the auto-flow variant. An example can be found by searching 'HW-0519 UART to RS485 interface'.

## Hardware Build

The hardware build is quite straightforward but will differ with your own use-case. I utilised a 12v to 5v DC-DC buck converter to power the ESP32 from my battery bank. The ESP32 sends 5v to the HW-0519 VCC pin and TX and RX connect to pins 16 and 17 on the ESP32. These can be changed in the YAML code if you wish. On the input side of the HW-0519, the RJ-45 jack utilises pins 1, 2 and 8 with pin 1 connecting to A+, pin 2 to B- and pin 8 to GND (the 3rd connection). Be careful with pin 7, it is 5v from the controller.

## Communication

While the Ampinvt units do utilize a form of Modbus, it is far from standard implementation. Rather than reading registers, a hexadecimal (hex) sequence is transmitted to the controller and a 37-byte hex sequence is received in response. The structure of the response can be found in the included protocol files. Be careful when reading these files as there is an unexplained difference between them; while the 'NEW MPPT communication protocol V1.0' file has more in-depth information, the use of the 0xA3 command code did not work successfully for me. The second document I received was in Chinese language and was converted via Google Translate revealing that 0xB3 is the correct command code, at least for my AP-80A unit. You may need to experiment with the command code used in the code section under uart.write in the YAML code.

Once the response is received from the unit, the code in the include file (ampinvt.h) breaks out the individual bytes to values, with bytes 0, 1 and 2 being discarded, bytes 3, 4 and 5 being split into bit values for boolean status indicators and the remaining bytes being either decimal values or ignored.

## Integration into Home Assistant

The code creates various sensors in the HA instance, many of which are boolean flags mapped to their function. Play around and build a test dashboard to see what works for you. The YAML and ampinvt.h files go into your esphome directory.

# Credits

I have no previous experience with RS485 or Modbus and I have relied heavily on solutions created by others for the base code. In addition, their help in assisting with the work here should be noted. You can find their work here:

@assembly12 ~ [Foxess T-Series Solar controller and inverter](https://github.com/assembly12/Foxess-T-series-ESPHome-Home-Assistant)
@htvekov ~ [Delta Solvia Inverter and Gateway](https://github.com/htvekov/solivia_esphome)

I also want to thank @koying on the [Home Assistant community](https://community.home-assistant.io/t/looking-for-someone-that-knows-c-c-c-to-help-me-over-a-hurdle-with-an-include-file/499067) for the help with creating a solution to split out the bits from the bytes. That was the biggest hurdle in the whole endeavour and I am grateful.
