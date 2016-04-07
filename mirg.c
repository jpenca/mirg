#include <bcm2835.h>
#include <stdio.h>
#include <alsa/asoundlib.h>

// Blinks on RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PIN RPI_GPIO_P1_11

// there are 12 clock ticks per 8th-note
#define CLOCK_DIV 12

// we want a 15 ms pulse
#define PULSE_LEN 15

typedef enum play_state
{
	play_state_stopped,
	play_state_playing,
}
play_state;

static void errormessage(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    putc('\n', stderr);
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("must define hardware midi port, e.g. \"sync hw:1,0,0\"\n");
		return 1;
	}
	
	int err;
	snd_rawmidi_t* midiin;
	const char* portname = argv[1];
	
	err = snd_rawmidi_open(&midiin, NULL, portname, SND_RAWMIDI_SYNC);
	if(err)
	{
		errormessage("error opening midi port\n");
		return 1;
	}
	err = snd_rawmidi_read(midiin, NULL, 0);
	if(err)
	{
		errormessage("error initializing midi read\n");
		return 1;
	}
	
	if (!bcm2835_init())
	{
		errormessage("failed to init the GPIO library\n");
		return 1;
	}
		
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
	play_state state = play_state_stopped;
	uint8_t ticks = 0;

	while (1)
	{
		uint8_t status;
		snd_rawmidi_read(midiin, &status, 1);
		
		if(status & 0xF0) 
		{
			switch(status)
			{
				case 0xFA: // start
				{
					ticks = 0; // reset clock
					state = play_state_playing;
					break;
				}
				case 0xFB: // continue
				{
					state = play_state_playing;
					break;
				}
				case 0xFC: // stop
				{
					state = play_state_stopped;
					break;
				}
				case 0xF8: // clock tick
				{
					if(ticks % CLOCK_DIV == 0) // 
					{
						ticks = 0; // reset clock
						if(state == play_state_playing)
						{
							bcm2835_gpio_write(PIN, HIGH);
							bcm2835_delay(PULSE_LEN);
							bcm2835_gpio_write(PIN, LOW);
						}
					}
					
					if(state == play_state_playing)
						ticks++;
					
					break;
				}
				default:
					break;
			}
		}
	}
	bcm2835_close();
	return 0;
}
