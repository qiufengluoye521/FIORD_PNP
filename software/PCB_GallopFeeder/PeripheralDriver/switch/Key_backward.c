#include "Key_backward.h"
#include "Switch_Manager.h"

static int32_t m_switchbackward_status = 1;

static int8_t Key_backward_HardInit(void);
static int32_t get_Keybackward_Status(void);

static T_SwitchOpr g_tswitchOpr_backward = {
    .name                           = "Key_backward",
    .isCanUse                       = 1,
    .SwitchOprInit                  = Key_backward_HardInit,
    .getSwitchStatus                = get_Keybackward_Status,
};


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static int8_t Key_backward_HardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pins : PB1 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0))
    {
        m_switchbackward_status = 1;
    }
    else
    {
        m_switchbackward_status = 0;
    }

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

    return 0;
}

static int32_t get_Keybackward_Status(void)
{
    if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0))
    {
        m_switchbackward_status = 1;
    }
    else
    {
        m_switchbackward_status = 0;
    }
    return m_switchbackward_status;
}

int8_t Key_backward_init(void)
{
    RegisterSwitchOpr(&g_tswitchOpr_backward);

    return 0;

}

