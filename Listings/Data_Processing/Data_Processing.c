/**
  ******************************************************************************
  * @file    Data_Processing/Data_Processing.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Data processing source
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "Main.h"
#include "Data_Processing.h"
#include "Threads.h"

DMA_HandleTypeDef g_hMemcpDma;

void Init_Memcp_DMA(uint16_t* a_Source,uint16_t* a_Destination, uint16_t a_Size)
{
	/*##-1- Enable DMA Clocks ###################################################*/
	MEMCP_DMAx_CLK_ENABLE();
	
	/*##-2- Configure the DMA ##################################################*/
	// Prepare DMA struct
	g_hMemcpDma.Instance = MEMCP_DMAx_STREAM;
	g_hMemcpDma.Init.Channel = MEMCP_DMAx_CHANNEL;
	g_hMemcpDma.Init.Direction = DMA_MEMORY_TO_MEMORY;
	g_hMemcpDma.Init.MemInc = DMA_MINC_ENABLE;
	g_hMemcpDma.Init.PeriphInc = DMA_PINC_ENABLE;
	g_hMemcpDma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	g_hMemcpDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	g_hMemcpDma.Init.Mode = DMA_NORMAL;
	g_hMemcpDma.Init.Priority = DMA_PRIORITY_MEDIUM;
	g_hMemcpDma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	g_hMemcpDma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	g_hMemcpDma.Init.MemBurst = DMA_MBURST_SINGLE;
	
	// Initialize DMA
	HAL_DMA_Init(&g_hMemcpDma);
	
	/*##-3- Configure the NVIC for DMA #########################################*/ 
	HAL_NVIC_SetPriority(MEMCP_DMAx_IRQn,1,1);
	
	HAL_DMA_Start_IT(&g_hMemcpDma,(uint32_t)a_Source,(uint32_t)a_Destination,a_Size); // IN CASE OF PROBLEMS
																																										// REFER TO "Table 29. Restriction on NDT versus PSIZE and MSIZE"
																																										// IN STM32F7 REFERENCE MANUAL
}

void minmax (ADCData8* a, uint16_t i, uint16_t j, uint8_t* min, uint8_t* max)
	{
  uint8_t lmin, lmax, rmin, rmax, mid;
  if (i == j) {
    *min = a[i].payload;
    *max = a[j].payload;
  } else if (j == i + 1) {
    if (a[i].payload > a[j].payload) {
      *min = a[j].payload;
      *max = a[i].payload;
    } else {
      *min = a[i].payload;
      *max = a[j].payload;
    }
  } else {
    mid = (i + j) / 2;
    minmax(a, i, mid, &lmin, &lmax);
    minmax(a, mid + 1, j, &rmin, &rmax);
    *min = (lmin > rmin) ? rmin : lmin;
    *max = (lmax > rmax) ? lmax : rmax;
  }
}

uint8_t Auto_Trigger(ADCData8* Signal, uint16_t Size)
{
	// NOT USED
	uint8_t min,max;
	uint8_t trigger;
	osMutexWait(mid_Acquisition,osWaitForever);
	/* Critical section */
		{
			minmax(Signal,0,Size,&min,&max);
			trigger = (max+min)/2;
		}
	/* END of critical section */
	osMutexRelease(mid_Acquisition);
		return trigger;
}

uint16_t Trigger(uint8_t Trigger_Point, volatile ADCData8* Signal, uint16_t Size, uint16_t Offset, uint16_t Screen_Size )
{
	uint16_t i = 0;
	uint16_t ctr = 0;
	Size -= Screen_Size/2;

	for(i=Offset;i<=Size;i++) // Find first raising edge. Start from offset
	{
		if(Signal[i].payload <= Trigger_Point)// Signal is invertet, that's why there is '<='
		{
			ctr=i;
			break;
		}
	}

	for(i=ctr;i<=Size;i++) // Find falling edge
	{
		if(Signal[i].payload > Trigger_Point)
		{
			ctr=i;
			break;
		}
	}

	for(i=ctr;i<=Size;i++) // Find second raising edge
	{
		if(Signal[i].payload <= Trigger_Point)
		{
			return i - Screen_Size/2;
		}
	}

	return Offset;
}
/**** END OF FILE ****/
