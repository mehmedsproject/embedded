/*
 * arduinoLibraryForStm32f1xx.c
 *
 *  Created on: Apr 8, 2024
 *      Author: Muhammed
 */

/* Includes ----------------------------------------------------------*/
#include "arduinoLibraryForStm32f1xx.h"

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

	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(port, &GPIO_InitStruct);
}
