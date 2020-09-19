#include <stdint.h>
#include "imxrt.h"

#define FASTRUN __attribute__ ((section(".fastrun") ))

extern volatile uint32_t systick_millis_count;
extern volatile uint32_t systick_cycle_count;
FASTRUN void systick_isr()
{
	systick_cycle_count = ARM_DWT_CYCCNT;
	systick_millis_count++;
}
FASTRUN void pendablesrvreq_isr()
{
}

void yield(void) __attribute__ ((weak));
void yield(void)
{
}

// pwm.c
void flexpwm_init(IMXRT_FLEXPWM_t *p)
{
	int i;

	p->FCTRL0 = FLEXPWM_FCTRL0_FLVL(15); // logic high = fault
	p->FSTS0 = 0x000F; // clear fault status
	p->FFILT0 = 0;
	p->MCTRL |= FLEXPWM_MCTRL_CLDOK(15);
	for (i=0; i < 4; i++) {
		p->SM[i].CTRL2 = FLEXPWM_SMCTRL2_INDEP | FLEXPWM_SMCTRL2_WAITEN
			| FLEXPWM_SMCTRL2_DBGEN;
		p->SM[i].CTRL = FLEXPWM_SMCTRL_FULL;
		p->SM[i].OCTRL = 0;
		p->SM[i].DTCNT0 = 0;
		p->SM[i].INIT = 0;
		p->SM[i].VAL0 = 0;
		p->SM[i].VAL1 = 33464;
		p->SM[i].VAL2 = 0;
		p->SM[i].VAL3 = 0;
		p->SM[i].VAL4 = 0;
		p->SM[i].VAL5 = 0;
	}
	p->MCTRL |= FLEXPWM_MCTRL_LDOK(15);
	p->MCTRL |= FLEXPWM_MCTRL_RUN(15);
}
void quadtimer_init(IMXRT_TMR_t *p)
{
	int i;

	for (i=0; i < 4; i++) {
		p->CH[i].CTRL = 0; // stop timer
		p->CH[i].CNTR = 0;
		p->CH[i].SCTRL = TMR_SCTRL_OEN | TMR_SCTRL_OPS | TMR_SCTRL_VAL | TMR_SCTRL_FORCE;
		p->CH[i].CSCTRL = TMR_CSCTRL_CL1(1) | TMR_CSCTRL_ALT_LOAD;
		// COMP must be less than LOAD - otherwise output is always low
		p->CH[i].LOAD = 24000;   // low time  (65537 - x) - 
		p->CH[i].COMP1 = 0;  // high time (0 = always low, max = LOAD-1)
		p->CH[i].CMPLD1 = 0;
		p->CH[i].CTRL = TMR_CTRL_CM(1) | TMR_CTRL_PCS(8) |
			TMR_CTRL_LENGTH | TMR_CTRL_OUTMODE(6);
	}
}
void pwm_init(void)
{
	//printf("pwm init\n");
	CCM_CCGR4 |= CCM_CCGR4_PWM1(CCM_CCGR_ON) | CCM_CCGR4_PWM2(CCM_CCGR_ON) |
		CCM_CCGR4_PWM3(CCM_CCGR_ON) | CCM_CCGR4_PWM4(CCM_CCGR_ON);
	CCM_CCGR6 |= CCM_CCGR6_QTIMER1(CCM_CCGR_ON) | CCM_CCGR6_QTIMER2(CCM_CCGR_ON) |
		CCM_CCGR6_QTIMER3(CCM_CCGR_ON) | CCM_CCGR6_QTIMER4(CCM_CCGR_ON);
	flexpwm_init(&IMXRT_FLEXPWM1);
	flexpwm_init(&IMXRT_FLEXPWM2);
	flexpwm_init(&IMXRT_FLEXPWM3);
	flexpwm_init(&IMXRT_FLEXPWM4);
	quadtimer_init(&IMXRT_TMR1);
	quadtimer_init(&IMXRT_TMR2);
	quadtimer_init(&IMXRT_TMR3);
}
