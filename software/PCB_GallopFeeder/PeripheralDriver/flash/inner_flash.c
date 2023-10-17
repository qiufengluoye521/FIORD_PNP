#include "inner_flash.h"
#include "Storage_Manager.h"

#define STORAGE_DATA_INNER_ADDR                     (0x0800F800)

uint64_t WriteFlashData[4] = {0x11111111,0x22222222,0x33333333,0x44444444};//数据
uint64_t *Address;

static int8_t innerFlash_HardInit(void);
static int32_t get_innerflash_parament(PT_StorageParament para);
static int32_t set_innerflash_parament(PT_StorageParament para);

static T_StorageOpr g_tStorageOpr = {
    .name                           = "inner_flash",
    .isCanUse                       = 1,
    .StorageOprInit                 = innerFlash_HardInit,
    .getStorageParament             = get_innerflash_parament,
    .setStorageParament             = set_innerflash_parament,
};


static int8_t innerFlash_HardInit(void)
{
    ;  // Nothing
}

static int32_t get_innerflash_parament(PT_StorageParament para)
{
    void *p_data;
    uint32_t len;

    p_data = (void *)STORAGE_DATA_INNER_ADDR;
    len    = sizeof(T_StorageParament);
    memcpy(para,p_data,len);

    return 0;
}
#include "Debug_Print.h"
static int32_t set_innerflash_parament(PT_StorageParament para)
{
    uint32_t i = 0;
    uint32_t len = 0;
    uint64_t *p_data;

    //WriteFlash_test();
    //return 0;
    /* 1/4解锁FLASH*/
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    /* 2/4擦除FLASH*/
    /*初始化FLASH_EraseInitTypeDef*/
    /*擦除方式页擦除FLASH_TYPEERASE_PAGES，块擦除FLASH_TYPEERASE_MASSERASE*/
    /*擦除页数*/
    /*擦除地址*/
    FLASH_EraseInitTypeDef FlashSet;
    FlashSet.TypeErase   = TYPEERASE_PAGES;
    FlashSet.Banks       = FLASH_BANK_1;
    FlashSet.Page        = (FLASH_PAGE_NB - 1);                     // total 32,user last one page
    FlashSet.NbPages     = 1;
    /*设置PageError，调用擦除函数*/
    uint32_t PageError = 0;
    // 此处直接擦除后，写有用的数据，保存等操作user进行
    HAL_FLASHEx_Erase(&FlashSet, &PageError);
    /* 3/4对FLASH烧写*/
    len    = sizeof(T_StorageParament);
    p_data = (uint64_t *)para;
    printm(KERN_DEBUG "len:%d\r\n",len);
    for(i = 0;i < len;)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, STORAGE_DATA_INNER_ADDR + 8*i, p_data[i]);
        i += 8;
    }
    /* 4/4锁住FLASH*/
    HAL_FLASH_Lock();

    return 0;
}

void WriteFlash_test(void)
{
    WriteFlash(STORAGE_DATA_INNER_ADDR,WriteFlashData,4);
}

/*FLASH写入程序*/
void WriteFlash(uint32_t addr,uint64_t data[],uint32_t len)
{
    uint32_t i = 0;
    /* 1/4解锁FLASH*/
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    /* 2/4擦除FLASH*/
    /*初始化FLASH_EraseInitTypeDef*/
    /*擦除方式页擦除FLASH_TYPEERASE_PAGES，块擦除FLASH_TYPEERASE_MASSERASE*/
    /*擦除页数*/
    /*擦除地址*/
    FLASH_EraseInitTypeDef FlashSet;
    FlashSet.TypeErase   = TYPEERASE_PAGES;
    FlashSet.Banks       = FLASH_BANK_1;
    FlashSet.Page        = (FLASH_PAGE_NB - 1);                     // total 32,user last one page
    FlashSet.NbPages     = 1;
    /*设置PageError，调用擦除函数*/
    uint32_t PageError = 0;
    // 此处直接擦除后，写有用的数据，保存等操作user进行
    HAL_FLASHEx_Erase(&FlashSet, &PageError);
    /* 3/4对FLASH烧写*/
    for(i = 0;i < len;i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr + 8*i, data[i]);
    }
    /* 4/4锁住FLASH*/
    HAL_FLASH_Lock();
}

int8_t inner_flash_init(void)
{
	RegisterStorageOpr(&g_tStorageOpr);

    return 0;

}

