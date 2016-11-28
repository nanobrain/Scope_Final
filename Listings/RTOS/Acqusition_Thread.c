/**
  ******************************************************************************
  * @file    RTOS\Acqusition_Thread.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Main acquisition thread
  ******************************************************************************
  */

/*----------------------------------------------------------------------------
 *      Acquisition_Thread:  ADC communication thread
 *---------------------------------------------------------------------------*/
 
 /* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "main.h"
#include "Error_Handler.h"
#include "ADC_Config.h"
#include "Threads.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t values[480] __attribute__((at(0xC0400000)));
osThreadDef (Acqusition_Thread, osPriorityNormal, 1, 0);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int Init_Acqusition_Thread (void) {

  tid_Acqusition_Thread = osThreadCreate (osThread(Acqusition_Thread), NULL);
  if (!tid_Acqusition_Thread) return(-1);
  
  return(0);
}

void Acqusition_Thread (void const *argument) {

  while (1) {
		if( ADC_Receive() != HAL_OK )
			Error_Handler(ERROR_CONVERSION);
		
    osThreadYield ();                                         // suspend thread
  }
}
