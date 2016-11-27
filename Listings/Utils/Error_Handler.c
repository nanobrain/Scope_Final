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
	Leds_All_Off();
	
	switch(a_errCode)
	{
		case ERROR_INIT:
		{
			while(1)
			{
				Led(LEDRED1,TRUE); // Red1 led on
				Led(LEDBLUE,TRUE); // 3x blue led blink
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(100);
				Led(LEDBLUE,TRUE);
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(100);
				Led(LEDBLUE,TRUE);
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(500);
			}
		}break;
		case ERROR_CONVERSION:
		{
			while(1)
			{
				Led(LEDRED2,TRUE); // Red2 led on
				Led(LEDBLUE,TRUE); // 3x blue led blink
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(100);
				Led(LEDBLUE,TRUE);
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(100);
				Led(LEDBLUE,TRUE);
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(500);
			}
		}break;
		case ERROR_THREAD:
		{
			while(1)
			{
				Led(LEDRED1,TRUE); // Red1 & Red2 leds on
				Led(LEDRED2,TRUE);
				Led(LEDBLUE,TRUE); // 3x blue led blink
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(100);
				Led(LEDBLUE,TRUE);
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(100);
				Led(LEDBLUE,TRUE);
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(500);
			}
		}break;
		case ERROR_HARDFAULT:
		{
			while(1)
			{
				Led(LEDRED1,TRUE);
				Led(LEDRED2,TRUE);
				Led(LEDRED3,TRUE);
				Led(LEDBLUE,TRUE); // 3x blue led blink
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(100);
				Led(LEDBLUE,TRUE);
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(100);
				Led(LEDBLUE,TRUE);
				HAL_Delay(100);
				Led(LEDBLUE,FALSE);
				HAL_Delay(500);
			}
		}break;
		case ERROR_HAL_INIT:
		{
			while(1)
			{
				Led(LEDRED1,TRUE);
				Led(LEDRED2,TRUE);
				Led(LEDRED3,TRUE); // 3x all red leds blink
				HAL_Delay(100);
				Led(LEDRED1,FALSE);
				Led(LEDRED2,FALSE);
				Led(LEDRED3,FALSE);
				HAL_Delay(100);
				Led(LEDRED1,TRUE);
				Led(LEDRED2,TRUE);
				Led(LEDRED3,TRUE);
				HAL_Delay(100);
				Led(LEDRED1,FALSE);
				Led(LEDRED2,FALSE);
				Led(LEDRED3,FALSE);
				HAL_Delay(100);
				Led(LEDRED1,TRUE);
				Led(LEDRED2,TRUE);
				Led(LEDRED3,TRUE);
				HAL_Delay(100);
				Led(LEDRED1,FALSE);
				Led(LEDRED2,FALSE);
				Led(LEDRED3,FALSE);
				HAL_Delay(500);
			}
		}
		default:
		{
			while(1)
			{
				Leds_All_Off();
				HAL_Delay(500);
				Leds_All_On();
				HAL_Delay(500);
			}
		}
	}
	
	while(1){}
}

/**** END OF FILE ****/
