#include <stdint.h>
#include "pwm.h"
#include "sam.h"

#define PIO_ABSR_PIOC 0x400E1270

/* We can use PWH1 at PB13 /

/* PWM in ASCII */
#define WPKEY 0x50574D
/* Apply WPCMD to all register groups */
#define WPRG 0b111111
/* Disable write protect SW for the given register groups */
#define WPCMD 0

/* For PWM_CLK */
#define DIVA 84
#define DIVB 84
#define PREA 0
#define PREB 0

/* For PWM_CMR0 */
/* Set channel prescaler to CLKA */
#define CPRE 0b1011
/* Set period as left aligned */
#define CALG 0
/* Set output waveform to start at a low level */
#define CPOL 1

#define PWM ((PWM_REGS*)0x40094000)

void pwm_init() {
    /* Power on PWM */
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    /* Set PB13 as peripheral B */
    PIOB->PIO_ABSR |= PIO_ABSR_P13;
    /* Disable PIO, enable peripheral, to control the corresponding pin */
    PIOB->PIO_PDR |= PIO_PDR_P13;

    /* Disable write protect for PWM registers */
    PWM->PWM_WPCR |= (WPKEY << 8) | (WPRG << 2) | (WPCMD);

    /* Set CLK as MCK / DIVA */
    PWM->PWM_CLK = (PREB << 24) | (DIVB << 16) | (PREA << 8) | (DIVA);
    PWM->PWM_CH1.PWM_CMR |= (CPOL << 9) | CPRE;
    /* Period = CPRD / (84MHz / DIVA) */
    PWM->PWM_CH1.PWM_CPRD = 20000; 
    PWM->PWM_CH1.PWM_CDTY = 10000;
    /* Enable PWM output for channel 1 */
    PWM->PWM_ENA = 0b00000010;

    /* Set PB12 as peripheral B */
    PIOB->PIO_ABSR |= PIO_ABSR_P12;
    /* Disable PIO, enable peripheral, to control the corresponding pin */
    PIOB->PIO_PDR |= PIO_PDR_P12;

    /* Disable write protect for PWM registers */
    PWM->PWM_WPCR |= (WPKEY << 8) | (WPRG << 2) | (WPCMD);

    /* Set CLK as MCK / DIVA */
    PWM->PWM_CH0.PWM_CMR |= (CPOL << 9) | CPRE;
    /* Period = CPRD / (84MHz / DIVA) */
    PWM->PWM_CH0.PWM_CPRD = 20000; 
    PWM->PWM_CH0.PWM_CDTY = 10000;
    /* Enable PWM output for channel 0 */
    PWM->PWM_ENA |= 0b00000001;
}

void pwm_set_duty_cycle(uint32_t dc) {
    if (dc > 2100) {
        PWM->PWM_CH1.PWM_CDTY = 2100;
    } else if (dc < 900) {
        PWM->PWM_CH1.PWM_CDTY = 900;
    } else {
        PWM->PWM_CH1.PWM_CDTY = dc;
    }
}