# vmix-blackmagic-tally
Tally system for controlling Blackmagic camera tallies from vmix, using Arduino + BMD 3G Sheild.

Use your Blackmagic URSA & Studio camera tally lights with vMix for about US $150 in hardware.

System connects to vMix tally system over IP, and converts tally data to ATEM commands over SDI that the cameras can read.  

# Required / Tested Hardware

1. [Arduino Uno R3](https://store-usa.arduino.cc/products/arduino-uno-rev3/)
2. [Arduino Ethernet Shield 2](https://store-usa.arduino.cc/products/arduino-ethernet-shield-2)
3. [Blackmagic 3G Shield](https://www.blackmagicdesign.com/developer/product/arduino)

# Instructions

1. Configure MAC Address
2. Configure Vmix Address
3. Set conversion map for vMix input IDs to Blackmagic camera IDs

# Roadmap / Future Features

- Web configuration
- Static network configuration

# About

Developed by [Atomic](https://weareatomic.com)
