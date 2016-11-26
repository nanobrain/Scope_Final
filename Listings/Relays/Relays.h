/**
  ******************************************************************************
  * @file    Relays/Relays.h
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for relays configuration file
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RELAY_CONFIG_H
#define __RELAY_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum RELAY{
	REL_GND,
	REL_ATT,
	REL_ACDC
}RELAY;

/* Exported constants --------------------------------------------------------*/
/* Definition for Pins */
#define REL_GND_PIN											GPIO_PIN_7
#define REL_ATT_PIN											GPIO_PIN_0
#define REL_ACDC_PIN										GPIO_PIN_15
#define REL_GND_GPIO_PORT								GPIOG
#define REL_ATT_GPIO_PORT								GPIOI
#define REL_ACDC_GPIO_PORT							GPIOA
#define REL_GND_GPIO_CLK_ENABLE					__HAL_RCC_GPIOG_CLK_ENABLE
#define REL_ATT_GPIO_CLK_ENABLE					__HAL_RCC_GPIOI_CLK_ENABLE
#define REL_ACDC_GPIO_CLK_ENABLE				__HAL_RCC_GPIOA_CLK_ENABLE

/* Exported macro ------------------------------------------------------------*/
/* Functions ------------------------------------------------------- */
HAL_StatusTypeDef Relays_Init(void);
HAL_StatusTypeDef Relays_DeInit(void);
void Relay(RELAY a_Relay,LOGIC a_State);

#endif /* __RELAY_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
