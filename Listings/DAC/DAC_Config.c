/**
  ******************************************************************************
  * @file    DAC/DAC_Config.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Dac configuration file
  ******************************************************************************
  */

/****** Pin config ******/
/************************/
/********** CS **********/
/********** PI2 *********/
/************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "DAC_Config.h"
#include "Error_Handler.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DACTXBUFFER _CurrentState;
int16_t _CurrentOutVoltage;
extern SPI_HandleTypeDef g_hSpi;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

HAL_StatusTypeDef DAC_Init(void)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		DAC_GPIO_CLK_ENABLE();
		GPIO_InitStructure.Pin = DAC_SPIx_CS_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(DAC_SPIx_CS_GPIO_PORT,&GPIO_InitStructure);
		HAL_GPIO_WritePin(DAC_SPIx_CS_GPIO_PORT,DAC_SPIx_CS_PIN,GPIO_PIN_SET);
		
		memset((void*)&_CurrentState.buffer,0,sizeof(_CurrentState.buffer));	// Clear. NECESSARY!!
		_CurrentState.DataFields.power = 0x0;
		_CurrentState.DataFields.value = 0x0000;
		errCode = DAC_Transmit();
	}
	else
	{
		errCode=HAL_ERROR;
	}
	
	return errCode;
}

HAL_StatusTypeDef DAC_DeInit(void)
{
	HAL_GPIO_DeInit(DAC_SPIx_CS_GPIO_PORT,DAC_SPIx_CS_PIN);
	return HAL_OK;
}

HAL_StatusTypeDef DAC_Set_Power_Mode(DAC_POWER_MODE a_PowerMode)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	switch(a_PowerMode)
	{
		case POWER_DOWN:
		{
		_CurrentState.DataFields.power = 0x3;
		}break;
		case POWER_FULL:
		{
		_CurrentState.DataFields.power = 0x0;
		}break;
		default:
		{
			/**/
		}
	}
	errCode = DAC_Transmit();
	return errCode;
}

HAL_StatusTypeDef DAC_Set_Voltage(uint8_t a_8u_voltage)
{
	//_CurrentOutValue = a_8u_voltage;
	
	return HAL_ERROR;
}

HAL_StatusTypeDef DAC_Set_Value(uint16_t a_16u_value)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	_CurrentState.DataFields.value = a_16u_value;
	errCode = DAC_Transmit();
	return errCode;
}

uint8_t Get_DAC_Voltage(void)
{
	return _CurrentOutVoltage;
}

uint16_t Get_DAC_Value(void)
{
	return _CurrentState.DataFields.value;
}

DAC_POWER_MODE Get_DAC_Power_Mode(void)
{
	return POWER_FULL;
}

static HAL_StatusTypeDef DAC_Transmit()
{
	HAL_StatusTypeDef errCode=HAL_OK;
	uint8_t Data[4];
	
	// Reorganize data
	Data[0] = _CurrentState.buffer[2];
	Data[1] = _CurrentState.buffer[0];
	Data[2] = 0x00;
	Data[3] = _CurrentState.buffer[1];
	
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		SPI_NSS_DeInit();
		DAC_CS_Write(FALSE);
		if( SPI_DMA )
		{
			errCode = HAL_SPI_Transmit_DMA(&g_hSpi,Data,2);
		}
		else
		{
			errCode = HAL_SPI_Transmit_IT(&g_hSpi,Data,2);
		}
		while(HAL_SPI_GetState(&g_hSpi) == HAL_SPI_STATE_BUSY_TX ) {}
		DAC_CS_Write(TRUE);
		SPI_NSS_Init();
	}
	else
	{
		errCode = HAL_ERROR;
	}
	return errCode;
}

static void DAC_CS_Write(LOGIC a_State)
{
	HAL_GPIO_WritePin(DAC_SPIx_CS_GPIO_PORT,DAC_SPIx_CS_PIN,(a_State ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

/**** END OF FILE ****/
