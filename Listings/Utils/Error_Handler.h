/**
  ******************************************************************************
  * @file    DAC/DAC_Config.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for dac configuration file
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DAC_CONFIG_H
#define __DAC_CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum ERRCODE{
	ERROR_INIT,
	ERROR_CONVERSION,
	ERROR_THREAD,
	ERROR_HARDFAULT,
	ERROR_MEMMANAGE,
	ERROR_BUSFAULT,
	ERROR_USAGEFAULT,
	ERROR_DEBUGMON,
	ERROR_HAL_INIT,
	NMI_ERROR
}ERRCODE;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Functions ------------------------------------------------------- */
void Error_Handler(ERRCODE a_errCode);

#endif /* __DAC_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
