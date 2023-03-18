#include <stdint.h>
#include "stm32f3xx.h"

int main();                                                 //declare main funtion

extern uint32_t _data_flash;                                //declare variables for memory control
extern uint32_t _data_ram;
extern uint32_t _data_size;

extern uint32_t _bss_ram;
extern uint32_t _bss_size;

void xPortPendSVHandler( void ) __attribute__( ( naked ) ); //required by freeRTOS from port.c
void xPortSysTickHandler( void );                           //required by freeRTOS from port.c
void vPortSVCHandler( void ) __attribute__( ( naked ) );    //required by freeRTOS from port.c

void init(void)
{
    volatile uint8_t *ram_ptr = (volatile uint8_t *)&_data_ram; //assigning address of flash to ram
    const uint8_t *flash_ptr = (const uint8_t *)&_data_flash;
    for (int i = 0; i < (int)&_data_size; i++)              //copying all data from flast to ram
    {
        *ram_ptr = *flash_ptr;
        ram_ptr++;
        flash_ptr++;
    }

    ram_ptr = (volatile uint8_t *)&_bss_ram;                //initialize .bss section of RAM to zero
    for(int i = 0; i < (int)&_bss_size; i++)
    {
        ram_ptr[i] = 0u;
    }
}

void reset(void)                                            //reset haldler starts and executes init, main functions
{
    __asm volatile ("bkpt #0");
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));            // set CP10 and CP11 Full Access for FPU
    init();
    main();
    while(1)
    {

    }
}

void nmi(void)
{
    while(1)
    {

    }
}

void hardfault(void)
{    while(1)
    {

    }
}

extern uint32_t _initial_sp;
__attribute__((section(".vector_table"))) const uint32_t vector_table[128] = 
{
    (uint32_t)&_initial_sp,
    (uint32_t)&reset,
    (uint32_t)&nmi,
    (uint32_t)&hardfault,
    0u,0u,0u,0u,0u,0u,0u,                       //skiping of adresses between HardFauld and SVCall (see ref.man.)
    (uint32_t)&vPortSVCHandler,                 //required by freeRTOS
    0u,0u,
    (uint32_t)&xPortPendSVHandler,              //required by freeRTOS
    (uint32_t)&xPortSysTickHandler              //required by freeRTOS
};

void _close(void)
{
    __asm volatile ("bkpt #0");
}

int _fstat(int fd, void *buffer)
{
    __asm volatile ("bkpt #0");
    return 0;
}

void _getpid(void)
{
    __asm volatile ("bkpt #0");
}

void _isatty(void)
{
    __asm volatile ("bkpt #0");
}

void _kill(void)
{
    __asm volatile ("bkpt #0");
}

void _lseek(void)
{
    __asm volatile ("bkpt #0");
}

void _read(void)
{
    __asm volatile ("bkpt #0");
}

void _write(void)
{
    __asm volatile ("bkpt #0");
}

// int _write(int fd, const char *buf, int count)
// {
//     for(int i = 0; i < count; i++)
// 	{
// 		USART2->TDR = buf[i];
// 		while((USART2->ISR & USART_ISR_TC)==0);
// 	}

//     return count;
// }