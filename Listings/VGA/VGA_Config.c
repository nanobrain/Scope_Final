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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "VGA_Config.h"
#include <cstdio>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t u8_txBuffer[3];
static uint8_t u8_CurrentGain=0;
extern SPI_HandleTypeDef g_hSpi;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

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
		errCode=HAL_OK;
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

HAL_StatusTypeDef Set_VGA_Power_Mode(VGA_POWER_MODE a_PowerMode)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	switch(a_PowerMode)
	{
		case POWER_FULL:
		{
			/**/
		}break;
		case AUX_HIGHZ:
		{
			/**/
		}break;
		case HIGHZ:
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

HAL_StatusTypeDef Set_VGA_Gain(uint8_t a_8u_gain)
{
	// TODO: Implement
	u8_CurrentGain = a_8u_gain;
	return HAL_ERROR;
}

uint8_t Get_VGA_Gain(void)
{
	return u8_CurrentGain;
}

VGA_POWER_MODE Get_VGA_Power_Mode(void)
{
	return POWER_FULL;
}

static HAL_StatusTypeDef VGA_Transmit(uint8_t*  a_p8u_Data)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		VGA_CS_Write(FALSE);
		errCode = HAL_SPI_Transmit_DMA(&g_hSpi,a_p8u_Data,BUFFERSIZE(a_p8u_Data));
		while(HAL_SPI_GetState(&g_hSpi) == HAL_SPI_STATE_BUSY_TX ) {}
		VGA_CS_Write(TRUE);
	}
	else
	{
		errCode = HAL_ERROR;
	}
	return errCode;
}

static void VGA_CS_Write(LOGIC a_SetReset)
{
	if( a_SetReset == TRUE )
		HAL_GPIO_WritePin(VGA_SPIx_CS_GPIO_PORT,VGA_SPIx_CS_PIN,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(VGA_SPIx_CS_GPIO_PORT,VGA_SPIx_CS_PIN,GPIO_PIN_RESET);
}

static void Set_VGA_Preamp_Gain(VGA_PREAMP_GAIN a_gain)
{
		if( a_gain == HG )
		{
			/**/
		}
	else
		{
			/**/
		}
}
/**** END OF FILE ****/
