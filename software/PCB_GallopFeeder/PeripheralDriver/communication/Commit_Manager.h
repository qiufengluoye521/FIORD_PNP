#ifndef __COMMIT_MANAGER_H
#define __COMMIT_MANAGER_H

#include "stdint.h"
#include "stddef.h"
#include "rs485_01.h"

typedef struct CommitOpr {
    int8_t *name;
    uint32_t isCanUse;
    int8_t (*Commit_Init)(void);
    void (*Commit_TransferByte)(uint8_t data);
    void (*Commit_TransferBuffer)(uint8_t *buff,uint16_t Size);
    uint8_t (*get_message_ready_flag)(uint8_t *buff,uint16_t *size);
    void (*set_message_ready_flag)(uint8_t flag);
    void (*Commit_Debug)(void);
    struct MotorMoveOpr *ptNext;
}T_CommitOpr,*PT_CommitOpr;

int8_t RegisterCommitOpr(PT_CommitOpr ptCommitOpr);
void Commit_driver_init(void);
int32_t Commit_Tranfer_Byte(int8_t *name,uint8_t data);
int32_t Commit_Tranfer_Buffer(int8_t *name,uint8_t *buffer,uint16_t size);
uint8_t Commit_get_message_ready_flag(int8_t *name,uint8_t *buff,uint16_t *size);
int32_t Commit_set_message_ready_flag(int8_t *name,uint8_t flag);

#endif //__MOTOR_MANAGER_H
