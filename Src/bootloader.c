#include "stm32g473xx.h"
#include "dfu_utility.h"
#include "stm32g4xx_hal.h"

extern uint32_t g_pfnVectors[2]; // 0 for estack, 1 for 

__attribute__((section(".loader")))
void bl_Reset_Handler()
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    if(READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE)) {
        // something bad happened, halt mcu
        while(1);
    }

    // bank swap
    __disable_irq();

    // check if bank2 has code
    if(*(int32_t *)0x08040800 > 0) {
        

        /* disable ART */
        CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
        CLEAR_BIT(FLASH->ACR, FLASH_ACR_ICEN | FLASH_ACR_DCEN);

        /* clear cache */
        SET_BIT(FLASH->ACR, FLASH_ACR_ICRST | FLASH_ACR_DCRST);

        __DSB();
        SYSCFG->MEMRMP |= SYSCFG_MEMRMP_FB_MODE;
        __ISB();

        /* enable ART */
        SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
        SET_BIT(FLASH->ACR, FLASH_ACR_ICEN | FLASH_ACR_DCEN);

    }

    SCB->VTOR = (intptr_t)&g_pfnVectors;


    /* restore interrupt capability */
    __enable_irq();

    // jump to app
    asm("mov    r0, 0x08000800\r\n"
        "ldr    r0, [r0]\r\n"
        "mov    sp, r0\r\n"
        "mov    r0, 0x08000800\r\n"
        "ldr    r0, [r0, 4]\r\n"
        "mov    pc, r0\r\n");
    

    while(1);

}


__attribute__((section(".loader")))
void bl_HardFault_Handler()
{
    while(1) {
        
    }
}