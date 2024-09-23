# vmix-blackmagic-tally
Tally system for controlling Blackmagic camera tallies from vmix, using Arduino + BMD 3G Sheild.

Use your Blackmagic URSA & Studio camera tally lights with vMix for about US $150 in hardware.

System connects to vMix tally system over IP, and converts tally data to ATEM commands over SDI that the cameras can read.  

# Required / Tested Hardware

1. [Arduino Uno Mega 2560 Rev3](https://store.arduino.cc/products/arduino-mega-2560-rev3)
2. [Arduino Ethernet Shield 2](https://store-usa.arduino.cc/products/arduino-ethernet-shield-2)
3. [Blackmagic 3G‑SDI Shield for Arduino](https://www.blackmagicdesign.com/developer/product/arduino)

# Instructions

1. Add Ethernet, SPI, and BMDSDIControl libraries to your Arduino IDE
2. Compile and upload the code to the Arduino.
3. Configure Vmix Address and via web interface or by making HTTP GET requests.
4. Set conversion map for vMix input IDs to Blackmagic camera IDs via web interface or by making HTTP GET requests.

vMix tally data does not provide the GUID/hash for the input, only the input ID.  So be careful when re-ordering vMix input numbers.  The tool currently supports 8 cameras, but can easily expanded to more (simply change the MAXTALLIES variable).

# Web Interface & Configuration API

The arudino does have a small web server that can accept and display configuration settings.

## Web Interface

Open up the /admin-interface/index.html file in your browser. This will allow you to configure the vMix IP address and tally mapping via a GUI.

## Manually via HTTP GET Requests

### Show Configuration
http://device-ip/ 

### Configure vMix IP Address
http://device-ip/save?vmixip=192.168.1.100


### Configure Tally Map

To set vmix input 1 to camera 1, and vmix input 2 to camera 2:
http://device-ip/save?input0=1&cam0=1
http://device-ip/save?input1=2&cam1=2

# Roadmap / Future Features

- We'll be adding the web interface directly to the advice, but have some issues with not having enough memory on the Arduino, so additional testing will be required.
- Static network configuration

# Notes & Compatibility

We recommend using an Arduino Mega, due to the large amount of memory required for the HTML admin page. The Blackmagic Shield and Ethernet library use up most of the available memory. The UNO will work, but there is not enough memory to host the admin page.

# About

Developed by [Atomic](https://weareatomic.com)
More info at [Atomic Lab Blog Post](https://weareatomic.com/lab/vmix-tally-lights/)
