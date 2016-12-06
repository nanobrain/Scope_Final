/**
  ******************************************************************************
  * @file    Data_Processing/Data_Processing.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for data processing source
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATA_PROCESSING_H
#define __DATA_PROCESSING_H

/* Private typedef -----------------------------------------------------------*/
typedef struct Data8{
	int /*NULL*/	:4;
	int payload		:8;
	int /*NULL*/	:4;
// Bits:				16
} __attribute__ ((__packed__)) Data8;

typedef struct Data10{
	int /*NULL*/	:4;
	int payload		:10;
	int /*NULL*/	:2;
// Bits:				16
} __attribute__ ((__packed__)) Data10 ;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void minmax (uint16_t* a, uint16_t i, uint16_t j, uint16_t* min, uint16_t* max);
void Auto_Trigger(void);
uint16_t Trigger(uint8_t Trig_SP, volatile Data8* Signal, uint16_t Sig_Size );

#endif /* __DATA_PROCESSING_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
