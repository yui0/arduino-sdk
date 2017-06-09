// Berry Audio
// ©2017 Yuichiro Nakada

#include "WProgram.h"

//#define TEST_MODE

#define TPM_C		72000000		// core clock
#define PWM_FREQ	(234375*2)		// PWM frequency * 2
#define MODULO		(TPM_C / PWM_FREQ)	// modulo for FTM0

#define TIME		1000000/44100		// 22
//#define TIME		1000000/48000		// 20

void timerCallback0()
{
#ifdef TEST_MODE2
	// sin wave
	static int c = 0, n = 0;
	uint16_t s = sin(c/100.0 * 2*3.14)*(sin(n/1000000.0*2*3.14)*65535); c++; n++;
	if (c>100) c = 0;
	if (n>1000000) n = 0;
	uint8_t *l = (uint8_t *)&s;
	FTM0_C2V = *l++;	// 9
	FTM0_C3V = *l;		// 10
	FTM0_SYNC |= 0x80;	// set PWM value update
#else
	int16_t *a;
	if (!(a = (int16_t *)fifo_read(&usb_audio_fifo))) return;
	int16_t left = *a++;		// left
	int16_t right = *a;		// right

//	left += 32767;
//	right += 32767;
	left += 32768;			// ??? noisy
	right += 32768;

	// PWM (8bit)
#ifdef TEST_MODE
	// sin wave
	static int c = 0, n = 0;
	uint16_t s = sin(c/100.0 * 2*3.14)*(sin(n/1000000.0*2*3.14)*65535); c++; n++;
	if (c>100) c = 0;
	if (n>1000000) n = 0;
	uint8_t *l = (uint8_t *)&s;
#else
	uint8_t *l = (uint8_t *)&left;
#endif
	uint8_t *r = (uint8_t *)&right;
	FTM0_C2V = *l++;	// 9
	FTM0_C3V = *l;		// 10
	FTM0_C5V = *r++;	// 20
	FTM0_C6V = *r;		// 21
	FTM0_SYNC |= 0x80;	// set PWM value update
#endif
}

void setup()
{
	// XXXX!!! CPU 240000000 Hz (mk20dx128.c)
//	MCG_C5 = MCG_C5_PRDIV0(0);
//	MCG_C6 = MCG_C6_PLLS | MCG_C6_VDIV0(14);
	// CPU 216000000 Hz (mk20dx128.c)
//	MCG_C5 = MCG_C5_PRDIV0(0);
//	MCG_C6 = MCG_C6_PLLS | MCG_C6_VDIV0(11);
	// CPU 180000000
//	MCG_C5 = MCG_C5_PRDIV0(1);
//	MCG_C6 = MCG_C6_PLLS | MCG_C6_VDIV0(29);

	// LED
//	pinMode(13, OUTPUT);

//	analogWriteResolution(8);		// 8bit/Resolution (234375 Hz)
//	analogWriteFrequency(10, 234375);	// FTM0 8bit (o)

#if F_CPU > 16000000
#define F_TIMER (F_PLL/2)
#else 
#define F_TIMER (F_PLL)
#endif
	FTM0_OUTMASK = 0xff;            // Use mask to disable outputs
	CORE_PIN9_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN10_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN20_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN21_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	FTM0_SC = 0;
	FTM0_CNT = 0;
	FTM0_MOD = 384; // mod 180000000Hz/2/234375Hz, 2160000000/2/384=2812500Hz
	FTM0_SC = FTM_SC_CLKS(1) | FTM_SC_PS(0); // prescale
	FTM0_OUTMASK = 0x00;		// Use mask to enable outputs
}

void loop()
{
//	if (usb_audio_receive_buffer[0]<0x7fff) digitalWriteFast(13, LOW);
//	else digitalWriteFast(13, HIGH);
	delay(1000);
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

