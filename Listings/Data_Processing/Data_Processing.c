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
#include "Main.h"
#include "Data_Processing.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

void minmax (uint16_t* a, uint16_t i, uint16_t j, uint16_t* min, uint16_t* max)
	{
  uint16_t lmin, lmax, rmin, rmax, mid;
  if (i == j) {
    *min = a[i];
    *max = a[j];
  } else if (j == i + 1) {
    if (a[i] > a[j]) {
      *min = a[j];
      *max = a[i];
    } else {
      *min = a[i];
      *max = a[j];
    }
  } else {
    mid = (i + j) / 2;
    minmax(a, i, mid, &lmin, &lmax);
    minmax(a, mid + 1, j, &rmin, &rmax);
    *min = (lmin > rmin) ? rmin : lmin;
    *max = (lmax > rmax) ? lmax : rmax;
  }
}

void Auto_Trigger()
{
	uint16_t min,max;
	//osMutexWait(mid_Acquisition,osWaitForever);
	/* Critical section */
		{
			//minmax(g_d8_SamplesBuffer,0,BUFFERSIZE(g_d8_SamplesBuffer),&min,&max);
			//_TriggerPoint = max - min;
		}
	/* END of critical section */
	//osMutexRelease(mid_Acquisition);
}

uint16_t Trigger(uint8_t Trig_SP, volatile Data8* Signal, uint16_t Sig_Size )
{

	uint16_t i=0;
	uint16_t ctr=0;
	uint8_t offset = 50;
	Sig_Size -=480;
	
	for(i=0;i<=Sig_Size;i++) // Find first raising edge. Start from offset
	{
		if((Signal[i].payload >= Trig_SP) && i>=offset)
		{
			ctr=i;
			break;
		}
	}

	for(i=ctr;i<=Sig_Size;i++) // Find falling edge
	{
		if(Signal[i].payload < Trig_SP)
		{
			ctr=i;
			break;
		}
	}
	
	for(i=ctr;i<=Sig_Size;i++) // Find second raising edge
	{
		if(Signal[i].payload >= Trig_SP)
		{
			return i;
		}
	}

	return offset;
}

/**** END OF FILE ****/
