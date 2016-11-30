/**
  ******************************************************************************
  * @file    ADC/ADC_Config.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   ADC configuration file
  ******************************************************************************
  */

/*********** Pin config ***********/
/**********************************/
/********* CS   OP_AMP_!PD ********/
/********** PG_6    PB_8 **********/
/**********************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ADC_Config.h"
#include <cstdio>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern SPI_HandleTypeDef g_hSpi;
uint8_t g_8u_SamplesBuffer[2048]; // Main acquisition buffer

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

HAL_StatusTypeDef ADC_Init(void)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		ADC_GPIO_CLK_ENABLE();
		GPIO_InitStructure.Pin = ADC_SPIx_CS_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(ADC_SPIx_CS_GPIO_PORT,&GPIO_InitStructure);
		HAL_GPIO_WritePin(ADC_SPIx_CS_GPIO_PORT,ADC_SPIx_CS_PIN,GPIO_PIN_SET);
		errCode=HAL_OK;
	}
	else
	{
		errCode=HAL_ERROR;
	}
	
	return errCode;
}

HAL_StatusTypeDef ADC_DeInit(void)
{
	HAL_GPIO_DeInit(ADC_SPIx_CS_GPIO_PORT,ADC_SPIx_CS_PIN);
	return HAL_OK;
}

HAL_StatusTypeDef ADC_Receive(void)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	uint16_t u16_bufferSize = BUFFERSIZE(g_8u_SamplesBuffer);
	uint16_t u16_dataCtr;
	uint8_t u8_Buffer[2];
	
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		while (u16_dataCtr++ < u16_bufferSize)
		{
			ADC_CS_Write(FALSE);
			errCode = HAL_SPI_Receive_IT(&g_hSpi,u8_Buffer,2);
			while(HAL_SPI_GetState(&g_hSpi) == HAL_SPI_STATE_BUSY_RX ) {}
			ADC_CS_Write(TRUE);
		}
	}
	else
	{
		errCode = HAL_ERROR;
	}
	return errCode;
}

static void ADC_CS_Write(LOGIC a_SetReset)
{
	if( a_SetReset == TRUE )
		HAL_GPIO_WritePin(ADC_SPIx_CS_GPIO_PORT,ADC_SPIx_CS_PIN,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(ADC_SPIx_CS_GPIO_PORT,ADC_SPIx_CS_PIN,GPIO_PIN_RESET);
}

/**** END OF FILE ****/
