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

typedef struct USART_t{

    __IO uint32_t USART_SR;
    __IO uint32_t USART_DR;
    __IO uint32_t USART_BRR;
    __IO uint32_t USART_CR1;
    __IO uint32_t USART_CR2;
    __IO uint32_t USART_CR3;
    __IO uint32_t USART_GTPR;
}USART_t;

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
USART_t * const USART2 = (USART_t *)0x40004400;

void ms_wait(int time){
    for(int i = 0; i < time; i++){
        for(int j = 0; j < 1600; j++);
    }
}

void USART2_init(void){

    // 1. Enable GPIOA Clock
    RCC->RCC_AHB1ENR |= 1;

    // 2. Enable UART2 Clock P17
    RCC->RCC_APB1ENR |= (1 << 17);

    // 3. Configure AF07 for PA2 -> UART2_TX 
    GPIOA->GPIOx_AFRL &= ~(0xF << (2 * 4));
    GPIOA->GPIOx_AFRL |=  (7   << (2 * 4));

    // 4. Enable Alternjate Function for PA2 over MODER by setting 0b10
    GPIOA->GPIOx_MODER &= ~(3 << (2 * 2));
    GPIOA->GPIOx_MODER |=  (7 << (2 * 2));

    // 5. Set Baudrate to 9600 -> BRR = USARTDIV -> 16/(Sample * Baud) = 104.166666 -> Mantissa = 104 | Fraction = (USARTDIV - Mantissa) * oversample ~ 3
    USART2->USART_BRR = ((0x68 << 4) | 0x3);

    // 6. Enable TX CR1[3] , set word length to 8 CR1[12]
    USART2->USART_CR1 |= (1 << 3);
    USART2->USART_CR1 &= ~(1 << 12);

    // 7. Set 1 Stop Bit
    USART2->USART_CR2 = 0x0000;
    
    // 8. Disable flow control
    USART2->USART_CR3 = 0x0000;

    // 9. Enable CR1[13] USART Enable
    USART2->USART_CR1 |= (1 << 13);
}

void USART2_write(uint8_t ch){
    while(!((USART2->USART_SR & (1 << 7))));
    USART2->USART_DR = (ch & 0xFF);
}

int main(void){


    uint8_t word[13] = "Hello There\n\0";
    
    USART2_init();

    for(;;){

        for(int i = 0; i < 12; i++){

            USART2_write(word[i]);
        }
        ms_wait(2000);
    }
}
