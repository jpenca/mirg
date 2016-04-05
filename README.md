# mirg
a simple program for the raspberry pi. needs a usb-midi device which sends clock/start/stop, provides korg-style pulse sync on GPIO.


#### Dependencies
libasound2-dev is needed for USB-MIDI:

    sudo apt-get install libasound2-dev

this bcm2835 library is needed to use the GPIO pins:
http://www.airspayce.com/mikem/bcm2835/index.html

#### Compilation
    gcc -o mirg mirg.c -lbcm2835 -lasound
	
#### Usage
Connect the USB-MIDI device you want to use as master clock. 
Find out the ALSA ID of your MIDI device:

    amidi -l


then use your chosen device, e.g. `hw:1,0,0` as an argument to mirg:

    ./mirg hw:1,0,0

Press play on your MIDI device. Mirg provides a Korg-style pulse once every six MIDI clock ticks.