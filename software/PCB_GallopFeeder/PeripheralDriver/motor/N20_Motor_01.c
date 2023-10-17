#include "N20_Motor_01.h"
#include "Motor_Manager.h"

static int8_t N20_Motor_01_HardInit(void);
static int32_t N20_1_Enable(void);
static int32_t N20_1_Disable(void);
static int32_t N20_1_Forward(uint8_t distence);
static int32_t N20_1_Backward(uint8_t distence);
static int32_t N20_1_Brake(void);

static T_MotorMoveOpr g_tN20Motorpr_01 = {
    .name                           = "N20Motor_01",
    .isCanUse                       = 1,
    .MotorEnable                    = N20_1_Enable,
    .MotorDisable                   = N20_1_Disable,
    .MotorForward                   = N20_1_Forward,
    .MotorBackward                  = N20_1_Backward,
    .MotorBrake                     = N20_1_Brake,
    .MotorMoveOprPositionInit       = N20_Motor_01_HardInit,
};


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static int8_t N20_Motor_01_HardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

    /*Configure GPIO pin : PA15 */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PB3 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    return 0;
}

static int32_t N20_1_Enable(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
}

static int32_t N20_1_Disable(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}

static int32_t N20_1_Forward(uint8_t distence)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}

static int32_t N20_1_Backward(uint8_t distence)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

}

static int32_t N20_1_Brake(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}

int8_t n20_Motor_init_01(void)
{
    RegisterMotorOpr(&g_tN20Motorpr_01);

    return 0;

}

