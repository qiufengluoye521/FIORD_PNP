#include "UserMotorStrategy.h"
#include "UserProtocolAnalysis.h"
#include "driver_manager.h"
#include "Debug_Print.h"

extern T_StorageParament gt_storage_para;
static uint32_t g_motor_run_cnt = 0;
static uint8_t g_key_forward = 1;
static uint8_t g_key_backward = 1;
static uint8_t g_key_forward_status = 0;
// move to system files
static uint8_t g_feeder_mode = FEEDER_MODE_NORMAL;
static uint64_t g_system_tick_ms = 0;
static uint8_t g_forward_key_status = 0;
static uint32_t g_forward_time_cnt = 0;
static uint8_t g_backward_key_status = 0;
static uint32_t g_backward_time_cnt = 0;

int32_t user_motor_init(void)
{
    float angle = 0.3f;
    // test
    Motor_Enable("N20Motor_01");
    Motor_Enable("N20Motor_02");
    //Motor_MoveForward("N20Motor_02",2);
    //Motor_MoveBackward("N20Motor_02",2);
    //Commit_Tranfer_Byte("RS485_01",'a');
    //Commit_Tranfer_Buffer("RS485_01","Hello world!!!",14);
    //printm(KERN_DEBUG "data:%d\r\n",3);
}

void key_event_state_machine(uint8_t forward,uint8_t backward,uint32_t t)
{
    // Forward key detect
    if(FORWARD_KEY_INIT == g_forward_key_status)
    {
        if(0 == forward)
        {
            g_forward_time_cnt = 0;
            g_forward_key_status = FORWARD_KEY_TIME_CNT;
        }
        else
        {
            g_forward_time_cnt = 0;
        }
    }
    else if(FORWARD_KEY_TIME_CNT == g_forward_key_status)
    {
        if(0 == forward)
        {
            g_forward_time_cnt += t;
        }
        else
        {
            // 20ms内为误触发
            if(g_forward_time_cnt <= 20)
            {
                g_forward_key_status = FORWARD_KEY_INIT;
                g_forward_time_cnt   = 0;
                printm(KERN_DEBUG "press too short time \r\n");
            }
            // 20ms-2S内认为是短按
            else if(g_forward_time_cnt <= 2000)
            {
                g_forward_key_status = FORWARD_KEY_EVENT;
                // 事件回调 ... 以下函数暂时写到这里，为快速实现功能，后续隔离
                Motor_MoveForward("N20Motor_02",2);
                g_forward_key_status = FORWARD_KEY_INIT;
                g_forward_time_cnt   = 0;
                printm(KERN_DEBUG "press forward key \r\n");
            }
            // 2S-5S内认为是长按
            else if(g_forward_time_cnt <= 5000)
            {
                g_forward_key_status = FORWARD_KEY_EVENT;
                // 事件回调 ...
                g_forward_key_status = FORWARD_KEY_INIT;
                g_forward_time_cnt   = 0;
                printm(KERN_DEBUG "long press forward key \r\n");
            }
        }
    }

    // backward key detect
    if(BACKWARD_KEY_INIT == g_backward_key_status)
    {
        if(0 == backward)
        {
            g_backward_time_cnt = 0;
            g_backward_key_status = BACKWARD_KEY_TIME_CNT;
        }
        else
        {
            g_backward_time_cnt = 0;
        }
    }
    else if(BACKWARD_KEY_TIME_CNT == g_backward_key_status)
    {
        if(0 == backward)
        {
            g_backward_time_cnt += t;
        }
        else
        {
            // 20ms内为误触发
            if(g_backward_time_cnt <= 20)
            {
                g_backward_key_status = BACKWARD_KEY_INIT;
                g_backward_time_cnt   = 0;
                printm(KERN_DEBUG "press too short time \r\n");
            }
            // 20ms-2S内认为是短按
            else if(g_backward_time_cnt <= 2000)
            {
                g_backward_key_status = BACKWARD_KEY_EVENT;
                // 事件回调 ... 以下函数暂时写到这里，为快速实现功能，后续隔离
                Motor_MoveBackward("N20Motor_02",2);
                g_backward_key_status = BACKWARD_KEY_INIT;
                g_backward_time_cnt   = 0;
                printm(KERN_DEBUG "press backward key \r\n");
            }
            // 2S-5S内认为是长按
            else if(g_backward_time_cnt <= 5000)
            {
                g_backward_key_status = BACKWARD_KEY_EVENT;
                // 事件回调 ...
                Motor_MoveBackward("N20Motor_02",40);
                g_backward_key_status = BACKWARD_KEY_INIT;
                g_backward_time_cnt   = 0;
                printm(KERN_DEBUG "long press backward key \r\n");
            }
        }
    }
}

void user_motor_run(void)
{
    g_system_tick_ms += 10;
    g_key_forward = Switch_get_status("Key_forward");
    g_key_backward = Switch_get_status("Key_backward");
    // 开机3s内按下forward键盘，进入FEEDER_MODE_INSTALL装料模式
    if(g_system_tick_ms <= 3000)
    {
        if(0 == g_key_backward)
        {
            g_feeder_mode = FEEDER_MODE_INSTALL;
        }
        return;
    }
    // 3S之后按下forward键，从装料模式恢复为正常模式
    if( (FEEDER_MODE_INSTALL == g_feeder_mode) && (0 == g_key_backward) )
    {
        g_feeder_mode = FEEDER_MODE_NORMAL;
    }
    key_event_state_machine(g_key_forward,g_key_backward,10);
    if( (1 == Switch_get_status("Key_01")) && (gt_storage_para.flag && (1 << STORAGE_PARA_FLAG_MOTOR_BIT)) && (FEEDER_MODE_NORMAL == g_feeder_mode) )
    {
        Motor_MoveForward("N20Motor_01",0);
    }
    else
    {
        Motor_Brake("N20Motor_01");
    }
}

