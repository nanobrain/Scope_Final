/**
  ******************************************************************************
  * @file    Leds/Leds.h
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for leds configuration file
  ******************************************************************************
  */
  
#ifndef __LEDS_CONFIG_H
#define __LEDS_CONFIG_H

typedef enum LEDS{
	LEDRED1,
	LEDRED2,
	LEDRED3,
	LEDBLUE,
	LEDGREEN,
}LEDS;

/* Definition for Pins */
#define LEDRED1_PIN									GPIO_PIN_0
#define LEDRED2_PIN									GPIO_PIN_10
#define LEDRED3_PIN									GPIO_PIN_9
#define LEDBLUE_PIN									GPIO_PIN_8
#define LEDGREEN_PIN								GPIO_PIN_7

#define LEDRED1_GPIO_PORT						GPIOA
#define LEDRED2_GPIO_PORT						GPIOF
#define LEDRED3_GPIO_PORT						GPIOF
#define LEDBLUE_GPIO_PORT						GPIOF
#define LEDGREEN_GPIO_PORT					GPIOF

#define LEDRED1_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOA_CLK_ENABLE();
#define LEDRED2_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOF_CLK_ENABLE();
#define LEDRED3_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOF_CLK_ENABLE();
#define LEDBLUE_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOF_CLK_ENABLE();
#define LEDGREEN_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOF_CLK_ENABLE();

HAL_StatusTypeDef Leds_Init(void);
HAL_StatusTypeDef Leds_DeInit(void);
void Led(LEDS a_Led,uint8_t a_State);
void Leds_Binary(uint8_t a_u8_Value);
void Leds_All_On(void);
void Leds_All_Off(void);

#endif /* __LEDS_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
