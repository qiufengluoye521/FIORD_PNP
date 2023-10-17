#include "Storage_Manager.h"

static PT_StorageOpr g_ptStorageOprHead;

int8_t RegisterStorageOpr(PT_StorageOpr ptStorageOpr)
{
    PT_StorageOpr ptTmp;

    if(!g_ptStorageOprHead)
    {
        g_ptStorageOprHead   = ptStorageOpr;
        ptStorageOpr->ptNext = NULL;
    }
    else
    {
        ptTmp = g_ptStorageOprHead;
        while(ptTmp->ptNext)
        {
            ptTmp = ptTmp->ptNext;
        }
        ptTmp->ptNext           = ptStorageOpr;
        ptStorageOpr->ptNext      = NULL;
    }
}

int32_t Storage_set_parament(int8_t *name,PT_StorageParament para)
{
    PT_StorageOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_ptStorageOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->setStorageParament(para);
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return -1;
}



int32_t Storage_get_parament(int8_t *name,PT_StorageParament para)
{
    PT_StorageOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_ptStorageOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->getStorageParament(para);
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return -1;
}

void storage_driver_init(void)
{
    PT_StorageOpr ptTmp;

    inner_flash_init();

    ptTmp = g_ptStorageOprHead;
    while(ptTmp)
    {
        ptTmp->StorageOprInit();
        ptTmp = ptTmp->ptNext;
    }
}
