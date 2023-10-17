#ifndef __N20_MOTOR_02_H
#define __N20_MOTOR_02_H

#include "stdint.h"
#include "stm32g0xx_hal.h"

#define MOTOR_DERECTION_FORWARD        (1)
#define MOTOR_DERECTION_BACKWARD       (-1)

typedef void (*n20_signal_data_cb_t)(void);

int8_t n20_Motor_init_02(void);
int32_t exit_signal_register(n20_signal_data_cb_t fun);

#endif //__N20_MOTOR_02_H
