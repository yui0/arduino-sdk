// ©2017 Yuichiro Nakada

#include "WProgram.h"

#define TIME	1000000/44100

void timerCallback0()
{
	//digitalWrite(13, !digitalRead(13));
//	analogWrite(9, usb_audio_receive_buffer[0]/256);

	uint32_t a;
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(6, a&0xff);
	analogWrite(7, (a>>8)&0xff);
	analogWrite(8, (a>>16)&0xff);
	analogWrite(9, a>>24);
	//analogWrite(9, a&0xffff);
}

void setup()
{
	pinMode(13, OUTPUT);

	pinMode(6, OUTPUT);
	analogWriteFrequency(6, 234375);
	pinMode(7, OUTPUT);
	analogWriteFrequency(7, 234375);
	pinMode(8, OUTPUT);
	analogWriteFrequency(8, 234375);
	pinMode(9, OUTPUT);
//	analogWriteFrequency(9, 234375);
	analogWriteFrequency(9, 160000);

	analogWriteResolution(8);	// 8bit/Resolution (234375 Hz)
	//analogWriteResolution(16);	// 8bit/Resolution (915527 Hz)
	//analogWriteFrequency(9, 915527);
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

