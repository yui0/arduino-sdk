// ©2017 Yuichiro Nakada

#include "WProgram.h"

#define TIME	1000000/44100
//#define TIME	1000000/48000

void timerCallback0()
{
	//digitalWrite(13, !digitalRead(13));
//	analogWrite(9, usb_audio_receive_buffer[0]/256);

#if 0
	// sin wave
	static int r = 0;
	uint32_t a = sin(r/100.0 * 2*3.14)*255; r++;
	analogWrite(9, a);
#else
	uint8_t a;
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(8, a);
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(9, a);
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(10, a);
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(11, a);
#endif

	// PWM
//	analogWrite(10, a&0xff);
//	analogWrite(11, (a>>8)&0xff);
//	analogWrite(8, (a>>16)&0xff);
//	analogWrite(9, a>>24);
	//analogWrite(9, a&0xffff);

	// GPIO
	digitalWriteFast(0, a&0x01);
	digitalWriteFast(1, a&0x02);
	digitalWriteFast(2, a&0x04);
	digitalWriteFast(3, a&0x08);
	digitalWriteFast(4, a&0x10);
	digitalWriteFast(5, a&0x20);
	digitalWriteFast(6, a&0x40);
	digitalWriteFast(7, a&0x80);
}

void setup()
{
	// LED
	pinMode(13, OUTPUT);

	// PWM
	pinMode(10, OUTPUT);
	analogWriteFrequency(10, 234375);
	pinMode(11, OUTPUT);
	analogWriteFrequency(11, 234375);
	pinMode(8, OUTPUT);
	analogWriteFrequency(8, 234375);
	pinMode(9, OUTPUT);
//	analogWriteFrequency(9, 234375);
	analogWriteFrequency(9, 160000);

	analogWriteResolution(8);	// 8bit/Resolution (234375 Hz)
	//analogWriteResolution(16);	// 8bit/Resolution (915527 Hz)
	//analogWriteFrequency(9, 915527);

	// GPIO
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
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

