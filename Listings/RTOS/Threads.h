/**
  ******************************************************************************
  * @file    RTOS/Threads.h
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for threads configuration
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __THREADS_H
#define __THREADS_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Functions prototypes ----------------------------------------------------- */
/* Heart beat thread */
void Heart_Beat_Thread (void const *argument);
osThreadId tid_Heart_Beat_Thread;

/* GUI thread */
void GUIThread (void const *argument);
osThreadId tid_GUIThread;

/* Acquisiion thread */
void Acqusition_Thread (void const *argument);
osThreadId tid_Acqusition_Thread;

#endif /* __THREADS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
