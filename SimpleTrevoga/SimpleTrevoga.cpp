/*
 * SimpleTrevoga.cpp
 *
 * Created: 22.08.2017 14:21:25
 *  Author: user
 */ 


#define F_CPU 1200000UL

#include <avr/io.h>
#include <util/delay.h>

#define BUTTONS_DDR DDRB
#define BUTTONS_PIN PINB
#define BUTTON_1 3
#define BUTTON_2 4

#define SOUND_DDR DDRB
#define SOUND_PORT PORTB
#define SOUND_OUT 1

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_OUT 0

void initPorts(void)
{
	SOUND_DDR |= (1 << SOUND_OUT);
	SOUND_PORT &= ~(1 << SOUND_OUT);
	
	LED_DDR |= (1 << LED_OUT);
	LED_PORT &= ~(1 << LED_OUT);
	
	BUTTONS_DDR &= ~((1 << BUTTON_1)|(1 << BUTTON_2));
}

void soundOn(void)
{
	SOUND_DDR |= (1 << SOUND_OUT); //Sound as output
	TCCR0A |= (1 << COM0B1)|(1 <<  WGM01)|(1 << WGM00); //Set Fast PWM non-inverting mode
	TCCR0B |= (1 << WGM02)|(1 << CS01)|(1 << CS00); //Set prescaler 1024
}

int main(void)
{
	_delay_ms(500);
	
	initPorts();
	
	bool trevogaIsPressed = false;
	uint8_t counter = 0;
	uint8_t point = 0;
	
    while(trevogaIsPressed == false)
	{
		if(((BUTTONS_PIN & (1 << BUTTON_1)) == (0 << BUTTON_1)) || ((BUTTONS_PIN & (1 << BUTTON_2)) == (0 << BUTTON_2)))
		{
			trevogaIsPressed = true;
		}
	}
	
	soundOn();
	while(trevogaIsPressed == true)
	{
		if(counter < 4)
		{
			switch(point)
			{
				case 0:
				SOUND_DDR |= (1 << SOUND_OUT);
				OCR0A = 30; OCR0B = OCR0A/2;
				point = 1;
				break;
				
				case 1:
				OCR0A--; OCR0B = OCR0A/2;
				if(OCR0A < 10)
				{
					point = 2;
				}
				_delay_ms(10);
				break;
				
				case 2:
				OCR0A++; OCR0B = OCR0A/2;
				LED_PORT |= (1 << LED_OUT);
				if(OCR0A > 100)
				{
					LED_PORT &= ~(1 << LED_OUT);
					counter++;
					point = 0;
				}
				_delay_ms(10);
				break;
			}
		}
			
		else
		{
			switch(point)
			{
				case 0:
				OCR0A = 30; OCR0B = OCR0A/2;
				point = 1;
				break;
				
				case 1:
				for(uint8_t i = 0; i < 4; i++)
				{
					SOUND_DDR |= (1 << SOUND_OUT);
					LED_PORT |= (1 << LED_OUT);
					_delay_ms(500);
					SOUND_DDR &= ~(1 << SOUND_OUT);
					LED_PORT &= ~(1 << LED_OUT);
					_delay_ms(300);
				}
				point = 2;
				break;
				
				case 2:
				counter = 0;
				point = 0;
				break;
			}
		}	
	}
}