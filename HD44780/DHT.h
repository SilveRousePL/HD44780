#pragma once
#include <wiringPi.h>
#include <ctime>
#include <iostream>
#include <vector>

class DHT
{
	unsigned pin;
	float temperature;
	float humidity;
	bool error;

public:
	DHT(unsigned int pin);
	~DHT();

	float getTemperature();
	float getHumidity();
	void updateStatus();
	void setPin(unsigned int pin);
	friend std::ostream& operator <<(std::ostream& ostr, DHT obj);

private:
	std::vector<float> readSensor();
	bool downloadBit();
};

