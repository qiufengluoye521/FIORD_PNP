#include "Commit_Manager.h"

static PT_CommitOpr g_pCommitOprHead;

int8_t RegisterCommitOpr(PT_CommitOpr ptCommitOpr)
{
    PT_CommitOpr ptTmp;

    if(!g_pCommitOprHead)
    {
        g_pCommitOprHead   = ptCommitOpr;
        ptCommitOpr->ptNext = NULL;
    }
    else
    {
        ptTmp = g_pCommitOprHead;
        while(ptTmp->ptNext)
        {
            ptTmp = ptTmp->ptNext;
        }
        ptTmp->ptNext           = ptCommitOpr;
        ptCommitOpr->ptNext      = NULL;
    }
}

int32_t Commit_Tranfer_Byte(int8_t *name,uint8_t data)
{
    PT_CommitOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_pCommitOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->Commit_TransferByte(data);
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

int32_t Commit_Tranfer_Buffer(int8_t *name,uint8_t *buffer,uint16_t size)
{
    PT_CommitOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_pCommitOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->Commit_TransferBuffer(buffer,size);
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

uint8_t Commit_get_message_ready_flag(int8_t *name,uint8_t *buff,uint16_t *size)
{
    PT_CommitOpr ptTmp;
    uint32_t len = 0;
    uint8_t flag = 0;

    ptTmp = g_pCommitOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            flag = ptTmp->get_message_ready_flag(buff,size);
            return flag;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

int32_t Commit_set_message_ready_flag(int8_t *name,uint8_t flag)
{
    PT_CommitOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_pCommitOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->set_message_ready_flag(flag);
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return -1;
}

void Commit_driver_init(void)
{
    PT_CommitOpr ptTmp;

    Commit_Rs485_init_01();

    ptTmp = g_pCommitOprHead;
    while(ptTmp)
    {
        ptTmp->Commit_Init();
        ptTmp = ptTmp->ptNext;
    }
}
