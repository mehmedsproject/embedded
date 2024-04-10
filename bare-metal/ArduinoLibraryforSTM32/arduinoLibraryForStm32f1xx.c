/*
 * arduinoLibraryForStm32f1xx.c
 *
 *  Created on: Apr 8, 2024
 *      Author: Muhammed
 */

/* Includes ----------------------------------------------------------*/
#include "arduinoLibraryForStm32f1xx.h"
#include "string.h"
#include "stdlib.h"
#include "main.h"
/* Private Global Variables ------------------------------------------*/


/* TypeDefs --------------------------------------------------*/

/* Functions  -----------------------------------------------*/


// digitalWrite Fonksiyonu
void digitalWrite(uint16_t GPIOpin, GPIO_PinState state) {

    HAL_GPIO_WritePin(port, pin, state);
}

// GPIO yapılandırma fonksiyonu

//void pinMode(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t mode) {
void pinMode(uint16_t GPIOpin, uint32_t mode) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	port = NULL;
	pin = 0;


#ifdef STM32F103BLUEPILLBOARD
	switch(GPIOpin){
    case B11: stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_11; break;
    case B10: stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_10; break;
    case B1:  stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_1;  break;
    case B0:  stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_0;  break;
    case A7:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_7;  break;
    case A6:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_6;  break;
    case A5:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_5;  break;
    case A4:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_4;  break;
    case A3:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_3;  break;
    case A2:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_2;  break;
    case A1:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_1;  break;
    case A0:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_0;  break;
    case C15: stm32f103BluePillBoard.port = GPIOC; stm32f103BluePillBoard.pin = GPIO_PIN_15; break;
    case C14: stm32f103BluePillBoard.port = GPIOC; stm32f103BluePillBoard.pin = GPIO_PIN_14; break;
    case C13: stm32f103BluePillBoard.port = GPIOC; stm32f103BluePillBoard.pin = GPIO_PIN_13; break;
    case B12: stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_12; break;
    case B13: stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_13; break;
    case B14: stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_14; break;
    case B15: stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_15; break;
    case A8:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_8;  break;
    case A9:  stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_9;  break;
    case A10: stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_10; break;
    case A11: stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_11; break;
    case A12: stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_12; break;
    case A15: stm32f103BluePillBoard.port = GPIOA; stm32f103BluePillBoard.pin = GPIO_PIN_15; break;
    case B3:  stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_3;  break;
    case B5:  stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_5;  break;
    case B6:  stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_6;  break;
    case B7:  stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_7;  break;
    case B8:  stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_8;  break;
    case B9:  stm32f103BluePillBoard.port = GPIOB; stm32f103BluePillBoard.pin = GPIO_PIN_9;  break;
	default:
		stm32f103BluePillBoard.port = NULL;
		stm32f103BluePillBoard.pin = 0;
	}

	port = stm32f103BluePillBoard.port;
	pin = stm32f103BluePillBoard.pin;
#endif

	HAL_GPIO_WritePin(port, pin, mode);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void Serial_begin(uint32_t baudrate, uint8_t usartNumber) {
	// UART configuration structure
	 switch(usartNumber){
		 case 1:
			 usartCommunication.huartNum = &huart1;
			 huart1.Instance = USART1;
			 huart1.Init.WordLength = UART_WORDLENGTH_8B; // Set word length to 8 bits
			 huart1.Init.StopBits = UART_STOPBITS_1; // Set number of stop bits to 1
			 huart1.Init.Parity = UART_PARITY_NONE; // Disable parity
			 huart1.Init.Mode = UART_MODE_TX_RX; // Enable both transmission and reception
			 huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE; // Disable hardware flow control
			 huart1.Init.OverSampling = UART_OVERSAMPLING_16; // Set oversampling to 16

			 // Initialize the UART peripheral
			 if (HAL_UART_Init(&huart1) != HAL_OK) {
			    // UART initialization error
				 Error_Handler();
			 }
			 break;
		 case 2:
			 usartCommunication.huartNum = &huart2;
			 huart2.Instance = USART2;
			 huart2.Init.WordLength = UART_WORDLENGTH_8B; // Set word length to 8 bits
			 huart2.Init.StopBits = UART_STOPBITS_1; // Set number of stop bits to 1
			 huart2.Init.Parity = UART_PARITY_NONE; // Disable parity
			 huart2.Init.Mode = UART_MODE_TX_RX; // Enable both transmission and reception
			 huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // Disable hardware flow control
			 huart2.Init.OverSampling = UART_OVERSAMPLING_16; // Set oversampling to 16
			 // Initialize the UART peripheral
			 if (HAL_UART_Init(&huart2) != HAL_OK) {
			    // UART initialization error
				 Error_Handler();
			 }
			 break;
		 case 3:
			 usartCommunication.huartNum = &huart3;
			 huart3.Instance = USART3;
			 huart3.Init.WordLength = UART_WORDLENGTH_8B; // Set word length to 8 bits
			 huart3.Init.StopBits = UART_STOPBITS_1; // Set number of stop bits to 1
			 huart3.Init.Parity = UART_PARITY_NONE; // Disable parity
			 huart3.Init.Mode = UART_MODE_TX_RX; // Enable both transmission and reception
			 huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE; // Disable hardware flow control
			 huart3.Init.OverSampling = UART_OVERSAMPLING_16; // Set oversampling to 16
			 // Initialize the UART peripheral
			 if (HAL_UART_Init(&huart3) != HAL_OK) {
			    // UART initialization error
				 Error_Handler();
			 }
			 break;
		 default:
			 usartCommunication.huartNum = &huart1;
			 huart1.Instance = USART1;
			 huart1.Init.WordLength = UART_WORDLENGTH_8B; // Set word length to 8 bits
			 huart1.Init.StopBits = UART_STOPBITS_1; // Set number of stop bits to 1
			 huart1.Init.Parity = UART_PARITY_NONE; // Disable parity
			 huart1.Init.Mode = UART_MODE_TX_RX; // Enable both transmission and reception
			 huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE; // Disable hardware flow control
			 huart1.Init.OverSampling = UART_OVERSAMPLING_16; // Set oversampling to 16
			 // Initialize the UART peripheral
			 if (HAL_UART_Init(&huart1) != HAL_OK) {
			    // UART initialization error
				 Error_Handler();
			 }
	 }
}

