#include "dfu_utility.h"
#include "stm32g4xx.h"
#include "stm32g4xx_hal_flash.h"


int erase_bank(int bank)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase = {.TypeErase=FLASH_TYPEERASE_MASSERASE, .Banks=FLASH_BANK_2};
    if(bank == 0)
        erase.Banks = FLASH_BANK_1;
    uint32_t err;
    int ret = HAL_FLASHEx_Erase(&erase, &err) == HAL_OK;

    HAL_FLASH_Lock();
    return ret;
}

void write_flash(int bank, uint32_t offset, const char *buf, int len)
{
    HAL_FLASH_Unlock();

    for(int i=0; i<len; i+=8) {
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, offset+i, *(uint64_t *)(buf+i)) != HAL_OK) {
            volatile int xd =0;
            xd++;
        }
    }

    HAL_FLASH_Lock();
}

void swap_bank_and_reset()
{
    __disable_irq();
    FLASH_OBProgramInitTypeDef obinit, obinit2 = {0};
    HAL_FLASHEx_OBGetConfig(&obinit);

    obinit2.OptionType = OPTIONBYTE_USER;
    obinit2.USERType = OB_USER_BFB2;
    if ((obinit.USERConfig) & (OB_BFB2_ENABLE)) /* BANK1 active for boot */
    {
        obinit2.USERConfig = OB_BFB2_DISABLE;
    }
    else
    {
        obinit2.USERConfig = OB_BFB2_ENABLE;
    }


    /* Initiating the modifications */
    HAL_StatusTypeDef result = HAL_FLASH_Unlock();

    /* program if unlock is successful */
    if ( result == HAL_OK )
    {
        result = HAL_FLASH_OB_Unlock();

        /* program if unlock is successful*/
        if ((READ_BIT(FLASH->CR, FLASH_CR_OPTLOCK) == RESET))
        {
            result = HAL_FLASHEx_OBProgram(&obinit2);
        }
        HAL_FLASHEx_OBGetConfig(&obinit);
        volatile int a = (obinit.USERConfig) & (OB_BFB2_ENABLE);
        a++;

        HAL_FLASH_OB_Lock();
        HAL_FLASH_Lock();
        if (result == HAL_OK)
        {
            HAL_FLASH_OB_Launch();
            //perform standard reset
            HAL_NVIC_SystemReset();
        }
    }

    // error
    __enable_irq();
}