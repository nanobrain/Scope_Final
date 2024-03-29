/**
  ******************************************************************************
  * @file    Data_Processing/Data_Processing.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for data processing source
  ******************************************************************************
  */
  
#ifndef __DATA_PROCESSING_H
#define __DATA_PROCESSING_H

typedef struct ADCData8{
	int /*NULL*/	:4;
	int payload		:8;
	int /*NULL*/	:4;
// Bits:				16
} __attribute__ ((__packed__)) ADCData8;

typedef struct ADCData10{
	int /*NULL*/	:2;
	int payload		:10;
	int /*NULL*/	:4;
// Bits:				16
} __attribute__ ((__packed__)) ADCData10 ;

typedef struct DACDataField{
	int /*NULL*/	:6;
	int power 		:2;
	int	value			:16;
// Bits:				24
} /* __attribute__ ((__packed__)) __attribute__ ((aligned(32)))*/ DACDataField;

typedef union DACTXBUFFER {
	DACDataField DataFields;
	uint8_t	buffer[3];
} /* __attribute__ ((__packed__)) __attribute__ ((aligned(32)))*/ DACTXBUFFER;

typedef struct VGADataField{
	int ladder 		:4;
	int preAmp		:1;
	int /*NULL*/	:1;
	int filter		:3;
	int /*NULL*/	:1;
	int power			:1;
	int /*NULL*/	:5;
// Bits:				24
} /* __attribute__ ((__packed__)) __attribute__ ((aligned(32)))*/ VGADataField;

typedef union VGATXBUFFER {
	VGADataField DataFields;
	uint8_t	buffer[2];
} /* __attribute__ ((__packed__)) __attribute__ ((aligned(32)))*/ VGATXBUFFER;

void minmax (ADCData8* a, uint16_t i, uint16_t j, uint8_t* min, uint8_t* max);
uint8_t Auto_Trigger(ADCData8* Signal, uint16_t Size);
uint16_t Trigger(uint8_t Trigger_Point, volatile ADCData8* Signal, uint16_t Size, uint16_t Offset, uint16_t Screen_Size );

#endif /* __DATA_PROCESSING_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
