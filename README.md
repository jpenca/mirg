# mirg
a simple program for the raspberry pi. Listens to MIDI clock/start/stop on a USB port, provides korg-style pulse sync on GPIO. For example, this lets you sync Korg Volca or Teeange Engineering OP-XX to any device which provides clock via USB, such as Elektron Analog series or Teenage Engineering OP-1.

#### Dependencies
libasound2-dev is needed for USB-MIDI:

    sudo apt-get install libasound2-dev

this bcm2835 library is needed to use the GPIO pins:
http://www.airspayce.com/mikem/bcm2835/index.html

Follow the instructions on that site for installation.

#### Compile mirg
    gcc -o mirg mirg.c -lbcm2835 -lasound

#### Usage
Connect the USB-MIDI device you want to use as master clock before starting mirg.
Find out the ALSA ID of your MIDI device using amidi:

    amidi -l

This will list all connected devices. Use your chosen device, e.g. `hw:1,0,0` as an argument to mirg:

    ./mirg hw:1,0,0

Make sure you MIDI device sends clock/start/stop via USB. Press play on your MIDI device. Mirg creates a Korg-style pulse every 8th-note (once every 12 MIDI clock ticks) on GPIO pin 17. You'll need to connect a minijack cable to that pin, e.g. use a standard modular patch cable and cut off one end. Connect tip to pin 17, connect sleeve to ground.