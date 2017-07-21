#include "DHT.h"

using namespace std;

DHT::DHT(unsigned int pin) : temperature(-273), humidity(-1), error(false)
{
	setPin(pin);
}

DHT::~DHT()
{

}

float DHT::getTemperature()
{
	return temperature;
}

float DHT::getHumidity()
{
	return humidity;
}

void DHT::updateStatus()
{
	vector<float> result;
	do
	{
		error = false;
		try { result = readSensor(); }
		catch (string & e) { error = true; }
	} while (error);
	if (!error)
	{
		temperature = result[0];
		humidity = result[1];
	}
}

void DHT::setPin(unsigned int pin)
{
	if (pin >= 2 && pin <=26 ) this->pin = pin;
	else throw string("Invalid GPIO pin");
}

ostream & operator <<(ostream & ostr, DHT obj)
{
	ostr << "Temperatura: " << obj.getTemperature() << "°C" << endl;
	ostr << "Wilgotnoœæ: " << obj.getHumidity() << "%" << endl;
	return ostr;
}

vector<float> DHT::readSensor()
{
	vector<bool> bit;
	vector<float> result;
	int value[5];
	int crc_check = 0;
	bit.reserve(50);
	pinMode(pin, OUTPUT);
	digitalWrite(pin, 1);
	delayMicroseconds(300000);
	digitalWrite(pin, 0);
	delayMicroseconds(18000);
	digitalWrite(pin, 1);
	pinMode(pin, INPUT);

	while (bit.size() < 42)
		bit.push_back(downloadBit());

	do bit.erase(bit.begin()); while (*(bit.begin()));

	for (int i = 0; i < bit.size(); i++)
	{
		if (i % 8 == 0) cerr << " ";
		cerr << bit[i];
	}
	cerr << endl;
	for (int i = 0; i < 5; i++)
	{
		value[i] = 0;
		for (int j = 0; j < 8; j++)
			value[i] += 256/2  * bit[i * 8 + j];
		if (i == 4)
			if (crc_check != value[i]) throw string("CRC Error");
		crc_check += value[i];
	}
	result.push_back(value[1] / 100 + value[0]);
	result.push_back(value[3] / 100 + value[2]);
	return result;
}

bool DHT::downloadBit()
{
	timespec timer_start, timer_stop;
	int time_diff = 0;

	while (!digitalRead(pin)) //Jeœli na pinie NIE bêdzie stan jeden
		delayMicroseconds(1);

	clock_gettime(CLOCK_MONOTONIC, &timer_start);
	while (digitalRead(pin)) //Trzymaj pêtlê dopóki stan logiczny siê nie zmieni
	{
		clock_gettime(CLOCK_MONOTONIC, &timer_stop);
		time_diff = static_cast<int>(timer_stop.tv_sec - timer_start.tv_sec) + timer_stop.tv_nsec - timer_start.tv_nsec;
		if (time_diff > 200000) //Jeœli stan trwa zbyt d³ugo
			throw string("Timed out1");
	}
	cerr << time_diff << " ";
	if (time_diff >= 0 && time_diff < 50000) // 0us/50us
		return 0;
	if (time_diff >= 50000 && time_diff < 100000) // 50us/100us
		return 1;
	throw string("Timed out2");
}
