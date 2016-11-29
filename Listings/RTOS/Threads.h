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
/* Threads declarations  -----------------------------------------------------*/
/* Heart beat thread */
int Init_Heart_Beat_Thread (void);
void Heart_Beat_Thread (void const *argument);
extern osThreadId tid_Heart_Beat_Thread;

/* GUI thread */
int Init_GUIThread (void);
void GUIThread (void const *argument);
extern osThreadId tid_GUIThread;

/* Acquisiion thread */
int Init_Acqusition_Thread (void);
void Acqusition_Thread (void const *argument);
extern osThreadId tid_Acqusition_Thread;

/* Threads items -------------------------------------------------------------*/
extern osMutexId mid_Acquisition;

#endif /* __THREADS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
