#include "Sound.h"
#include <avr/io.h>

volatile uint8_t Sound::point;
volatile uint16_t Sound::timer;
volatile uint8_t Sound::counter;

Sound::Sound()
{
	point = 0;
	timer = 0;
	counter = 0;
}

void Sound::soundOn(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		DDRD |= (1 << 3);
		TCCR2A |= (1 << COM2B1)|(1 << WGM21)|(1 << WGM20);
		TCCR2B |= (1 << WGM22)|(1 << CS22)|(1 << CS21);
		TCCR1B |= (1 << CS10);
		TIMSK1 |= (1 << TOIE1);
		point = 0;
		timer = 0;
		counter = 0;
	}
}

void Sound::soundOff(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		DDRD &= ~(1 << 3);
		TCCR2A &= ~((1 << COM2A1)|(1 << WGM21)|(1 << WGM20));
		TCCR2B &= ~((1 << WGM22)|(1 << CS22)|(1 << CS21));
		TCCR1B &= ~(1 << CS10);
		TIMSK1 &= ~(1 << TOIE1);
		OCR2A = 0;
		OCR2B = 0;
		point = 0;
		timer = 0;
		counter = 0;
	}
}

void Sound::PWM_Generator(void)
{
	if(counter < 4)
	{
		if(point == 0)
		{
			OCR2A = 30; OCR2B = OCR2A/2;
			DDRD |= (1 << 3);
			DDRB |= (1 << 3);
			PORTB |= (1 << 3);
			point = 1;
		}
		else if(point == 1)
		{
			OCR2A--; OCR2B = OCR2A/2;
			if(OCR2A <= 10)
			{
				DDRB |= (1 << 3);
				PORTB &= ~(1 << 3);
				point = 2;
			}
		}
		else if(point == 2)
		{
			OCR2A++; OCR2B = OCR2A/2;
			if(OCR2A >= 100)
			{
				counter++;
				point = 0;
			}
		}
	}
	
	if(counter >= 4)
	{
		if(point == 0)
		{
			OCR2A = 30; OCR2B = OCR2A/2; timer = 100;
			DDRD |= (1 << 3);
			DDRB |= (1 << 3);
			PORTB |= (1 << 3);
			point = 1;
		}
		else if(point == 1)
		{
			timer--;
			if(timer <= 5)
			{
				DDRD &= ~(1 << 3);
				PORTB &= ~(1 << 3);
				point = 2;
			}
		}
		else if(point == 2)
		{
			timer++;
			if(timer >= 150)
			{
				counter++;
				point = 0;
				if(counter == 8)
				{
					counter = 0;
				}
			}
		}
	}
}
