#ifndef __INNER_FLASH_H
#define __INNER_FLASH_H

#include "stdint.h"
#include "stm32g0xx_hal.h"

void WriteFlash_test(void);
void WriteFlash(uint32_t addr,uint64_t data[],uint32_t len);
int8_t inner_flash_init(void);


#endif //__INNER_FLASH_H
