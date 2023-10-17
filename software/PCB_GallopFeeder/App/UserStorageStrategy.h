#ifndef __USER_STORAGE_STRATERGY_H__
#define __USER_STORAGE_STRATERGY_H__

#include "stdint.h"

int32_t user_storage_init(void);
int32_t user_storage_setpara(uint8_t* buff,uint8_t len);
int32_t user_storage_getpara(uint8_t* buff,uint8_t len);

#endif  //__USER_STORAGE_STRATERGY_H__
