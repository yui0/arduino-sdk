// ©2017 Yuichiro Nakada

#include "WProgram.h"

#define TIME	1000000/44100

void timerCallback0()
{
	//digitalWrite(13, !digitalRead(13));
//	analogWrite(9, usb_audio_receive_buffer[0]/256);

	uint32_t a;
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(9, a&0xffff);
}

void setup()
{
	pinMode(13, OUTPUT);

	analogWriteResolution(8);	// 8bit/Resolution (234375 Hz)
	pinMode(9, OUTPUT);
	analogWriteFrequency(9, 234375);
}

void loop()
{
	if (usb_audio_receive_buffer[0]<0x7fff) digitalWriteFast(13, LOW);
	else digitalWriteFast(13, HIGH);
	delay(10);
}

extern "C" int main()
{
	setup();

	IntervalTimer timer0;
	timer0.begin(timerCallback0, TIME);
	while (1) {
		loop();
		yield();
	}
	timer0.end();
}

