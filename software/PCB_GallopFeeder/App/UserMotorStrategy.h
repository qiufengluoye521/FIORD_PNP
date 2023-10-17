#ifndef __USER_MOTOR_STRATERGY_H__
#define __USER_MOTOR_STRATERGY_H__

#include "stdint.h"

#define FEEDER_MODE_NORMAL                (0)
#define FEEDER_MODE_INSTALL               (1)     // 装料模式

#define FORWARD_KEY_INIT                  (0)
#define FORWARD_KEY_TIME_CNT              (1)
#define FORWARD_KEY_EVENT                 (2)
#define BACKWARD_KEY_INIT                 (0)
#define BACKWARD_KEY_TIME_CNT             (1)
#define BACKWARD_KEY_EVENT                (2)

int32_t user_motor_init(void);
void user_motor_run(void);

#endif  //__USER_MOTOR_STRATERGY_H__
