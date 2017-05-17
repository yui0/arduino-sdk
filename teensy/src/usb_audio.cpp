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
	analogWrite(8, a);
	analogWrite(9, a);
	analogWrite(10, a);
	analogWrite(11, a);

	analogWrite(A21, a);
	//tone(A21, r/100);
#else
	/*uint8_t a;
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(8, a);
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(9, a);
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(10, a);
	fifo_read(&usb_audio_fifo, &a);
	analogWrite(11, a);*/

	uint32_t a;
	if (!fifo_read(&usb_audio_fifo, &a)) return;
	/*analogWrite(8, (int8_t)(a&0xff));
	analogWrite(9, (int8_t)((a>>8)&0xff));
	analogWrite(10, (int8_t)((a>>16)&0xff));
	analogWrite(11, (int8_t)(a>>24));*/
	int16_t left = a & 0xffff;		// left
	int16_t right = (a>>16) & 0xffff0000;	// right
	left += 32767;
	right += 32767;
	analogWrite(8, (left&0xff));
	analogWrite(9, ((left>>8)&0xff));
	analogWrite(10, ((right>>16)&0xff));
	analogWrite(11, (right>>24));

	// DAC
	uint8_t d = right>>8;
	analogWrite(A21, d);
	//int16_t d = (int16_t)(a>>16)+32767;
	//analogWrite(A21, d/256);
//	analogWrite(A21, a);
	/*static int n = 0;
	analogWrite(A21, usb_audio_receive_buffer[n]/256);
	n += 2;
	if (n>=180*2) n = 0;*/
#endif

	// PWM
//	analogWrite(10, a&0xff);
//	analogWrite(11, (a>>8)&0xff);
//	analogWrite(8, (a>>16)&0xff);
//	analogWrite(9, a>>24);
	//analogWrite(9, a&0xffff);

	// GPIO
	digitalWriteFast(0, d&0x01);
	digitalWriteFast(1, d&0x02);
	digitalWriteFast(2, d&0x04);
	digitalWriteFast(3, d&0x08);
	digitalWriteFast(4, d&0x10);
	digitalWriteFast(5, d&0x20);
	digitalWriteFast(6, d&0x40);
	digitalWriteFast(7, d&0x80);
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

