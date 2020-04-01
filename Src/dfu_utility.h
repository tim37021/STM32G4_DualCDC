

#ifndef LICAP_R_EVT_DFU_UTILITY_H
#define LICAP_R_EVT_DFU_UTILITY_H

#include "stm32g4xx.h"
/**
  * @brief  get current bank
  * @retval int 0 for bank 1, while 1 for bank 2
  */
static inline int get_bank()
{
    return READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == SYSCFG_MEMRMP_FB_MODE;
}

/**
  * @brief  boot_from_bank2
  */
static inline void boot_from_bank2()
{
    SET_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE);
}

/**
  * @brief  check if dual bank enabled
  * @retval int boolean value
  */
static inline int is_dual_bank_enabled();

int erase_bank(int bank);

void bank_swap();


// flash related function
void write_flash(int bank, uint32_t offset, const char *buf, int len);
int validate(int bank, int offset, uint32_t crc32);

#endif
