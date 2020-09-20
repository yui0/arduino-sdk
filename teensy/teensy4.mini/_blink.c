#include "WProgram.h"

int main()
{
	pinMode(13, OUTPUT);
	digitalWriteFast(13, HIGH);

	while (1) {
		//Serial.println("Hello World...");
//		usb_seremu_write("Hello World...\n\r", 16);
		digitalWriteFast(13, HIGH);
		delay(500);
		digitalWriteFast(13, LOW);
		delay(500);
	}
}
