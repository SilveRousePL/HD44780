#pragma once
#include <string>
#include <wiringPi.h>

class HD44780
{
	enum Mode { CMD, CHR };
	enum Line { LINE_1, LINE_2, LINE_3, LINE_4 };
	enum Direction { L, R };
	enum Position { LEFT, RIGHT, CENTER };
	enum Pin { RS, E, D7, D6, D5, D4, RW, LED, D3, D2, D1, D0 };

	const int DELAY = 50;
	int pin[12];
	int line[4];
	bool display_on = true;
	bool cursor_on = false;
	bool cursor_blink = false;
	bool cursor_direction = RIGHT;
	bool text_shift = false;
	int width = 16;

public:
	HD44780(int RS_pin, int E_pin, int D7_pin, int D6_pin, int D5_pin, int D4_pin, 
			int line_1 = 128, int line_2 = 0, int line_3 = 0, int line_4 = 0, int RW_pin = 0, 
			int LED_pin = 0, int D3_pin = 0, int D2_pin = 0, int D1_pin = 0, int D0_pin = 0);
	~HD44780();

	void displayOn();
	void displayOff();
	void clearDisplay();

	void cursorOn(bool blink = false);
	void cursorOff();
	void cursorDirection(Direction direction);

	void returnHome();
	void shiftCursor(int iteration = 1);
	void shiftText(int iteration, Direction direction);

	void setCursorPosition(Line line, int position);
	void setText(std::string text);
	void setChar(int ch);

	void defineChar(int which, int row_1, int row_2, int row_3, int row_4, int row_5, int row_6, int row_7, int row_8 = 0, int row_9 = 0, int row_10 = 0);
	void deleteChar(int which);

	//Only with RW pin
	std::string getText();
	int getChar();

private:
	void writeData(int byte, bool mode);
	int readData(bool mode);

	void setInterface(bool value); //false - 4-bit, true - 8-bit
	void textShift(bool value); //true or false
	void setLinesAddresses(int line_1 = 128, int line_2 = 0, int line_3 = 0, int line_4 = 0);

	int currentAddress();
	bool isBusy();

	void initDisplay();
	bool isCorrectPin();
};

