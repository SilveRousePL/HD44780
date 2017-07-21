#include "HD44780.h"
#include <wiringPi.h>

using namespace std;

HD44780::HD44780(int RS_pin, int E_pin, int D7_pin, int D6_pin, int D5_pin, int D4_pin, 
				 int line_1 = 128, int line_2 = 0, int line_3 = 0, int line_4 = 0, int RW_pin = 0, 
				 int LED_pin = 0, int D3_pin = 0, int D2_pin = 0, int D1_pin = 0, int D0_pin = 0)
{
	//Przypisanie pinów
	pin[RS] = RS_pin; pin[E] = E_pin; pin[D7] = D7_pin; pin[D6] = D6_pin;
	pin[D5] = D5_pin; pin[D4] = D4_pin; pin[RW] = RW_pin; pin[LED] = LED_pin;
	pin[D3] = D3_pin; pin[D2] = D2_pin; pin[D1] = D1_pin; pin[D0] = D0_pin;
	line[LINE_1] = line_1; line[LINE_2] = line_2; line[LINE_3] = line_3; line[LINE_4] = line_4;
	if (!isCorrectPin()) throw string("Pins numbers is not correct");
	initDisplay();
}

HD44780::~HD44780() {}

void HD44780::writeData(int byte, bool mode)
{
	digitalWrite(pin[RS], mode);

	if(byte >= 128) { digitalWrite(pin[D4], true); byte -= 128; } else digitalWrite(pin[D4], false);
	if(byte >= 64) { digitalWrite(pin[D5], true); byte -= 64; } else digitalWrite(pin[D5], false);
	if(byte >= 32) { digitalWrite(pin[D6], true); byte -= 32; } else digitalWrite(pin[D6], false);
	if(byte >= 16) { digitalWrite(pin[D7], true); byte -= 16; } else digitalWrite(pin[D7], false);

	if(pin[D0] == 0 || pin[D1] == 0 || pin[D2] == 0 || pin[D3] == 0) //4-bit interface
	{
		delayMicroseconds(DELAY);
		digitalWrite(pin[E], true);
		delayMicroseconds(DELAY);
		digitalWrite(pin[E], false);
		delayMicroseconds(DELAY);

		if(byte >= 8) { digitalWrite(pin[D4], true); byte -= 8; } else digitalWrite(pin[D4], false);
		if(byte >= 4) { digitalWrite(pin[D5], true); byte -= 4; } else digitalWrite(pin[D5], false);
		if(byte >= 2) { digitalWrite(pin[D6], true); byte -= 2; } else digitalWrite(pin[D6], false);
		if(byte >= 1) { digitalWrite(pin[D7], true); byte -= 1; } else digitalWrite(pin[D7], false);
	}
	else //8-bit interface
	{
		if(byte >= 8) { digitalWrite(pin[D0], true); byte -= 8; } else digitalWrite(pin[D0], false);
		if(byte >= 4) { digitalWrite(pin[D1], true); byte -= 4; } else digitalWrite(pin[D1], false);
		if(byte >= 2) { digitalWrite(pin[D2], true); byte -= 2; } else digitalWrite(pin[D2], false);
		if(byte >= 1) { digitalWrite(pin[D3], true); byte -= 1; } else digitalWrite(pin[D3], false);
	}
	delayMicroseconds(DELAY);
	digitalWrite(pin[E], true);
	delayMicroseconds(DELAY);
	digitalWrite(pin[E], false);
}

void HD44780::initDisplay()
{
	for (int i = 0; i < 12; i++)
	{
		pinMode(pin[RS], OUTPUT);
	}
}

bool HD44780::isCorrectPin()
{
	for (int i = 0; i <= 5; i++)
		if (pin[i] < 2 || pin[i] > 26) return false;
	for (int i = 6; i <= 11; i++)
		if ((pin[i] < 2 || pin[i] > 26) && pin[i] != 0) return false;

	for (int i = 0; i <= 11; i++)
	{
		for (int j = 0; j <= 11; j++)
		{
			if (i == j) continue;
			if (pin[i] == pin[j]) return false;
		}
	}
	return true;
}
