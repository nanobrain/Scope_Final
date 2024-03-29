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

#include "main.h"
#include "ADC_Config.h"
#include "Data_Processing.h"
#include <stdlib.h>
#include <cstdio>

extern SPI_HandleTypeDef g_hSpi;

// Pointer to buffer which is currently used for acquisition
//ADCData8* g_d8_pBuffer;
// Main acquisition buffers
ADCData8 g_d8_RxBufferMain1			[RX_BUFFERCOUNT]
														__attribute__((at(AQQ_MAIN_BUFFER_ADDRESS)))
														/*__attribute__((aligned(16)))*/;

/*ADCData8 g_d8_RxBufferMain2		[RX_BUFFERCOUNT]
														__attribute__((at(AQQ_COPY_BUFFER_ADDRESS)))*/
														/*__attribute__((aligned(16)))*/;

HAL_StatusTypeDef ADC_Init(void)
{
	//g_d8_RxBufferMain = (ADCData8*)malloc(BUFFERSIZE(g_d8_RxBufferMain));
	// TODO: Check returned address!!
	return HAL_OK;
}

HAL_StatusTypeDef ADC_DeInit(void)
{
	return HAL_OK;
}

HAL_StatusTypeDef ADC_Receive(void)
{
	#if FAKE_WAVEFORM == 1
		static uint16_t offset=0, ctr=0;
	#endif

	HAL_StatusTypeDef errCode=HAL_OK;
	
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )// If SPI initialized
	{
		if( SPI_DMA )
			errCode = HAL_SPI_Receive_DMA(&g_hSpi,(uint8_t*)g_d8_RxBufferMain1,BUFFERSIZE(g_d8_RxBufferMain1));
		else
			errCode = HAL_SPI_Receive_IT(&g_hSpi,(uint8_t*)g_d8_RxBufferMain1,BUFFERSIZE(g_d8_RxBufferMain1));
	}
	else
	{
		errCode = HAL_ERROR;
	}
	
	#if FAKE_WAVEFORM == 1
	while(ADC_Is_Busy()){}
			
	if( ctr++ % 127 )
		offset=(offset+1)%127;
	
	k=0;
	for(i=0;i<RX_BUFFERCOUNT-offset;i++)
	{
		k=(k+1)%127;
		g_d8_RxBufferMain1[i+offset].payload = k;
	}
	#endif
	
 	return errCode;
}

uint8_t ADC_Is_Busy()
{
	return HAL_SPI_GetState(&g_hSpi) == HAL_SPI_STATE_BUSY_RX;
}

uint8_t ADC_Get_Payload(uint16_t a_Data)
{
	return (((a_Data & 0x0FF0) >> 4) + 127);
}

/**** END OF FILE ****/
