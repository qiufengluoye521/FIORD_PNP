#ifndef __SWITCH_MANAGER_H
#define __SWITCH_MANAGER_H

#include "stdint.h"
#include "stddef.h"
#include "Key_01.h"

typedef struct SwitchOpr {
    int8_t *name;
    uint32_t isCanUse;
    int8_t (*SwitchOprInit)(void);
    int32_t (*getSwitchStatus)(void);
    struct SwitchOpr *ptNext;
}T_SwitchOpr,*PT_SwitchOpr;

int32_t Switch_get_status(int8_t *name);
void switch_driver_init(void);

#endif //__SWITCH_MANAGER_H
