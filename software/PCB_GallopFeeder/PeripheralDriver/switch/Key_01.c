#include "Key_01.h"
#include "Switch_Manager.h"

static int32_t m_switch01_status = 1;

static int8_t Key_01_HardInit(void);
static int32_t get_Key01_Status(void);

static T_SwitchOpr g_tswitchOpr_01 = {
    .name                           = "Key_01",
    .isCanUse                       = 1,
    .SwitchOprInit                  = Key_01_HardInit,
    .getSwitchStatus                = get_Key01_Status,
};


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static int8_t Key_01_HardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    
    /*Configure GPIO pin : PB10 */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10))
    {
        m_switch01_status = 1;
    }
    else
    {
        m_switch01_status = 0;
    }

    return 0;
}

static int32_t get_Key01_Status(void)
{
    if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10))
    {
        m_switch01_status = 1;
    }
    else
    {
        m_switch01_status = 0;
    }
    return m_switch01_status;
}

int8_t Key01_init(void)
{
    RegisterSwitchOpr(&g_tswitchOpr_01);

    return 0;

}

