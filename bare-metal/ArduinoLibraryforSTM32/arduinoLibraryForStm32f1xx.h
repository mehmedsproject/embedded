/*
 * arduinoLibraryForStm32f1xx.h
 *
 *  Created on: Apr 8, 2024
 *      Author: Muhammed
 */

#ifndef ARDUINOLIBRARYFORSTM32_ARDUINOLIBRARYFORSTM32F1XX_H_
#define ARDUINOLIBRARYFORSTM32_ARDUINOLIBRARYFORSTM32F1XX_H_

/* Includes ----------------------------------------------------------*/
#include "stm32f1xx_hal.h"
/* Typedef -----------------------------------------------------------*/

/* Defines ------------------------------------------------------------*/


/*
 *  STM32F103C8T6 - BLUEPILL BOARD PIN DEFINITIONS
 */
//#define STM32F103BLUEPILLBOARD

// PIN STATE DEFINITIONS
#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET

#define INPUT 0
#define OUTPUT 1

#define PowerLED C13



/* TypeDefs -----------------------------------------------------------*/
typedef enum { false, true } bool;

typedef enum{
 B11, B10, B1, B0, A7, A6, A5, A4, A3, A2, A1, A0, C15, C14, C13,
 B12, B13, B14, B15, A8, A9, A10, A11, A12, A15, B3, B4, B5, B6, B7, B8, B9
}stm32f103BluePillPins;

typedef struct{
	GPIO_TypeDef *port;
	uint16_t pin;
}gpio;

/* Macros -------------------------------------------------------------*/

/* Private Variables ---------------------------------------------------------*/
GPIO_TypeDef *port;
uint16_t pin;

gpio stm32f103BluePillBoard;
/* Function prototypes -----------------------------------------------*/

void pinMode(uint16_t GPIOpin, uint32_t mode);
void digitalWrite(uint16_t GPIOpin, GPIO_PinState state);

#endif /* ARDUINOLIBRARYFORSTM32_ARDUINOLIBRARYFORSTM32F1XX_H_ */
