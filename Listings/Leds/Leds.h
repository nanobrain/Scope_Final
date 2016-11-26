/**
  ******************************************************************************
  * @file    Leds/Leds.h
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for leds configuration file
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LEDS_CONFIG_H
#define __LEDS_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum LEDS{
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
}LEDS;

/* Exported constants --------------------------------------------------------*/
/* Definition for Pins */
#define LED1_PIN									GPIO_PIN_0
#define LED2_PIN									GPIO_PIN_10
#define LED3_PIN									GPIO_PIN_9
#define LED4_PIN									GPIO_PIN_8
#define LED5_PIN									GPIO_PIN_7

#define LED1_GPIO_PORT						GPIOA
#define LED2_GPIO_PORT						GPIOF
#define LED3_GPIO_PORT						GPIOF
#define LED4_GPIO_PORT						GPIOF
#define LED5_GPIO_PORT						GPIOF

#define LED1_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOA_CLK_ENABLE();
#define LED2_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOF_CLK_ENABLE();
#define LED3_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOF_CLK_ENABLE();
#define LED4_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOF_CLK_ENABLE();
#define LED5_GPIO_CLK_ENABLE();		__HAL_RCC_GPIOF_CLK_ENABLE();

/* Exported macro ------------------------------------------------------------*/
/* Functions ------------------------------------------------------- */
HAL_StatusTypeDef Leds_Init(void);
HAL_StatusTypeDef Leds_DeInit(void);
void Led(LEDS a_Led,uint8_t a_State);
void Leds_Binary(uint8_t a_u8_Value);
void Leds_All_On(void);
void Leds_All_Off(void);

#endif /* __LEDS_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
