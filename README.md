EE 250L Spring 2018: Example to run MQTT using an m3pi and ESP8266 device.

## Summary

The main purpose for this repository is to show an educational example of a
multi-threaded, (mostly) event-based MQTT application built on top of mbed OS 
using the ESP8266-01 wifi interface. This code is configured for the mbed 
LPC1768 board, but should be compatible for other mbed OS capatible boards.

## Preface

This repository is intended for educational purposes. The code here is heavily
commented, and we absolutely welcome any recommendations to improve the code or 
documentation. Please communicate to us via an issue. Pull requests are welcome!

## Quick Start (to run this example out of the box)

Materials Needed:

1) Update the firmware of the LPC1768 and ESP8266-01 
2) Follow the Fritzing diagram to connect the LPC1768 to the ESP8266-01
3) Edit the MQTT broker hostname and port macros in main.cpp accordingly
4) Configure your mbed_app.json with your wifi SSID and PW
7) Boot everything up. To test if everything is working, subscribe to [topic] 
and publish to [topic].

## Motivation

1) While mbed OS is well documented with many examples (including examples of
using MQTTClient), there is a lack of more complex example applications to 
illustrate the use of the mbed OS API. In addition, there is not a single 
place that documents how to run MQTT with an ESP8266 device in mbed OS. This
repository is provided to help fill these gaps.

2) Covering ISRs, polling loops, etc. is the first step of learning embedded 
programming. The next step is to equip students with the ability to program 
richer applications. Embedded operating systems and programming design patterns 
can empower students to create these applications with cleaner code. This 
application example uses the simple dispatcher programming pattern with the use 
of mbed OS libraries typically available on other embedded OSes. Patterns are
hard for students to wrap their head around, and teaching from a book has not 
shown to be very effective. We have found showing deeper examples illustrating 
how a pattern is useful and having students develop using the pattern helps 
students understand their uses and overall create richer applications (with
more organized code!). One way to use this repository is start by reading 
through this example carefully and running the code. Then, use the structure
of this example and cater it to your target application.

## Powering the ESP8266-01

The ESP8266-01 (or ESP-01) chip takes a 3.3V power supply. Powering it with 5v
may burn the chip, so please be careful with this, especially because the 
LPC1768 has a 5V output. The ESP-01 needs a clean 3.3V power 
supply so filtering a 3.3V output from the mbed board with a large capacitor 
(we have used 100uF+ capacitors just in case) will help prevent the ESP-01 from 
random power cycles. Also, even though the GPIO2 pin is not used, we connect it 
to VCC (high) to try to further prevent power cycles. We are not quite sure if 
this matters but we have found many reports of needing to do this on forums.

## Updating the mbed LPC1768 and ESP8266-01 Firmware

LPC1768:

Updating the LPC1768 is very simple, and an updated firmware is needed for 
mbed OS 5+ support. Follow the instructions at 
[this page.](https://os.mbed.com/handbook/Firmware-LPC1768-LPC11U24)
The firmware revision we use is 141212.

ESP8266-01:

Updating the ESP8266-01 (or simply ESP-01) is a more involved process. First, we have found that
using an mbed board as a serial passthrough does NOT work very well. It works if
you use specific esptool versions and slow down the flash block sizes. Please 
purchase an FTDI board with a 3.3V output to save you the headache. We 
specifically used the [need link and name of board here]. 

Clone the latest version of the 
[esptool](https://github.com/espressif/esptool) to flash your ESP-01. Also, 
clone the 
[ESP8266_NONOS_SDK](https://github.com/espressif/ESP8266_NONOS_SDK) repository 
to get the binaries we used to flash our ESP-01. The ESP8266 comes in very many 
flavors, and there are many revisions (and manufacturers?) of the ESP-01. 
Hopefully, the settings and version we successfully used will also work for you. 
The binaries we used were from the master branch at the commit 
[509eae8515793ec62f6501e2783c865f9a8f87e3](https://github.com/espressif/ESP8266_NONOS_SDK/tree/509eae8515793ec62f6501e2783c865f9a8f87e3)
of the ESP8266_NONOS_SDK repository. Since we have the 1MB flash variant, we 
used the binary files listed below. To prepare for flashing, consolidate all the 
following binary files into one folder:

* boot_v1.6.bin
* user1.1024.new.2.bin
* esp_init_data_default.bin
* blank.bin

Then, flash using the following commands. Make sure to change the port value
accordingly to your workspace:

    cd esptool/
    ./esptool.py --port /dev/ttyUSB0 --baud 115200 write_flash -fm dout -fs 1MB -ff 26m 0x00000 path/to/binaries/boot_v1.6.bin 0x01000 path/to/binaries/user1.1024.new.2.bin 0xfc000 path/to/binaries/esp_init_data_default.bin 0x7e000 path/to/binaries/blank.bin

To test if the flash worked, you can use our example.

## Compiling this Example in Linux

It is possible to import this example into the mbed OS online compiler to
build. However, we only provide instructions for compiling using mbed-cli. 
First, install the pre-reqs (mbed-cli only currently supports python2 so please
install pip2 using the get-pip.py script that can be found online 
`python2 get-pip.py`):

1. pip2 install mbed-cli
2. Install python packages: `pip2 install -r requirements.txt`
3. Install python, libusb and libncursus (i386 to be compatible with arm-none-eabi-gdb)
    
        sudo apt-get install python libusb-1.0-0-dev libncurses5:i386

4. It might be necessary to update your USB settings to get non-root access to DAP:

        sudo sh -c 'echo SUBSYSTEM==\"usb\", ATTR{idVendor}==\"0d28\", ATTR{idProduct}==\"0204\", MODE:=\"666\" > /etc/udev/rules.d/mbed.rules' 
        sudo /etc/init.d/udev restart 

The next step is to initialize the mbed project and designate the compiler and
target. First make sure to clone this repository or fork the repository and 
clone your fork.

```
cd m3pi-mqtt-ee250/
mbed deploy
mbed new .
mbed toolchain GCC_ARM
mbed target LPC1768
```





