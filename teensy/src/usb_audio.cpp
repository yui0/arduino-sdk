// Berry Audio
// ©2017 Yuichiro Nakada

#include "WProgram.h"
//#include "TeensyDelay.h"
//#include "TimerOne.h"

#define TIME	1000000/44100	// 22
//#define TIME	1000000/48000	// 20

//#define MODE_16BIT

#if defined(__MK20DX128__)
#define FTM0_CH0_PIN 22
#define FTM0_CH1_PIN 23
#define FTM0_CH2_PIN  9
#define FTM0_CH3_PIN 10
#define FTM0_CH4_PIN  6
#define FTM0_CH5_PIN 20
#define FTM0_CH6_PIN 21
#define FTM0_CH7_PIN  5
#define FTM1_CH0_PIN  3
#define FTM1_CH1_PIN  4
#elif defined(__MK20DX256__)
#define FTM0_CH0_PIN 22
#define FTM0_CH1_PIN 23
#define FTM0_CH2_PIN  9
#define FTM0_CH3_PIN 10
#define FTM0_CH4_PIN  6
#define FTM0_CH5_PIN 20
#define FTM0_CH6_PIN 21
#define FTM0_CH7_PIN  5
#define FTM1_CH0_PIN  3
#define FTM1_CH1_PIN  4
#define FTM2_CH0_PIN 32
#define FTM2_CH1_PIN 25
#elif defined(__MKL26Z64__)
#define FTM0_CH0_PIN 22
#define FTM0_CH1_PIN 23
#define FTM0_CH2_PIN  9
#define FTM0_CH3_PIN 10
#define FTM0_CH4_PIN  6
#define FTM0_CH5_PIN 20
#define FTM1_CH0_PIN 16
#define FTM1_CH1_PIN 17
#define FTM2_CH0_PIN  3
#define FTM2_CH1_PIN  4
#elif defined(__MK64FX512__)
#define FTM0_CH0_PIN 22
#define FTM0_CH1_PIN 23
#define FTM0_CH2_PIN  9
#define FTM0_CH3_PIN 10
#define FTM0_CH4_PIN  6
#define FTM0_CH5_PIN 20
#define FTM0_CH6_PIN 21
#define FTM0_CH7_PIN  5
#define FTM1_CH0_PIN  3
#define FTM1_CH1_PIN  4
#define FTM2_CH0_PIN 29
#define FTM2_CH1_PIN 30
#define FTM3_CH0_PIN  2
#define FTM3_CH1_PIN 14
#define FTM3_CH2_PIN  7
#define FTM3_CH3_PIN  8
#define FTM3_CH4_PIN 35
#define FTM3_CH5_PIN 36
#define FTM3_CH6_PIN 37
#define FTM3_CH7_PIN 38
#elif defined(__MK66FX1M0__)
#define FTM0_CH0_PIN 22
#define FTM0_CH1_PIN 23
#define FTM0_CH2_PIN  9
#define FTM0_CH3_PIN 10
#define FTM0_CH4_PIN  6
#define FTM0_CH5_PIN 20
#define FTM0_CH6_PIN 21
#define FTM0_CH7_PIN  5
#define FTM1_CH0_PIN  3
#define FTM1_CH1_PIN  4
#define FTM2_CH0_PIN 29
#define FTM2_CH1_PIN 30
#define FTM3_CH0_PIN  2
#define FTM3_CH1_PIN 14
#define FTM3_CH2_PIN  7
#define FTM3_CH3_PIN  8
#define FTM3_CH4_PIN 35
#define FTM3_CH5_PIN 36
#define FTM3_CH6_PIN 37
#define FTM3_CH7_PIN 38
#define TPM1_CH0_PIN 16
#define TPM1_CH1_PIN 17
#endif
#define FTM_PINCFG(pin) FTM_PINCFG2(pin)
#define FTM_PINCFG2(pin) CORE_PIN ## pin ## _CONFIG

