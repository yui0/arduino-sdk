// ©2017 Yuichiro Nakada

#include "WProgram.h"

void setup()
{
	pinMode(13, OUTPUT);
}

void loop()
{
	if (usb_audio_receive_buffer[0]<0x7fff) digitalWriteFast(13, LOW);
	else digitalWriteFast(13, HIGH);
}

extern "C" int main()
{
	setup();
	while (1) {
		loop();
		yield();
	}
}

