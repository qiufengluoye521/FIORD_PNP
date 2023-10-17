#include "RS485_01.h"
#include "Commit_Manager.h"
#include "Debug_Print.h"

#define UART_RX_BUFFER_LEN            (1)
#define UART_RX_ARRAY_LEN             (128)

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

static int8_t RS485_HardInit(void);
static void RS485_TransferByte(uint8_t data);
static void RS485_TransferBuffer(uint8_t* buffer, uint16_t Size);
static void RS485_Debug_Process(void);
static uint8_t RS485_get_message_ready_flag(uint8_t *buff,uint16_t *size);
static void RS485_set_message_ready_flag(uint8_t flag);

static int8_t m_uart_rx_buff[UART_RX_BUFFER_LEN]   = {};
static int8_t m_uart_rx_buff2[UART_RX_BUFFER_LEN]  = {};
static int8_t m_uart_rx_array[UART_RX_ARRAY_LEN]   = {};
static int8_t m_uart_rx_array_index                = 0;
static int8_t m_message_ready_flag                 = 0;

static T_CommitOpr g_tRS485opr_01 = {
    .name                           = "RS485_01",
    .isCanUse                       = 1,
    .Commit_Init                    = RS485_HardInit,
    .Commit_TransferByte            = RS485_TransferByte,
    .Commit_TransferBuffer          = RS485_TransferBuffer,
    .get_message_ready_flag         = RS485_get_message_ready_flag,
    .set_message_ready_flag         = RS485_set_message_ready_flag,
    .Commit_Debug                   = RS485_Debug_Process,
};

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart1, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_8_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_8_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)m_uart_rx_buff,UART_RX_BUFFER_LEN);
  /* USER CODE END USART1_Init 2 */

}


/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /*Configure GPIO pins : PA11 PA12 */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
}


/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t i = 0;
    if(huart->Instance == USART1)
    {
        //IT  RX-CALLBACK
        ////memcpy(m_uart_rx_buff2,m_uart_rx_buff,UART_RX_BUFFER_LEN);
        if( (m_uart_rx_array_index >= 0) && (m_uart_rx_array_index < UART_RX_ARRAY_LEN) )
        {
            m_uart_rx_array[m_uart_rx_array_index] = m_uart_rx_buff[0];
            m_uart_rx_array_index++;
        }
        else
        {
            m_uart_rx_array_index = UART_RX_ARRAY_LEN;
            printm(KERN_DEBUG "buffer full, please input cmd len less then %d end with '\n' ",UART_RX_ARRAY_LEN);
        }
        if('\n' == m_uart_rx_buff[0])
        {
            m_message_ready_flag = 1;
        }
        // Enable Uart it
        HAL_UART_Receive_IT(&huart1, (uint8_t *)m_uart_rx_buff,UART_RX_BUFFER_LEN);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    printm(KERN_WARNING "HAL_UART_ErrorCallback\r\n");
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static int8_t RS485_HardInit(void)
{
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_DMA_Init();
    return 0;
}

static void RS485_TransferByte(uint8_t data)
{
    HAL_UART_Transmit(&huart1, &data,1, HAL_MAX_DELAY);
}

static void RS485_TransferBuffer(uint8_t* buffer, uint16_t Size)
{
    HAL_UART_Transmit(&huart1, buffer,Size, HAL_MAX_DELAY);
}

static void RS485_Debug_Process(void)
{
    ;
}

static uint8_t RS485_get_message_ready_flag(uint8_t *buff,uint16_t *size)
{
    memcpy(buff,m_uart_rx_array,m_uart_rx_array_index);
    *size   = m_uart_rx_array_index;
    if(m_message_ready_flag)
    {
        m_uart_rx_array_index = 0;
    }
    return m_message_ready_flag;
}

static void RS485_set_message_ready_flag(uint8_t flag)
{
    m_message_ready_flag = flag;
}

int8_t Commit_Rs485_init_01(void)
{
    RegisterCommitOpr(&g_tRS485opr_01);

    return 0;

}

