/**
  ******************************************************************************
  * @file    Relays/Relays.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Relays configuration
  ******************************************************************************
  */

/****************** Pin config *******************/
/*************************************************/
/********** LED1 LED2 LED3 LED4 LED5 *************/
/********** PA0  PF10 PF9  PF8  PF7 **************/
/*************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Leds.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

HAL_StatusTypeDef Leds_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	LED1_GPIO_CLK_ENABLE();
	LED2_GPIO_CLK_ENABLE();
	LED3_GPIO_CLK_ENABLE();
	LED4_GPIO_CLK_ENABLE();
	LED5_GPIO_CLK_ENABLE();	

	GPIO_InitStructure.Pin = LED1_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LED2_PIN;
	HAL_GPIO_Init(LED2_GPIO_PORT,&GPIO_InitStructure);	

	GPIO_InitStructure.Pin = LED3_PIN;
	HAL_GPIO_Init(LED3_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LED4_PIN;
	HAL_GPIO_Init(LED4_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LED5_PIN;
	HAL_GPIO_Init(LED5_GPIO_PORT,&GPIO_InitStructure);
		
	
	return HAL_OK;
}

HAL_StatusTypeDef Leds_DeInit(void)
{
	HAL_GPIO_DeInit(LED1_GPIO_PORT,LED1_PIN);
	HAL_GPIO_DeInit(LED2_GPIO_PORT,LED2_PIN);
	HAL_GPIO_DeInit(LED3_GPIO_PORT,LED3_PIN);
	HAL_GPIO_DeInit(LED4_GPIO_PORT,LED4_PIN);
	HAL_GPIO_DeInit(LED5_GPIO_PORT,LED5_PIN);
	
	return HAL_OK;
}

void Led(LEDS a_Led,uint8_t a_State)
{
	switch(a_Led)
	{
		case LED1:
		{
			HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case LED2:
		{
			HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case LED3:
		{
			HAL_GPIO_WritePin(LED3_GPIO_PORT,LED3_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case LED4:
		{
			HAL_GPIO_WritePin(LED4_GPIO_PORT,LED4_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case LED5:
		{
			HAL_GPIO_WritePin(LED5_GPIO_PORT,LED5_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
	}
}

void Leds_Binary(uint8_t a_u8_Value)
{
	Led(LED1,a_u8_Value & 0x80);
	Led(LED2,a_u8_Value & 0x40);
	Led(LED3,a_u8_Value & 0x20);
	Led(LED4,a_u8_Value & 0x10);
	Led(LED5,a_u8_Value & 0x08);
}

void Leds_All_On()
{
	Leds_Binary(1);
}

void Leds_All_Off()
{
	Leds_Binary(0);
}

/**** END OF FILE ****/
