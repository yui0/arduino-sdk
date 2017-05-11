// ©2017 Yuichiro Nakada

#include "WProgram.h"

#define TIMER 50000000 / 44100

#define TEN 0x1
#define TIE 0x2

void pit0_isr()
{
	//digitalWrite(13, !digitalRead(13));
	analogWrite(9, usb_audio_receive_buffer[0]/256);
	PIT_TFLG0 = 1;
}

void setup()
{
	SIM_SCGC6 |= SIM_SCGC6_PIT;	// Enables/disables clock used by PIT timers
	PIT_MCR = 0x00;			// Enables[0] and disables[1] the PIT timers
	NVIC_ENABLE_IRQ(IRQ_PIT_CH0);
	PIT_LDVAL0 = TIMER;		// Sets the timer count value (50MHz)
	PIT_TCTRL0 = TIE;
	PIT_TCTRL0 |= TEN;
	PIT_TFLG0 |= 1;			// Flag to indicate timer waiting

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
	while (1) {
		loop();
		yield();
	}
}

