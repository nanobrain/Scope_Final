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
	
	LEDRED1_GPIO_CLK_ENABLE();
	LEDRED2_GPIO_CLK_ENABLE();
	LEDRED3_GPIO_CLK_ENABLE();
	LEDBLUE_GPIO_CLK_ENABLE();
	LEDGREEN_GPIO_CLK_ENABLE();	

	GPIO_InitStructure.Pin = LEDRED1_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(LEDRED1_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LEDRED2_PIN;
	HAL_GPIO_Init(LEDRED2_GPIO_PORT,&GPIO_InitStructure);	

	GPIO_InitStructure.Pin = LEDRED3_PIN;
	HAL_GPIO_Init(LEDRED3_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LEDBLUE_PIN;
	HAL_GPIO_Init(LEDBLUE_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = LEDGREEN_PIN;
	HAL_GPIO_Init(LEDGREEN_GPIO_PORT,&GPIO_InitStructure);
		
	
	return HAL_OK;
}

HAL_StatusTypeDef Leds_DeInit(void)
{
	HAL_GPIO_DeInit(LEDRED1_GPIO_PORT,LEDRED1_PIN);
	HAL_GPIO_DeInit(LEDRED2_GPIO_PORT,LEDRED2_PIN);
	HAL_GPIO_DeInit(LEDRED3_GPIO_PORT,LEDRED3_PIN);
	HAL_GPIO_DeInit(LEDBLUE_GPIO_PORT,LEDBLUE_PIN);
	HAL_GPIO_DeInit(LEDGREEN_GPIO_PORT,LEDGREEN_PIN);
	
	return HAL_OK;
}

void Led(LEDS a_Led,uint8_t a_State)
{
	switch(a_Led)
	{
		case LEDRED1:
		{
			HAL_GPIO_WritePin(LEDRED1_GPIO_PORT,LEDRED1_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case LEDRED2:
		{
			HAL_GPIO_WritePin(LEDRED2_GPIO_PORT,LEDRED2_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case LEDRED3:
		{
			HAL_GPIO_WritePin(LEDRED3_GPIO_PORT,LEDRED3_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case LEDBLUE:
		{
			HAL_GPIO_WritePin(LEDBLUE_GPIO_PORT,LEDBLUE_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case LEDGREEN:
		{
			HAL_GPIO_WritePin(LEDGREEN_GPIO_PORT,LEDGREEN_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
	}
}

void Leds_Binary(uint8_t a_u8_Value)
{
	a_u8_Value = a_u8_Value << 4;
	
	Led(LEDRED1,a_u8_Value & 0x80);
	Led(LEDRED2,a_u8_Value & 0x40);
	Led(LEDRED3,a_u8_Value & 0x20);
	Led(LEDBLUE,a_u8_Value & 0x10);
}

void Leds_All_On()
{
	Leds_Binary(1);
}

void Leds_All_Off()
{
	Leds_Binary(0);
}

/*
// Implement this in another thread
void Led_Blinking_Start(){}
void Led_Blinking_Stop(){}
*/

/**** END OF FILE ****/
