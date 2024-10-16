#include "stm32f3xx.h"
#include "uart_diag.h"
#include <FreeRTOS.h>
#include <task.h>
#include <stdint.h>

static void clock_init(void);
static void task1(void *pvParameters);
static void task2(void *pvParameters);

static long counter = 0;

int main()
{
    clock_init(); //adding a new comment
	uart_init();

	xTaskCreate(task1, "task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
	xTaskCreate(task2, "task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
	
	vTaskStartScheduler();

    while(1)
    {
	
    }
    return 0;
}

static void clock_init(void)
{
	RCC->CR   |= RCC_CR_HSEON; 				// set HSE ON for external oscilator 8 MHz
	while((RCC->CR & RCC_CR_HSERDY) == 0u) { }	// wait 6 cycles to HSE to stabilise

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN ; 	// on 
    __IO uint32_t tmpreg = RCC->AHBENR & (~RCC_AHBENR_GPIOAEN);
    (void)tmpreg;

    GPIOA->MODER |= GPIO_MODER_MODER5_0;  	// 0100 0000 0000 we put that value into moder

    /*FLASH*/
	FLASH->ACR|= FLASH_ACR_PRFTBE;			// enabling FLASH prefatch buffer
	FLASH->ACR &= ~FLASH_ACR_LATENCY;		// set all bits to 0 (to ensure)
	FLASH->ACR|= FLASH_ACR_LATENCY_2;		// Two wait sates, if 48 < HCLK ≤ 72 MHz
	/*FLASH*/

    RCC->CFGR |= RCC_CFGR_PLLMUL9;  		// multiplicator 9 , input 8*9=72MHz
	RCC->CFGR |= RCC_CFGR_PLLSRC;   		// PLL entry clock source, external selected as PLL input clock

	RCC->CR	  |= RCC_CR_PLLON; 				// start the PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0u) { }	// wait for PLL is ready
	
	RCC->CFGR &= ~RCC_CFGR_SW; 				// System clock switch to external
	RCC->CFGR |= RCC_CFGR_SW_PLL; 			// select PLL as system clock
	
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { } // wait for switch status
}

static void task1(void *pvParameters)
{
	(void) pvParameters;
	for(long i = 0;i < 1000000;i++)
	{
		//__asm volatile ("cpsid i" : : : "memory"); // Disable IRQ interrupts by setting the I-bit in the CPSR
		counter++;
		//__asm volatile ("cpsie i" : : : "memory"); // Enable IRQ interrupts by clearing the I-bit in the CPSR
	}
	for ( ; ; ) {
		vTaskDelay((TickType_t)1000 / portTICK_PERIOD_MS);
	}
}

static void task2(void *pvParameters)
{
	(void) pvParameters;

	for(long i = 0;i < 1000000;i++)
	{
		//__asm volatile ("cpsid i" : : : "memory"); // Disable IRQ interrupts by setting the I-bit in the CPSR
		counter++;
		//__asm volatile ("cpsie i" : : : "memory"); // Enable IRQ interrupts by clearing the I-bit in the CPSR
	}
	for ( ; ; ) {
			uart_send();
			vTaskDelay((TickType_t)1000 / portTICK_PERIOD_MS);
	}
}