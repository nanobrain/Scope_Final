/**
  ******************************************************************************
  * @file    VGA/VGA_Config.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Variable gain ampliier configuration file
  ******************************************************************************
  */

/****** Pin config ******/
/************************/
/********** CS **********/
/********* PB_4 *********/
/************************/

#include "main.h"
#include "VGA_Config.h"
#include "Error_Handler.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

static VGATXBUFFER _CurrentState;
VGA_GAIN _CurrentGain;
extern SPI_HandleTypeDef g_hSpi;

HAL_StatusTypeDef VGA_Init(void)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		VGA_GPIO_CLK_ENABLE();
		GPIO_InitStructure.Pin = VGA_SPIx_CS_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(VGA_SPIx_CS_GPIO_PORT,&GPIO_InitStructure);
		HAL_GPIO_WritePin(VGA_SPIx_CS_GPIO_PORT,VGA_SPIx_CS_PIN,GPIO_PIN_SET);
		
		memset((void*)_CurrentState.buffer,0,sizeof(_CurrentState.buffer));	// Clear. NECESSARY!!
		_CurrentState.DataFields.power 	= 0x1;		// Aux Hi-Z
		_CurrentState.DataFields.filter = 0x1;		// Lowest filter (20 MHz)
		_CurrentState.DataFields.ladder = 0x9;		// Lowest gain
		_CurrentState.DataFields.preAmp = 0x0;		// Lowest gain
		_CurrentGain = DB0_84;
		
		errCode = VGA_Transmit();
	}
	else
	{
		errCode=HAL_ERROR;
	}
	
	return errCode;
}

HAL_StatusTypeDef VGA_DeInit(void)
{
	HAL_GPIO_DeInit(VGA_SPIx_CS_GPIO_PORT,VGA_SPIx_CS_PIN);
	return HAL_OK;
}

int VGA_Step_Gain(STEP_DIRECTION a_Direction)
{
	VGA_GAIN curGain;
	curGain = VGA_Get_Gain();
	switch(a_Direction)
	{
		case STEP_UP:
		{
			if( curGain == DB38_84 )
				return -1;
			else
				VGA_Set_Gain((VGA_GAIN)(curGain + 1));
		}break;
		case STEP_DOWN:
		{
			if( curGain == DB0_84 )
				return -1;
			else
				VGA_Set_Gain((VGA_GAIN)(curGain - 1));
		}break;
		default:
			Error_Handler(ERROR_TRANSMIT);
	}
	return 0;
}

void VGA_Set_Gain(VGA_GAIN a_gain)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	
	// G = PreAmp + Constant - Ladder
	// G = ( HG OR LG ) + 8.86 - 2n
	// G = 10 + 8.86 - 2n = 18.86 - 2n
	// G = 30 + 8.86 - 2n = 38.86 - 2n
	// EXAMPLE:
	// G = 14.86
	// PreAmp = LG = 10
	// Constant = 8.86
	// Ladder = - 2*2 = - 4
	// G = 10 + 8.86 - 4	
	
	// Calculate enum value
	if( a_gain > DB18_84 )
	{
		_CurrentState.DataFields.preAmp = 0x1;
		_CurrentState.DataFields.ladder = (20UL - a_gain)&0xF;
	}
	else if( a_gain == DB18_84 )
	{
		_CurrentState.DataFields.preAmp = 0x1;
		_CurrentState.DataFields.ladder = (10UL - a_gain)&0xF;
	}
	else if( a_gain < DB18_84 )
	{
		_CurrentState.DataFields.preAmp = 0x0;
		_CurrentState.DataFields.ladder = (10UL - a_gain)&0xF;
	}
	else
		Error_Handler(ERROR_TRANSMIT);
	
	errCode = VGA_Transmit();
	_CurrentGain = a_gain;
	
	if( errCode != HAL_OK )
		Error_Handler(ERROR_TRANSMIT);
}

VGA_GAIN VGA_Get_Gain(void)
{
	/*
	VGA_GAIN curGain;
	
	// Calculate enum value
	if( _CurrentState.DataFields.preAmp == 1 )
	{
		if( (unsigned long)_CurrentState.DataFields.ladder == 0)
			curGain = DB18_84;
		else
			curGain =(VGA_GAIN)(20UL - (unsigned long)_CurrentState.DataFields.ladder);
	}
	else if( _CurrentState.DataFields.preAmp == 0 )
	{
		curGain = (VGA_GAIN)(10UL - (unsigned long)_CurrentState.DataFields.ladder);
	}
	else
		Error_Handler(ERROR_TRANSMIT);
	
	return curGain;
	*/
	return _CurrentGain;
}

static HAL_StatusTypeDef VGA_Transmit(void)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	uint8_t Data[4];
	
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		
		SPI_NSS_DeInit();
		VGA_CS_Write(FALSE);
		
		//Set command to Write and reorganize data
		Data[0] = _CurrentState.buffer[1];
		Data[1] = 0; // WRITE command
		Data[2] = 0; // Just padding
		Data[3] = _CurrentState.buffer[0];
		
		if( SPI_DMA )
		{
			errCode = HAL_SPI_Transmit_DMA(&g_hSpi,Data,1);
			errCode = HAL_SPI_Transmit_DMA(&g_hSpi,Data+2,1);
		}
		else
		{
			errCode = HAL_SPI_Transmit_IT(&g_hSpi,Data,1);
			errCode = HAL_SPI_Transmit_IT(&g_hSpi,Data+2,1);
		}
		
		while(HAL_SPI_GetState(&g_hSpi) == HAL_SPI_STATE_BUSY_TX ) {}
		
		VGA_CS_Write(TRUE);
		SPI_NSS_Init();
	}
	else
	{
		errCode = HAL_ERROR;
	}
	return errCode;
}

static void VGA_CS_Write(LOGIC a_SetReset)
{
	HAL_GPIO_WritePin(VGA_SPIx_CS_GPIO_PORT,VGA_SPIx_CS_PIN,(a_SetReset ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

/**** END OF FILE ****/
