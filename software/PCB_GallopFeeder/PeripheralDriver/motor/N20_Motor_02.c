#include "N20_Motor_02.h"
#include "Motor_Manager.h"
#include "Debug_Print.h"

#define N20_RELEASE_DELTA_DISTENCE          (1.0)

static int32_t m_signal_cnt = 0;
static int32_t m_signal2_cnt = 0;
static uint8_t g_last_signal_index = 0;
static uint8_t print_times = 0;
/* hal angle: 7 signals comes in 360 degree */
static double m_hal_angle  = 0;
/* reduction_ratio:1030:1 */
static double m_reduction_ratio_angle  = 0;
/* strip distence :32x4mm---360degree */
static double m_strap_distence  = 0;
static double m_target_distence  = 0;
static int8_t m_motor_direction = MOTOR_DERECTION_FORWARD;

TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

static int8_t N20_02_HardInit(void);
static int32_t N20_2_Enable(void);
static int32_t N20_2_Disable(void);
static int32_t N20_2_Forward(uint8_t distense);
static int32_t N20_2_Backward(uint8_t distense);
static int32_t N20_2_Brake(void);
static void N20_2_Debug_Process(void);

static T_MotorMoveOpr g_tN20Motorpr_02 = {
    .name                           = "N20Motor_02",
    .isCanUse                       = 1,
    .MotorMoveOprPositionInit       = N20_02_HardInit,
    .MotorEnable                    = N20_2_Enable,
    .MotorDisable                   = N20_2_Disable,
    .MotorForward                   = N20_2_Forward,
    .MotorBackward                  = N20_2_Backward,
    .MotorBrake                     = N20_2_Brake,
    .Motor_Debug_Process            = N20_2_Debug_Process,
};


/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 64-1;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 65535;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 7;
  if (HAL_TIM_IC_ConfigChannel(&htim17, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */
  HAL_TIM_IC_Start_IT(&htim17,TIM_CHANNEL_1);
  /* USER CODE END TIM17_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 64-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 65535;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 7;
  if (HAL_TIM_IC_ConfigChannel(&htim16, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */
  HAL_TIM_IC_Start_IT(&htim16,TIM_CHANNEL_1);
  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static int8_t N20_02_HardInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

    /*Configure GPIO pin : PA2 */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PB4 PB5 */
    GPIO_InitStruct.Pin = GPIO_PIN_4 |GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // input PD1 -- QUAD_A
    MX_TIM17_Init();
    // input PD0 -- QUAD_B
    MX_TIM16_Init();

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

    return 0;
}

static int32_t N20_2_Enable(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
    ////printm(KERN_DEBUG "signal_cnt hal_angle ratio_angle strap target \r\n");
}

static int32_t N20_2_Disable(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}

int32_t N20_2_Forward(uint8_t distence)
{
    m_motor_direction = MOTOR_DERECTION_FORWARD;
    m_target_distence    += (double)distence;
    //m_target_distence    += (double)0.1;
    if(m_target_distence > m_strap_distence)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    }
}

int32_t N20_2_Backward(uint8_t distence)
{
    m_motor_direction = MOTOR_DERECTION_BACKWARD;
    m_target_distence    -= distence;
    if(m_target_distence < m_strap_distence)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
    }
}

int32_t N20_2_Brake(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
}

int32_t N20_2_Release(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
}

static void N20_2_Debug_Process(void)
{
    double delta_distence = m_strap_distence - m_target_distence;

    if(0 == print_times)
    {
        printm(KERN_DEBUG "signal_cnt signal2_cnt hal_angle ratio_angle strap target delta\r\n");
        print_times++;
    }
    else
    {
        printm(KERN_DEBUG "%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",m_signal_cnt,m_signal2_cnt,(uint32_t)(m_hal_angle*10),
            (uint32_t)(m_reduction_ratio_angle*10),(uint32_t)(m_strap_distence*100),(uint32_t)(m_target_distence*100),
            (uint32_t)(delta_distence*100) );
    }
}

/**
  * @brief  Input Capture callback in non-blocking mode
  * @param  htim TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    double worm_gear_angle  = 0.0;
    double gear2_angle      = 0.0;
    if(htim == &htim17)
    {
        if(g_last_signal_index != 7)
        {
            g_last_signal_index = 7;
            if(MOTOR_DERECTION_FORWARD == m_motor_direction)
            {
                m_signal_cnt++;
            } else if(MOTOR_DERECTION_BACKWARD == m_motor_direction)
            {
                m_signal_cnt--;
            }
            m_hal_angle                 = (double)( (360.0 / 7.0) * m_signal_cnt );
            // 根据N20电机的传动比定，(38/10)*(35/13)*(37/10)*(27/20) = 1328670/26000=51.1026923
            //// m_reduction_ratio_angle     = (double)(m_hal_angle / 51.10270);// 1030;
            // 根据N20电机的传动比定，(39/10)*(36/13)*(38/10)*(35/10)*(25/12) = 46683000/156000=299.25
            m_reduction_ratio_angle     = (double)(m_hal_angle / 299.25);
            // 蜗杆为2头 蜗轮为50齿轮 传动比为25:1
            worm_gear_angle = (double)(m_reduction_ratio_angle / 25.0);
            // 蜗轮（齿轮1）为50齿轮 齿轮2为40齿 传动比为4/5 = 0.8
            gear2_angle     = (double)(worm_gear_angle / 0.8);
            // 物料拨盘一圈（360度）28个齿，对应料带位移28*4mm。gear2_angle角度对应的位移为
            m_strap_distence            = (double)(gear2_angle * 28.0 * 4.0 / 360.0);
            if( (MOTOR_DERECTION_FORWARD == m_motor_direction) && (m_strap_distence >= m_target_distence) )
            {
                N20_2_Brake();
            }
            else if( (MOTOR_DERECTION_BACKWARD == m_motor_direction) && (m_strap_distence <= m_target_distence) )
            {
                N20_2_Brake();
            }
        }
    }
    else if(htim == &htim16)
    {
        if(g_last_signal_index != 6)
        {
            g_last_signal_index = 6;
            if(MOTOR_DERECTION_FORWARD == m_motor_direction)
            {
                m_signal2_cnt++;
            } else if(MOTOR_DERECTION_BACKWARD == m_motor_direction)
            {
                m_signal2_cnt--;
            }
        }
    }
}

int8_t n20_Motor_init_02(void)
{
    RegisterMotorOpr(&g_tN20Motorpr_02);

    m_signal_cnt = 0;

    return 0;

}

