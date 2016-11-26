/**
  ******************************************************************************
  * @file    Utils/Error_Handler.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Simple handler of application errors
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Leds.h"
#include "Error_Handler.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Error_Handler(ERRCODE a_errCode)
{
	switch(a_errCode)
	{
		case INIT_ERROR:
		{
			while(1)
			{
				Led(LEDRED1,TRUE);
				HAL_Delay(100);
				Led(LEDRED1,FALSE);
				HAL_Delay(100);
				Led(LEDRED1,TRUE);
				HAL_Delay(100);
				Led(LEDRED1,FALSE);
				HAL_Delay(100);
				Led(LEDRED1,TRUE);
				HAL_Delay(100);
				Led(LEDRED1,FALSE);
				HAL_Delay(500);
			}
		}break;
		case CONVERSION_ERROR:
		{
			while(1)
			{
				Led(LEDRED2,TRUE);
				HAL_Delay(100);
				Led(LEDRED2,FALSE);
				HAL_Delay(100);
				Led(LEDRED2,TRUE);
				HAL_Delay(100);
				Led(LEDRED2,FALSE);
				HAL_Delay(100);
				Led(LEDRED2,TRUE);
				HAL_Delay(100);
				Led(LEDRED2,FALSE);
				HAL_Delay(500);
			}
		}break;
	}
	
	while(1){}
}

/**** END OF FILE ****/
