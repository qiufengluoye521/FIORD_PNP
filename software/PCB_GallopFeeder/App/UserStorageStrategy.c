#include "UserStorageStrategy.h"
#include "UserProtocolAnalysis.h"
#include "driver_manager.h"
#include "Debug_Print.h"

T_StorageParament gt_storage_para;

int32_t user_storage_init(void)
{
    Storage_get_parament("inner_flash",&gt_storage_para);
    printm(KERN_DEBUG "Feeder ID:%d\r\n",gt_storage_para.ID);
    printm(KERN_DEBUG "Feeder flag:0x%08x\r\n",gt_storage_para.flag);

}

int32_t user_storage_setpara(uint8_t* buff,uint8_t len)
{
    rx_para_t para_tmp;
    memcpy(para_tmp.rx_buff,buff,len);
    gt_storage_para.ID     = para_tmp.para.para_ID;
    gt_storage_para.flag   = para_tmp.para.para_flag;
    // 保存参数
    if(CTR_PARA_STORE_KEY == para_tmp.para.key)
    {
        Storage_set_parament("inner_flash",&gt_storage_para);
        Storage_get_parament("inner_flash",&gt_storage_para);
        if( (gt_storage_para.ID == para_tmp.para.para_ID) && (gt_storage_para.flag == para_tmp.para.para_flag) )
        {
            printm(KERN_DEBUG "parament saved\r\n");
            printm(KERN_DEBUG "ID:%d  flag:0x%08x\r\n",gt_storage_para.ID,gt_storage_para.flag);
        }
        else
        {
            printm(KERN_DEBUG "parament not saved !!! \r\n");
            printm(KERN_DEBUG "ID:%d  flag:0x%08x\r\n",gt_storage_para.ID,gt_storage_para.flag);
        }
    }
    else if(CTR_PARA_TEST_KEY == para_tmp.para.key)
    {
        printm(KERN_DEBUG "parament set just for test, will not saved in flash\r\n");
        printm(KERN_DEBUG "ID:%d  flag:0x%08x\r\n",gt_storage_para.ID,gt_storage_para.flag);
    }
}


int32_t user_storage_getpara(uint8_t* buff,uint8_t len)
{
    Storage_get_parament("inner_flash",&gt_storage_para);
    // send to master through rs485---TODO
}
