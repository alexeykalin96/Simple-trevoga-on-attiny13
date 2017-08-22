#ifndef Sound_H
#define Sound_H

class Sound
{
	private:
		static volatile uint8_t point;
		static volatile uint16_t timer;
		static volatile uint8_t counter;
	
	public:
		Sound();
		void soundOn(void);
		void soundOff(void);
		void PWM_Generator(void);
};

#endif