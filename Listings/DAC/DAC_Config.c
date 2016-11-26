/**
  ******************************************************************************
  * @file    DAC/DAC_Config.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Dac configuration file
  ******************************************************************************
  */

/* TODO: Write structure to store current state:
	- isInitialized
	- OutputVoltage
	- PowerState
*/

/****** Pin config ******/
/************************/
/********* SYNC *********/
/********** PI2 *********/
/************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DAC_Config.h"
#include <cstdio>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t u32_txBuffer[3];
static uint8_t u8_CurrentValue;
extern SPI_HandleTypeDef g_hspi;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

HAL_StatusTypeDef DAC_Init(void)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if( HAL_SPI_GetState(&g_hspi) != HAL_SPI_STATE_RESET )
	{
		DAC_GPIO_CLK_ENABLE();
		GPIO_InitStructure.Pin = DAC_SPIx_SYNC_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(DAC_SPIx_SYNC_GPIO_PORT,&GPIO_InitStructure);
		HAL_GPIO_WritePin(DAC_SPIx_SYNC_GPIO_PORT,DAC_SPIx_SYNC_PIN,GPIO_PIN_SET);
		errCode=HAL_OK;
	}
	else
	{
		errCode=HAL_ERROR;
	}
	
	return errCode;
}

HAL_StatusTypeDef DAC_DeInit(void)
{
	DAC_GPIO_CLK_DISABLE();
	HAL_GPIO_DeInit(DAC_SPIx_SYNC_GPIO_PORT,DAC_SPIx_SYNC_PIN);
	return HAL_OK;
}

HAL_StatusTypeDef Set_DAC_Power_Mode(DAC_POWER_MODE a_PowerMode)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	switch(a_PowerMode)
	{
		case POWER_DOWN:
		{
			/**/
		}break;
		case POWER_FULL:
		{
			/**/
		}break;
		default:
		{
			/**/
		}
	}
	return errCode;
}

HAL_StatusTypeDef Set_DAC_Output(uint8_t a_8u_voltage)
{
	char TEMP[4];
	sprintf(TEMP,"%d",a_8u_voltage);
	return DAC_Transmit((uint8_t*)TEMP);
	return HAL_OK;
}

uint8_t Get_DAC_Output(void)
{
	return 0;
}

DAC_POWER_MODE Get_DAC_Power_Mode(void)
{
	return POWER_FULL;
}

static HAL_StatusTypeDef DAC_Transmit(uint8_t*  a_p8u_Data)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	if( HAL_SPI_GetState(&g_hspi) != HAL_SPI_STATE_RESET )
	{
		DAC_Sync_Write(FALSE);
		errCode = HAL_SPI_Transmit_DMA(&g_hspi,a_p8u_Data,BUFFERSIZE(a_p8u_Data));
		while(HAL_SPI_GetState(&g_hspi) == HAL_SPI_STATE_BUSY_TX ) {}
		DAC_Sync_Write(TRUE);
	}
	else
	{
		errCode = HAL_ERROR;
	}
	return errCode;
}

static void DAC_Sync_Write(LOGIC a_SetReset)
{
	if( a_SetReset == TRUE )
		HAL_GPIO_WritePin(DAC_SPIx_SYNC_GPIO_PORT,DAC_SPIx_SYNC_PIN,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DAC_SPIx_SYNC_GPIO_PORT,DAC_SPIx_SYNC_PIN,GPIO_PIN_RESET);
}

/**** END OF FILE ****/
