#include "UserProtocolAnalysis.h"
#include "Debug_Print.h"
#include "stm32g0xx_hal.h"
#include "driver_manager.h"

static uint8_t m_message_buff[MAX_RX_DATA_LEN] = {0};
static uint16_t message_size = 0;
static uint32_t m_time_cnt = 0;

extern T_StorageParament gt_storage_para;

static int32_t protocol_motor_control(uint8_t* buff,uint8_t len)
{
    rx_motor_t motor_tmp;
    memcpy(motor_tmp.rx_buff,buff,len);
    if(CTR_MOVE_FORWARD_KEY == motor_tmp.motor.key)
    {
        if(1 == motor_tmp.motor.motor_index)
        {
            Motor_MoveForward("N20Motor_01",motor_tmp.motor.distence);
        } else if(2 == motor_tmp.motor.motor_index)
        {
            Motor_MoveForward("N20Motor_02",motor_tmp.motor.distence);
        }
        printm(KERN_DEBUG "motor%d move forward %d mm\r\n",motor_tmp.motor.motor_index,motor_tmp.motor.distence);
    }
    else if(CTR_MOVE_BACKWARD_KEY == motor_tmp.motor.key)
    {
        if(1 == motor_tmp.motor.motor_index)
        {
            Motor_MoveBackward("N20Motor_01",motor_tmp.motor.distence);
        } else if(2 == motor_tmp.motor.motor_index)
        {
            Motor_MoveBackward("N20Motor_02",motor_tmp.motor.distence);
        }
        printm(KERN_DEBUG "motor%d move backward %d mm\r\n",motor_tmp.motor.motor_index,motor_tmp.motor.distence);
    }
}

static void protocol_analysis(uint8_t* buff,uint8_t len)
{
    uint8_t buff2[MAX_RX_DATA_LEN];
    printm(KERN_DEBUG "protocol_analysis %d %d %d %d %d %d %d %d\r\n",
        buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7]);
    switch(buff[1])
    {
        case FIRMWARE_OTA_CMD:
            printm(KERN_DEBUG "function unrealized \r\n");
            break;
        case GET_PARA_CMD:
            user_storage_getpara(buff2,MAX_RX_DATA_LEN);
            break;
        case SET_PARA_CMD:
            // set para 不考虑ID是否匹配，通过有无按键确认是否需要设置此飞达
            if( (0 == Switch_get_status("Key_forward")) && (0 == Switch_get_status("Key_backward")) )
            {
                user_storage_setpara(buff,len);
            }
            else
            {
                printm(KERN_DEBUG "please press forward and backward key at the same time before you send this cmd!!! \r\n");
            }
            break;
        case CONTROL_MOTOR_CMD:
            // 命令ID与飞达ID匹配或命令ID为0(面对所有飞达)
            if( (gt_storage_para.ID == buff[0]) || (0 == buff[0]) )
            {
                protocol_motor_control(buff,len);
            }
            else
            {
                printm(KERN_DEBUG "none of my business. my ID:%d  cmd ID:%d\r\n",gt_storage_para.ID,buff[0]);
            }
            break;
        default:
            break;
    }
}

uint8_t parse_shell_cmd(uint8_t *cmd, uint8_t cnt)
{
    uint8_t value;
    float cur_angle = 0.0f;
    uint8_t cmd_ID = 0;

    if(memcmp(&cmd[6], "move forward", 12) == 0)
    {
        value  = (uint8_t)atoi((const char *)&cmd[19]);
        cmd_ID = (uint8_t)atoi((const char *)&cmd[3]);
        if( (gt_storage_para.ID == cmd_ID) || (0 == cmd_ID) )
        {
            Motor_MoveForward("N20Motor_02",value);
            printm(KERN_DEBUG "move forward %dmm \r\n",value);
        }
    }
    else if(memcmp(&cmd[6], "set feeder ID", 13) == 0)
    {
        printm(KERN_DEBUG "please press forward and backward button at the same time when send this command \r\n",value);
    }

    return 0;
}


void User_Protocol_run(void)
{
    m_time_cnt++;
    if( (m_time_cnt < 100) && (m_time_cnt >= 0) )
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    }
    else if(m_time_cnt < 200)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    }
    else
    {
        m_time_cnt = 0;
        Motor_PrintInfo("N20Motor_02");
    }
    if( Commit_get_message_ready_flag("RS485_01",m_message_buff,&message_size) )
    {
        parse_shell_cmd(m_message_buff,message_size);
        Commit_set_message_ready_flag("RS485_01",0);
    }
}
