/**
  ******************************************************************************
  * @file    VGA/VGA_Config.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for variable gain amplifier configuration file
  ******************************************************************************
  */
  
#ifndef __VGA_CONFIG_H
#define __VGA_CONFIG_H

typedef enum VGA_BW_FILTER{
	FULL_BW,
	LP20MHZ,
	LP100MHZ,
	LP200MHZ,
	LP350MHZ,
	LP650MHZ,
	LP750MHZ
}VGA_BW_FILTER;

typedef enum VGA_PREAMP_GAIN{
	HG,	// 38.86 dB
	LG	// 18.86 dB
}VGA_PREAMP_GAIN;

typedef enum VGA_GAIN{
//DBLOW				// LG; Ladder = 1010(10)
	DB0_84=1,		// LG; Ladder = 1001 (9)
	DB2_84=2,		// LG; Ladder = 1000 (8)
	DB4_84=3,		// LG; Ladder = 0111 (7)
	DB6_84=4,		// LG; Ladder = 0110 (6)
	DB8_84=5,		// LG; Ladder = 0101 (5)
	DB10_84=6,	// LG; Ladder = 0100 (4)
	DB12_84=7,	// LG; Ladder = 0011 (3)
	DB14_84=8,	// LG; Ladder = 0010 (2)
	DB16_84=9,	// LG; Ladder = 0001 (1)
	DB18_84=10,	// HG; Ladder = 0000 (0)
	DB20_84=11,	// HG; Ladder = 1001 (9)
	DB22_84=12,	// HG; Ladder = 1000 (8)
	DB24_84=13,	// HG; Ladder = 0111 (7)
	DB26_84=14,	// HG; Ladder = 0110 (6)
	DB28_84=15,	// HG; Ladder = 0101 (5)
	DB30_84=16,	// HG; Ladder = 0100 (4)
	DB32_84=17,	// HG; Ladder = 0011 (3)
	DB34_84=18,	// HG; Ladder = 0010 (2)
	DB36_84=19,	// HG; Ladder = 0001 (1)
	DB38_84=20	// HG; Ladder = 0000 (0)
}VGA_GAIN;

typedef enum STEP_DIRECTION{
	STEP_DOWN=0,
	STEP_UP=1
}STEP_DIRECTION;

/* Definition for SPIx Pins */
#define VGA_SPIx_CS_PIN                     GPIO_PIN_4
#define VGA_SPIx_CS_GPIO_PORT               GPIOB
#define VGA_GPIO_CLK_ENABLE									__HAL_RCC_GPIOB_CLK_ENABLE

static void VGA_CS_Write(LOGIC a_SetReset);
static HAL_StatusTypeDef VGA_Transmit(void);
void VGA_Set_Gain(VGA_GAIN a_gain);
int VGA_Step_Gain(STEP_DIRECTION a_Direction);
VGA_GAIN VGA_Get_Gain(void);
HAL_StatusTypeDef VGA_Init(void);
HAL_StatusTypeDef VGA_DeInit(void);

#endif /* __VGA_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
