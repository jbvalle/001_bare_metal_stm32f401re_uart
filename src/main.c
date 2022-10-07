#include <stdint.h>

#define PIN_5 5
#define MODER_WIDTH 2
#define __IO volatile

typedef struct RCC_t{
	__IO uint32_t RCC_CR;
	__IO uint32_t RCC_PLLCFGT;
	__IO uint32_t RCC_CFGR;
	__IO uint32_t RCC_CIR;
	__IO uint32_t RCC_AHB1RSTR;
	__IO uint32_t RCC_AHB2RSTR;
	__IO uint32_t res1[2];
	__IO uint32_t RCC_APB1RSTR;
	__IO uint32_t RCC_APB2RSTR;
	__IO uint32_t res2[2];
	__IO uint32_t RCC_AHB1ENR;
	__IO uint32_t RCC_AHB2ENR;
	__IO uint32_t res3[2];
	__IO uint32_t RCC_APB1ENR;
	__IO uint32_t RCC_APB2ENR;
	__IO uint32_t res4[2];
	__IO uint32_t RCC_AHB1LPENR;
	__IO uint32_t RCC_AHB2LPENR;
	__IO uint32_t res5[2];
	__IO uint32_t RCC_APB1LPENR;
	__IO uint32_t RCC_APB2LPENR;
	__IO uint32_t res6[2];
	__IO uint32_t RCC_BDCR;
	__IO uint32_t RCC_CSR;
	__IO uint32_t res7[2];
	__IO uint32_t RCC_SSCGR;
	__IO uint32_t RCC_PLLI2SCFGR;
	__IO uint32_t res8;
	__IO uint32_t RCC_DCKCFGR;
}RCC_t;

typedef struct GPIOx_t{
	__IO uint32_t GPIOx_MODER; 		
	__IO uint32_t GPIOx_OTYPER;	
	__IO uint32_t GPIOx_OSPEEDER;	
	__IO uint32_t GPIOx_PUPDR;	
	__IO uint32_t GPIOx_IDR;
	__IO uint32_t GPIOx_ODR;	
	__IO uint32_t GPIOx_BSRR;	
	__IO uint32_t GPIOx_LCKR;
	__IO uint32_t GPIOx_AFRL;
	__IO uint32_t GPIOx_AFRH;
}GPIOx_t;


GPIOx_t * const GPIOA = (GPIOx_t *)0x40020000;
GPIOx_t * const GPIOB = (GPIOx_t *)0x40020400;
RCC_t   * const RCC   = (RCC_t   *)0x40023800;

void ms_wait(int time){
    for(int i = 0; i < time; i++){
        for(int j = 0; j < 1600; j++);
    }
}
int main(void){

    // Enable GPIOA
    RCC->RCC_AHB1ENR |= 1;

    // Set PA5 as OUTPUT
    GPIOA->GPIOx_MODER &= ~(3 << (PIN_5 * MODER_WIDTH));
    GPIOA->GPIOx_MODER |=  (1 << (PIN_5 * MODER_WIDTH));

    for(;;){
        // Toggle PA5
        GPIOA->GPIOx_ODR ^= (1 << PIN_5);
        ms_wait(100);
    }
}
