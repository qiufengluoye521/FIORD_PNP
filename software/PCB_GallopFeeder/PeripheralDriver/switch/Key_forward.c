#include "Key_forward.h"
#include "Switch_Manager.h"

static int32_t m_switchforward_status = 1;

static int8_t Key_forward_HardInit(void);
static int32_t get_Keyforward_Status(void);

static T_SwitchOpr g_tswitchOpr_forward = {
    .name                           = "Key_forward",
    .isCanUse                       = 1,
    .SwitchOprInit                  = Key_forward_HardInit,
    .getSwitchStatus                = get_Keyforward_Status,
};


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static int8_t Key_forward_HardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pins : PB1 */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1))
    {
        m_switchforward_status = 1;
    }
    else
    {
        m_switchforward_status = 0;
    }

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

    return 0;
}

static int32_t get_Keyforward_Status(void)
{
    if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1))
    {
        m_switchforward_status = 1;
    }
    else
    {
        m_switchforward_status = 0;
    }
    return m_switchforward_status;
}

int8_t Key_forward_init(void)
{
    RegisterSwitchOpr(&g_tswitchOpr_forward);

    return 0;

}

