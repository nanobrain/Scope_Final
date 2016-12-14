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
/********** REL_GND  REL_ATT  REL_ACDC	**********/
/**********   PG7	     PI0      PA15 *************/
/*************************************************/

#include "main.h"
#include "Relays.h"

HAL_StatusTypeDef Relays_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	REL_GND_GPIO_CLK_ENABLE();
	REL_ATT_GPIO_CLK_ENABLE();
	REL_ACDC_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Pin = REL_GND_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(REL_GND_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = REL_ATT_PIN;
	HAL_GPIO_Init(REL_ATT_GPIO_PORT,&GPIO_InitStructure);	

	GPIO_InitStructure.Pin = REL_ACDC_PIN;
	HAL_GPIO_Init(REL_ACDC_GPIO_PORT,&GPIO_InitStructure);
	
	return HAL_OK;
}

HAL_StatusTypeDef Relays_DeInit(void)
{
	HAL_GPIO_DeInit(REL_GND_GPIO_PORT,REL_GND_PIN);
	HAL_GPIO_DeInit(REL_ATT_GPIO_PORT,REL_ATT_PIN);
	HAL_GPIO_DeInit(REL_ACDC_GPIO_PORT,REL_ACDC_PIN);
	return HAL_OK;
}

static void _Relay(RELAY a_Relay,LOGIC a_State)
{
	switch(a_Relay)
	{
		case REL_GND:
		{
			HAL_GPIO_WritePin(REL_GND_GPIO_PORT,REL_GND_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case REL_ATT:
		{
			HAL_GPIO_WritePin(REL_ATT_GPIO_PORT,REL_ATT_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
		case REL_ACDC:
		{
			HAL_GPIO_WritePin(REL_ACDC_GPIO_PORT,REL_ACDC_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
		}break;
	}
}

void Relay_ACDC(ACDC a_Value)
{
	switch(a_Value)
	{
		case AC:
		{
			_Relay(REL_ACDC,TRUE);
		}break;
		case DC:
		{
			_Relay(REL_ACDC,FALSE);
		}break;
		default:
		{}break;
	}
}

void Relay_Input(INPUT_GND a_Value)
{
	switch(a_Value)
	{
		case INPUT:
		{
			_Relay(REL_GND,TRUE);
		}break;
		case GND:
		{
			_Relay(REL_GND,FALSE);
		}break;
		default:
		{}break;
	}
}

void Relay_Attenuator(ATTENUATION a_Value)
{
	switch(a_Value)
	{
		case DB0:
		{
			_Relay(REL_ATT,FALSE);
		}break;
		case DB20:
		{
			_Relay(REL_ATT,TRUE);
		}break;
		default:
		{}break;
	}
}

void Relays_Default()
{
	Relay_Input(GND);
	Relay_Attenuator(DB20);
	Relay_ACDC(DC);
}

/**** END OF FILE ****/
