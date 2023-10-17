#include "Motor_Manager.h"

static PT_MotorMoveOpr g_ptMotorOprHead;

int8_t RegisterMotorOpr(PT_MotorMoveOpr ptMotorOpr)
{
    PT_MotorMoveOpr ptTmp;

    if(!g_ptMotorOprHead)
    {
        g_ptMotorOprHead   = ptMotorOpr;
        ptMotorOpr->ptNext = NULL;
    }
    else
    {
        ptTmp = g_ptMotorOprHead;
        while(ptTmp->ptNext)
        {
            ptTmp = ptTmp->ptNext;
        }
        ptTmp->ptNext           = ptMotorOpr;
        ptMotorOpr->ptNext      = NULL;
    }
}

int32_t Motor_Enable(int8_t *name)
{
    PT_MotorMoveOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_ptMotorOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->MotorEnable();
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

int32_t Motor_PrintInfo(int8_t *name)
{
    PT_MotorMoveOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_ptMotorOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->Motor_Debug_Process();
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

int32_t Motor_Disable(int8_t *name)
{
    PT_MotorMoveOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_ptMotorOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->MotorDisable();
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

int32_t Motor_MoveForward(int8_t *name,uint8_t distence)
{
    PT_MotorMoveOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_ptMotorOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->MotorForward(distence);
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

int32_t Motor_MoveBackward(int8_t *name,uint8_t distence)
{
    PT_MotorMoveOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_ptMotorOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->MotorBackward(distence);
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

int32_t Motor_Brake(int8_t *name)
{
    PT_MotorMoveOpr ptTmp;
    uint32_t len = 0;

    ptTmp = g_ptMotorOprHead;
    len = strlen((const char *)name);

    while(ptTmp)
    {
        if( 0 == memcmp(name,ptTmp->name,len) )
        {
            ptTmp->MotorBrake();
            return 0;
        }
        ptTmp = ptTmp->ptNext;
    }
    return 0;
}

void motor_driver_init(void)
{
    PT_MotorMoveOpr ptTmp;

    n20_Motor_init_01();
    n20_Motor_init_02();

    ptTmp = g_ptMotorOprHead;
    while(ptTmp)
    {
        ptTmp->MotorMoveOprPositionInit();
        ptTmp = ptTmp->ptNext;
    }
}
