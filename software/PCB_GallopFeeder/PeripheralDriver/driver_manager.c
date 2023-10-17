#include "driver_manager.h"

int8_t driver_init(void)
{
    Commit_driver_init();
    motor_driver_init();
    switch_driver_init();
    storage_driver_init();
}
