#ifndef __USER_PROTOCOL_ANALYSIS_H__
#define __USER_PROTOCOL_ANALYSIS_H__

#include "stdint.h"
#include "stdio.h"
#include "Commit_Manager.h"

#define MAX_RX_DATA_LEN                 128

#define FIRMWARE_OTA_CMD                0x01
#define GET_PARA_CMD                    0x02
#define SET_PARA_CMD                    0x03
#define CONTROL_MOTOR_CMD               0x04

#define CTR_MOVE_FORWARD_KEY            0x01
#define CTR_MOVE_BACKWARD_KEY           0x02
#define CTR_PARA_STORE_KEY              0x01
#define CTR_PARA_TEST_KEY               0x02

#define STORAGE_PARA_FLAG_MOTOR_BIT     (0x01)

typedef struct
{
    uint8_t ID:8;
    uint8_t cmd:8;
    uint8_t key:8;
    uint8_t motor_index:8;
    uint8_t distence:8;
    uint8_t reserved0:8;
    uint8_t reserved1:8;
    uint8_t reserved2:8;
}motor_move_protocal_t;

typedef union
{
    uint8_t                 rx_buff[MAX_RX_DATA_LEN];
    motor_move_protocal_t   motor;
}rx_motor_t;


typedef struct
{
    uint8_t ID:8;
    uint8_t cmd:8;
    uint8_t key:8;
    uint8_t para_ID:8;
    uint32_t para_flag:32;
}storage_para_protocal_t;

typedef union
{
    uint8_t                   rx_buff[MAX_RX_DATA_LEN];
    storage_para_protocal_t   para;
}rx_para_t;

void User_Protocol_run(void);

#endif //__USER_PROTOCOL_ANALYSIS_H__
