#ifndef __STORAGE_MANAGER_H
#define __STORAGE_MANAGER_H

#include "stdint.h"
#include "stddef.h"
#include "Key_01.h"

typedef struct storage_parament
{
    uint32_t ID;
    uint32_t flag;
}T_StorageParament,*PT_StorageParament;

typedef struct StorageOpr {
    int8_t *name;
    uint32_t isCanUse;
    int8_t (*StorageOprInit)(void);
    int32_t (*getStorageParament)(PT_StorageParament para);
    int32_t (*setStorageParament)(PT_StorageParament para);
    struct StorageOpr *ptNext;
}T_StorageOpr,*PT_StorageOpr;

int32_t Storage_set_parament(int8_t *name,PT_StorageParament para);
int32_t Storage_get_parament(int8_t *name,PT_StorageParament para);
void storage_driver_init(void);
int8_t RegisterStorageOpr(PT_StorageOpr ptStorageOpr);

#endif //__STORAGE_MANAGER_H
