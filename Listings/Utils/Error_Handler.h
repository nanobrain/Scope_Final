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
	INIT_ERROR,
	CONVERSION_ERROR
}ERRCODE;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Functions ------------------------------------------------------- */
void Error_Handler(ERRCODE a_errCode);

#endif /* __DAC_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
