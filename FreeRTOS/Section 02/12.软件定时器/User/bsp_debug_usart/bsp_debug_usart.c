/*
 * @File Name: 
 * @Description: 
 * @Autor: rXQiang
 * @Date: 2021-09-17 21:17:12
 * @LastEditors: rXQiang
 * @LastEditTime: 2021-09-29 21:08:59
 */
#include "bsp_debug_usart.h"

UART_HandleTypeDef UartHandle;

void DEBUG_USART_Config(void)
{
  UartHandle.Instance = DEBUG_USART;

  UartHandle.Init.BaudRate = DEBUG_USART_BAUDRATE;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;

  HAL_UART_Init(&UartHandle);

  /*ʹ�ܴ��ڽ��ն� */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  DEBUG_USART_CLK_ENABLE();

  DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();

  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(DEBUG_USART_IRQ, 0, 1);
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ);
}

void Usart_SendString(uint8_t *str)
{
  unsigned int k = 0;
  do
  {
    HAL_UART_Transmit(&UartHandle, (uint8_t *)(str + k), 1, 1000);
    k++;
  } while (*(str + k) != '\0');
}

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);

  return (ch);
}

int fgetc(FILE *f)
{
  int ch;
  HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);
  return (ch);
}