// Function to send a string over USART
void serialPrint(const char *data,  uint8_t usartNumber) {

	UART_HandleTypeDef *huartNum;

	switch(usartNumber){
	case 1: huartNum = &huart1; break;
	case 2: huartNum = &huart2; break;
	case 3: huartNum = &huart3; break;
	default:
		huartNum = usartCommunication.huartNum;
	}


    // Call the function to send the string
    HAL_UART_Transmit(huartNum, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
}

// Function to send a string followed by a newline character over USART
void serialPrintln(const char *data, uint8_t usartNumber) {

	UART_HandleTypeDef *huartNum;

	switch(usartNumber){
	case 1: huartNum = &huart1; break;
	case 2: huartNum = &huart2; break;
	case 3: huartNum = &huart3; break;
	default:
		huartNum = usartCommunication.huartNum;
	}
    // Call the function to send the string
    HAL_UART_Transmit(huartNum, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
    // Send a newline character ('\n')
    char newline = '\n';
    HAL_UART_Transmit(huartNum, (uint8_t *)&newline, 1, HAL_MAX_DELAY);
}

int analogRead(int analogPin, int adcNumber)
{
	ADC_HandleTypeDef adcNum;

	volatile int adcValue;

	ADC_ChannelConfTypeDef sConfig = {0};
	if(adcNumber == 1)
	{
	    adcNum = hadc1;
		hadc1.Instance = ADC1;
		hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
		hadc1.Init.ContinuousConvMode = ENABLE;
		hadc1.Init.DiscontinuousConvMode = DISABLE;
		hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc1.Init.NbrOfConversion = 1;
		if (HAL_ADC_Init(&hadc1) != HAL_OK)
		{
			Error_Handler();
		}
	}
	else if(adcNumber == 2)
	{
		adcNum = hadc2;
		hadc2.Instance = ADC2;
		hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
		hadc2.Init.ContinuousConvMode = ENABLE;
		hadc2.Init.DiscontinuousConvMode = DISABLE;
		hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
		hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
		hadc2.Init.NbrOfConversion = 1;
		if (HAL_ADC_Init(&hadc2) != HAL_OK)
		{
			Error_Handler();
		}
	}


	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

	switch(analogPin){

    case B1:
    	sConfig.Channel = ADC_CHANNEL_9;
    	break;
    case B0:
    	sConfig.Channel = ADC_CHANNEL_8;
    	break;
    case A7:
    	sConfig.Channel = ADC_CHANNEL_7;
    	break;
    case A6:
    	sConfig.Channel = ADC_CHANNEL_6;
    	break;
    case A5:
    	sConfig.Channel = ADC_CHANNEL_5;
    	break;
    case A4:
    	sConfig.Channel = ADC_CHANNEL_4;
    	break;
    case A3:
    	sConfig.Channel = ADC_CHANNEL_3;
    	break;
    case A2:
    	sConfig.Channel = ADC_CHANNEL_2;
    	break;
    case A1:
    	sConfig.Channel = ADC_CHANNEL_1;
    	break;
    case A0:
    	sConfig.Channel = ADC_CHANNEL_0;
    	break;
    default:
    	sConfig.Channel = 0;
	}

	HAL_ADC_ConfigChannel(&adcNum, &sConfig);

    HAL_ADC_Start(&adcNum);
    HAL_ADC_PollForConversion(&adcNum, HAL_MAX_DELAY); // Wait for conversion
    adcValue = HAL_ADC_GetValue(&adcNum); // Get ADC
    HAL_ADC_Stop(&adcNum);

    return((int)adcValue);
}
