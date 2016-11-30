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
struct AD74xx_Data8{
	int :4;
	int payload:8;
	int :4;
};

struct AD74xx_Data8{
	int :4;
	int payload:10;
	int :2;
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern SPI_HandleTypeDef g_hSpi;

#if FAKE_WAVEFORM
	uint8_t g_8u_SamplesBuffer[RX_FAKEBUFFERSIZE] = {49,46,48,48,46,49,47,47,49,46,47,48,45,48,47,46,48,46,47,47,45,48,47,46,48,46,47,48,45,47,47,46,48,46,47,48,45,47,47,45,48,47,46,48,46,47,47,45,48,47,46,51,57,62,70,72,78,85,86,93,97,98,104,104,108,113,113,118,121,121,126,126,128,132,131,135,136,137,140,140,141,144,142,146,146,146,149,148,149,151,149,152,152,151,154,153,153,155,154,155,156,154,156,156,156,158,156,157,158,156,158,158,157,160,158,158,160,157,159,160,158,160,159,160,161,159,160,161,159,161,160,160,162,160,161,162,160,161,161,160,163,156,146,140,130,125,120,111,108,103,95,94,87,83,81,74,73,70,64,65,60,57,56,51,53,51,48,51,48,48,50,47,48,48,46,49,47,47,49,46,47,48,45,48,48,46,49,47,46,48,45,47,47,45,48,47,46,48,46,47,48,45,48,47,46,48,46,47,48,45,47,47,47,48,46,46,48,45,47,47,46,48,46,47,48,51,60,66,69,77,81,84,91,92,96,102,102,108,111,112,118,119,120,124,124,127,131,130,134,135,136,139,138,141,143,142,146,146,146,148,147,149,150,149,152,151,152,154,152,154,154,153,155,155,154,157,155,156,158,155,157,158,156,159,158,158,159,157,158,159,157,160,159,158,161,159,160,160,158,161,160,159,162,160,160,162,159,161,161,160,162,161,161,162,160,151,145,135,129,123,115,112,105,99,96,89,86,83,76,76,71,67,66,61,58,57,52,54,51,49,51,49,49,49,47,49,48,47,49,47,47,49,46,48,47,46,49,47,47,48,45,47,47,46,48,47,46,48,46,47,48,45,48,47,45,48,46,46,48,45,47,47,45,48,47,47,48,45,47,47,46,48,47,46,48,45,47,47,48,57,63,66,74,78,81,88,89,94,99,100,106,109,110,115,116,118,123,123,127,129,129,133,133,136,138,137,141,141,142,145,144,146,148,146,149,149,149,152,151,151,153,151,154,154,153,156,154,155,157,154,156,157,155,158,157,157,159,157,158,159,157,159,159,158,160,159,159,160,158,160,160,158,161,160,160,162,160,160,161,159,161,161,159,162,161,161,162,160,158,150,139,134,126,119,115,107,102,99,91,89,85,79,78,72,69,68,62,61,58,54,55,51,50,51,48,50,49,47,50,48,47,49,46,48,48,46,49,47,46,49,46,47,48,45,48,47,46,49,47,47,48,45,47,48,45,48,47,46,48,45,47,47,45,48,47,46,48,46,47,48,45,48,47,46,48,46,47,48,45,47,47,46,53,58,63,71,73,79,85,87,93,96,99,105,105,109,114,114,118,121,122,127,127,128,132,132,134,137,137,141,141,141,145,143,146,147,146,149,149,149,151,150,152,152,151,154,153,152,155,153,155,156,154,156,155,156,158,157,157,158,156,158,158,157,159,158,158,160,158,158,160,158,159,160,159,161,160,160,161,159,161,161,160,162,160,161,162,160,161,161,160,163,156,146,140,130,124,119,111,107,102,95,93,87,82,81,74,71,70,64,64,154,154,155,153,156,156,155,157,156,156,158,156,157,158,156,159,158,158,160,158,159,160,157,160,159,158,161,160,160,161,159,160,161,159,162,160,160,162,160,161,162,160,161,161,161,156,145,137,131,122,117,112,104,102,96,90,88,81,78,76,70,69,66,61,61,55,54,53,49,52,50,48,50,48,48,49,46,48,48,46,49,47,47,49,46,47,48,46,49,48,46,49,46,47,48,45,48,47,46,47,46,47,48,45,46,47,45,48,47,46,48,45,47,48,45,48,47,46,48,46,46,48,45,48,47,46,49,46,47,56,60,67,73,76,83,87,89,96,96,101,106,107,112,115,115,121,121,123,127,127,130,133,133,137,137,138,141,139,142,144,144,147,147,147,150,148,150,151,150,153,152,152,154,152,154,155,153,156,156,155,157,156,156,158,155,157,158,156,159,158,158,159,157,159,159,158,160,159,159,161,159,160,160,159,161,160,160,162,160,160,161,159,161,161,160,162,161,161,160,149,141,134,125,122,115,107,105,97,93,90,83,81,78,72,71,67,62,62,57,55,54,50,52,50,49,51,48,48,49,47,49,48,47,49,46,47,48,46,48,47,46,48,46,47,48,45,47,47,46,48,47,47,48,45,47,47,45,48,47,46,48,46,46,47,45,47,47,46,48,46,47,47,45,47,47,46,48,46,46,48,45,47,51,56,65,70,73,81,83,87,93,95,99,104,105,110,113,114,119,119,122,126,126,130,131,132,136,135,138,140,139,144,143,144,146,145,147,148,147,151,150,150,153};
#else
	uint8_t g_8u_SamplesBuffer[RX_BUFFERSIZE]; // Main acquisition buffer
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

HAL_StatusTypeDef ADC_Init(void)
{
	return HAL_OK;
}

HAL_StatusTypeDef ADC_DeInit(void)
{
	return HAL_OK;
}

HAL_StatusTypeDef ADC_Receive(void)
{
	HAL_StatusTypeDef errCode=HAL_OK;
	
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		if( SPI_DMA )
			errCode = HAL_SPI_Receive_DMA(&g_hSpi,g_8u_SamplesBuffer,BUFFERSIZE(g_8u_SamplesBuffer));
		else
			errCode = HAL_SPI_Receive_IT(&g_hSpi,g_8u_SamplesBuffer,BUFFERSIZE(g_8u_SamplesBuffer));
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

uint8_t ADC_Is_Received()
{
	return HAL_SPI_GetState(&g_hSpi) == HAL_SPI_STATE_BUSY_RX;
}


/**** END OF FILE ****/
