#ifndef __MOTOR_MANAGER_H
#define __MOTOR_MANAGER_H

#include "stdint.h"
#include "stddef.h"
#include "N20_Motor_01.h"
#include "N20_Motor_02.h"

typedef struct MotorMoveOpr {
    int8_t *name;
    uint32_t isCanUse;
    int8_t (*MotorMoveOprPositionInit)(void);
    int32_t (*MotorEnable)(void);
    int32_t (*MotorDisable)(void);
    int32_t (*MotorForward)(uint8_t distense);
    int32_t (*MotorBackward)(uint8_t distense);
    int32_t (*MotorBrake)(void);
    void (*Motor_Debug_Process)(void);
    struct MotorMoveOpr *ptNext;
}T_MotorMoveOpr,*PT_MotorMoveOpr;

void motor_driver_init(void);
int32_t Motor_Enable(int8_t *name);
int32_t Motor_Disable(int8_t *name);
int32_t Motor_MoveForward(int8_t *name,uint8_t distence);
int32_t Motor_MoveBackward(int8_t *name,uint8_t distence);
int32_t Motor_Brake(int8_t *name);
int32_t Motor_PrintInfo(int8_t *name);

#endif //__MOTOR_MANAGER_H
