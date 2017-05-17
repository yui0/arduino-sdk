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
	uint32_t a;
	if (!fifo_read(&usb_audio_fifo, &a)) return;

	int16_t left = a & 0xffff;	// left
	int16_t right = a>>16;		// right
	left += 32767;
	right += 32767;
	uint16_t l = left;
	uint16_t r = right;

	// PWM
	analogWrite(8, (l&0xff));
	analogWrite(9, (l>>8));
	analogWrite(29, (r&0xff));
	analogWrite(30, (r>>8));

	// DAC
	uint8_t d = r>>8;
	analogWrite(A21, d);
#endif

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
	pinMode(8, OUTPUT);
	analogWriteFrequency(8, 234375);
	pinMode(9, OUTPUT);
	analogWriteFrequency(9, 234375);
	//analogWriteFrequency(9, 160000);
	pinMode(29, OUTPUT);
	analogWriteFrequency(29, 234375);
	pinMode(30, OUTPUT);
	analogWriteFrequency(30, 234375);

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

