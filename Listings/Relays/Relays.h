/**
  ******************************************************************************
  * @file    Relays/Relays.h
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for relays configuration file
  ******************************************************************************
  */
  
#ifndef __RELAY_CONFIG_H
#define __RELAY_CONFIG_H

typedef enum RELAY{
	REL_GND,
	REL_ATT,
	REL_ACDC
}RELAY;

typedef enum ACDC{
	AC,
	DC
}ACDC;

typedef enum INPUT_GND{
	INPUT,
	GND
}INPUT_GND;

typedef enum ATTENUATION{
	DB0,
	DB20	
}ATTENUATION;

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

HAL_StatusTypeDef Relays_Init(void);
HAL_StatusTypeDef Relays_DeInit(void);
static void _Relay(RELAY a_Relay,LOGIC a_State);
void Relay_ACDC(ACDC a_Value);
void Relay_Input(INPUT_GND a_Value);
void Relay_Attenuator(ATTENUATION a_Value);
void Relays_Default(void);

#endif /* __RELAY_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