void timerCallback0()
{
	//digitalWrite(13, !digitalRead(13));

#if 0
	// sin wave
	static int r = 0;
	uint32_t a = sin(r/100.0 * 2*3.14)*255; r++;
	analogWrite(8, a);
	analogWrite(9, a);
	analogWrite(10, a);
	analogWrite(11, a);	// Not PWM!

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

#ifdef MODE_16BIT
	// PWM (16bit)
	analogWrite(10, l);
	analogWrite(21, r);
#else
	// PWM (8bit)
/*	analogWrite(9, (l&0xff));
	analogWrite(10, (l>>8));
	analogWrite(20, (r&0xff));
	analogWrite(21, (r>>8));*/

	FTM0_C2V = l&0xff;
	FTM_PINCFG(FTM0_CH2_PIN) = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	FTM0_C3V = l>>8;
	FTM_PINCFG(FTM0_CH3_PIN) = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;

	FTM0_C5V = r&0xff;
	FTM_PINCFG(FTM0_CH5_PIN) = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
	FTM0_C6V = r>>8;
	FTM_PINCFG(FTM0_CH6_PIN) = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
#endif

	// DAC
//	analogWrite(A21, r>>4);	// 12bit??
#endif

	// GPIO
/*	uint8_t d = r>>8;
	digitalWriteFast(0, d&0x01);
	digitalWriteFast(1, d&0x02);
	digitalWriteFast(2, d&0x04);
	digitalWriteFast(3, d&0x08);
	digitalWriteFast(4, d&0x10);
	digitalWriteFast(5, d&0x20);
	digitalWriteFast(6, d&0x40);
	digitalWriteFast(7, d&0x80);*/
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
	pinMode(13, OUTPUT);

	// PWM (FTM0)
	pinMode(9, OUTPUT);	// left
	pinMode(10, OUTPUT);
	pinMode(20, OUTPUT);	// right
	pinMode(21, OUTPUT);

#ifdef MODE_16BIT
	analogWriteResolution(16);		// 16bit/Resolution
#else
	analogWriteResolution(8);		// 8bit/Resolution (234375 Hz)
#endif
//	analogWriteFrequency(10, 937500);	// FTM0 6bit
//	analogWriteFrequency(10, 468750);	// FTM0 7bit (keybord: slow)
	analogWriteFrequency(10, 234375);	// FTM0 8bit (o)
	//analogWriteFrequency(10, 421875);	// FTM0 8bit / 216MHz $((216000000/2/256))
//	analogWriteFrequency(10, 117187.5);	// FTM0 9bit
//	analogWriteFrequency(10, 58593.75);	// FTM0 10bit
//	analogWriteFrequency(10, 29296.875);	// FTM0 11bit
//	analogWriteFrequency(10, 14648.437);	// FTM0 12bit (high: xx)
//	analogWriteFrequency(10, 915.527);	// FTM0 16bit (915.527 Hz xxxxx)

//	analogWriteFrequency(10, 281250);	// FTM0 8bit (o) $((108000000/(320000*8)))
/*	analogWriteFrequency(9, 234375);	// FTM0 8bit (o) $((180000000/234375)) = 768
	analogWriteFrequency(10, 234375);	// FTM0 8bit (o)
	analogWriteFrequency(20, 234375);	// FTM0 8bit (o)
	analogWriteFrequency(21, 234375);	// FTM0 8bit (o)*/

	// GPIO
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);

/*FTM0_CNT = 0;
FTM0_C0SC = 0x28;
FTM0_C1SC = 0x28;
FTM0_MOD = 256;	// 8bit
FTM0_C0V = 0;
FTM0_C1V = 0;
CORE_PIN9_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
CORE_PIN10_CONFIG = PORT_PCR_MUX(4) | PORT_PCR_DSE | PORT_PCR_SRE;
NVIC_SET_PRIORITY(IRQ_FTM0, 0);
NVIC_ENABLE_IRQ(IRQ_FTM0);
// Setup clock (starts counter)
FTM0_SC = FTM_SC_CLKS(1) | FTM_SC_PS(0) | FTM_SC_TOIE;*/
}

void loop()
{
	if (usb_audio_receive_buffer[0]<0x7fff) digitalWriteFast(13, LOW);
	else digitalWriteFast(13, HIGH);
	delay(10);
}

extern "C" int main()
{
//	TeensyDelay::begin();
//	TeensyDelay::addDelayChannel(timerCallback0);
//	Timer1.initialize(TIME);
//	Timer1.attachInterrupt(timerCallback0);
	setup();
	IntervalTimer timer0;
	timer0.begin(timerCallback0, TIME);
	while (1) {
		loop();
		yield();
//		TeensyDelay::trigger(TIME);
	}
	timer0.end();
}

