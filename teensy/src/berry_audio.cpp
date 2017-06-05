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
//	uint32_t a;
//	if (!fifo_read(&usb_audio_fifo, &a)) return;
//	int16_t left = a & 0xffff;	// left
//	int16_t right = a>>16;		// right

	int16_t *a;
	if (!(a = (int16_t *)fifo_read(&usb_audio_fifo))) return;
	int16_t left = *a++;		// left
	int16_t right = *a;		// right

//	left += 32767;
//	right += 32767;
	left += 32768;			// ??? noisy
	right += 32768;

	// PWM (8bit)
/*	uint16_t l = left;
	uint16_t r = right;
	FTM0_C2V = l&0xff;	// 9
	FTM0_C3V = l>>8;	// 10
	FTM0_C5V = r&0xff;	// 20
	FTM0_C6V = r>>8;	// 21
	FTM0_SYNC |= 0x80;	// set PWM value update*/

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

	// PWM (FTM0)
	pinMode(9, OUTPUT);	// left
	pinMode(10, OUTPUT);
	pinMode(20, OUTPUT);	// right
	pinMode(21, OUTPUT);

//	analogWriteResolution(8);		// 8bit/Resolution (234375 Hz)
//	analogWriteFrequency(10, 234375);	// FTM0 8bit (o)
	CORE_PIN9_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN10_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN20_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN21_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;

#if F_CPU > 16000000
#define F_TIMER (F_PLL/2)
#else 
#define F_TIMER (F_PLL)
#endif
	/*for (prescale = 0; prescale < 7; prescale++) {
		minfreq = (float)(ftmClock >> prescale) / 65536.0f;	//Use ftmClock instead of F_TIMER
		if (frequency >= minfreq) break;
	}*/
	//mod = (float)(ftmClock >> prescale) / frequency - 0.5f;	//Use ftmClock instead of F_TIMER
	//if (mod > 65535) mod = 65535;
// echo $((90000000/65536))=1373 echo $((180000000/234375))=768
	FTM0_SC = 0;
	FTM0_CNT = 0;
	FTM0_MOD = 384; // mod
//	FTM0_MOD = 333; // mod 90000000/270000
//	FTM0_MOD = 300; // mod 90000000/300000
	FTM0_SC = FTM_SC_CLKS(1) | FTM_SC_PS(0); // prescale

/*	FTM0_OUTMASK = 0xFF;            // Use mask to disable outputs
	CORE_PIN9_CONFIG  = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN10_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN20_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	CORE_PIN21_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;

	SIM_SCGC6 |= 0x03000000;        // enable FTM0 and FTM0 module clock
	SIM_SCGC5 = SIM_SCGC5 | 0x3E00; // enable port A/B/C/D/E clock
	FTM0_SC = 0x00;                 // FTM0 disabled
	FTM0_MOD = MODULO;              // Period register
	FTM0_CONF = 0xC0;               // set up BDM in 11
	FTM0_FMS = 0x00;                // clear the WPEN so that WPDIS is set in FTM0_MODE reg
	FTM0_MODE |= 0x05;              // enable write the FTM CnV register
	FTM0_SYNC = 0x02;               // PWM sync @ max loading point enable
	FTM0_C2SC = 0x28;               // High-Low_high for combined and complementary mode
	FTM0_C3SC = 0x28;
	FTM0_C5SC = 0x28;
	FTM0_C6SC = 0x28;

	// complementary and combined mode for CH0&CH1, CH2&CH3
	// dead timer and sync enabled for CH0&CH1, CH2&CH3
	FTM0_COMBINE = 0x00003333;
	//FTM0_DEADTIME = 0x0F;		// deadtime is 15 system clock cycles
	FTM0_DEADTIME = 0x80;		// DeadTimer prescale systemClk/4
	FTM0_DEADTIME |= 12;		// 1uS DeadTime, max of 63 counts of 48Mhz clock

	FTM0_C2V = 0;
	FTM0_C3V = MODULO/2;		// Duty Cycle 50%
	FTM0_C5V = 0;
	FTM0_C6V = MODULO/2;

	FTM0_CNTIN = 0x00;
	FTM0_SC = 0x08|1;			// PWM edge_alignment, system clock driving, dividing by 1
	FTM0_OUTMASK = 0x00;		// Use mask to enable outputs*/
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

