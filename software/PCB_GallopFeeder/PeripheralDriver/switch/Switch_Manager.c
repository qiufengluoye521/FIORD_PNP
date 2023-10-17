#include "Switch_Manager.h"

static PT_SwitchOpr g_ptSwitchOprHead;

int8_t RegisterSwitchOpr(PT_SwitchOpr ptSwitchOpr)
{
    PT_SwitchOpr ptTmp;

    if(!g_ptSwitchOprHead)
    {
        g_ptSwitchOprHead   = ptSwitchOpr;
        ptSwitchOpr->ptNext = NULL;
    }
    else
    {
        ptTmp = g_ptSwitchOprHead;
        while(ptTmp->ptNext)
        {
            ptTmp = ptTmp->ptNext;
        }
        ptTmp->ptNext           = ptSwitchOpr;
        ptSwitchOpr->ptNext      = NULL;
    }
}

int32_t Switch_get_status(int8_t *name)
{
    PT_SwitchOpr ptTmp;
    uint32_t len = 0;
    int32_t status;

    ptTmp = g_ptSwitchOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            status = ptTmp->getSwitchStatus();
            return status;
        }
        ptTmp = ptTmp->ptNext;
    }
    return -1;
}

void switch_driver_init(void)
{
    PT_SwitchOpr ptTmp;

    Key01_init();
    Key_forward_init();
    Key_backward_init();

    ptTmp = g_ptSwitchOprHead;
    while(ptTmp)
    {
        ptTmp->SwitchOprInit();
        ptTmp = ptTmp->ptNext;
    }
}
