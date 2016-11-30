/**
  ******************************************************************************
  * @file    SPI/SPI_Config.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   SPI configuration file
  ******************************************************************************
  */

/****** Pin config ******/
/************************/
/***** MISO MOSI SCK ****/
/***** PB14 PB15 PI1 ****/
/************************/

/* DMA1 Request mapping */
/********** RX **********/
/* Channel 0 : Stream 3 */
/********** TX **********/
/* Channel 0 : Stream 4 */
/************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal_conf.h"
#include <stdlib.h>
/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef g_hSpi;
DMA_HandleTypeDef g_hTxDma, g_hRxDma;
uint8_t g_aTxBuffer[20]="DUMMY";

/* Private function prototypes -----------------------------------------------*/
HAL_StatusTypeDef Init_spi();
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspInit_IT(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit_IT(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspInit_DMA(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit_DMA(SPI_HandleTypeDef *hspi);

HAL_StatusTypeDef Init_spi()
{
	HAL_StatusTypeDef ERR_CODE;
	// Locals
	
	// Prepare SPI2 struct
	g_hSpi.Instance = SPIx;
	g_hSpi.Init.Mode = SPI_MODE_MASTER;
	g_hSpi.Init.Direction = SPI_DIRECTION_2LINES;
	g_hSpi.Init.DataSize = SPI_DATASIZE_16BIT;
	g_hSpi.Init.CLKPolarity = SPI_POLARITY_HIGH;
	g_hSpi.Init.CLKPhase = SPI_PHASE_1EDGE;
	g_hSpi.Init.NSS = SPI_NSS_HARD_OUTPUT;
	g_hSpi.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	g_hSpi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	g_hSpi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	g_hSpi.Init.TIMode = SPI_TIMODE_DISABLE;
	g_hSpi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	
	// Initialize SPI2
	HAL_SPI_Init(&g_hSpi);
	if ( ERR_CODE != HAL_OK)
		return ERR_CODE;
	
	return HAL_OK;
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	if ( SPI_DMA )
		HAL_SPI_MspInit_DMA(hspi);
	else
		HAL_SPI_MspInit_IT(hspi);
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	if ( SPI_DMA )
		HAL_SPI_MspDeInit_DMA(hspi);
	else
		HAL_SPI_MspDeInit_IT(hspi);
}

void HAL_SPI_MspInit_IT(SPI_HandleTypeDef *hspi)
{
		// Locals
	GPIO_InitTypeDef GPIO_InitStructure;

  if (hspi->Instance == SPIx)
		{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		SPIx_SCK_GPIO_CLK_ENABLE();
		SPIx_MISO_GPIO_CLK_ENABLE();
		SPIx_MOSI_GPIO_CLK_ENABLE();
		SPIx_NSS_GPIO_CLK_ENABLE();
		/* Enable SPI2 clock */
		SPIx_CLK_ENABLE();
		/* Enable DMA clock */
		DMAx_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStructure.Pin = SPIx_SCK_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(SPIx_SCK_GPIO_PORT,&GPIO_InitStructure);
		
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStructure.Pin = SPIx_MISO_PIN;
		HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT,&GPIO_InitStructure);
		
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStructure.Pin = SPIx_MOSI_PIN;
		HAL_GPIO_Init(SPIx_MISO_GPIO_PORT,&GPIO_InitStructure);
		
		/* SPI NSS GPIO pin configuration */
		GPIO_InitStructure.Pin = SPIx_NSS_PIN;
		HAL_GPIO_Init(SPIx_NSS_PORT,&GPIO_InitStructure);
		
		/*##-3- Configure the NVIC for SPI #########################################*/ 
		/* NVIC for SPI */
		HAL_NVIC_SetPriority(SPIx_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SPIx_IRQn);
	}
}

void HAL_SPI_MspInit_DMA(SPI_HandleTypeDef *hspi)
{
	// Locals
	GPIO_InitTypeDef GPIO_InitStructure;

  if (hspi->Instance == SPIx)
		{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		SPIx_SCK_GPIO_CLK_ENABLE();
		SPIx_MISO_GPIO_CLK_ENABLE();
		SPIx_MOSI_GPIO_CLK_ENABLE();
		SPIx_NSS_GPIO_CLK_ENABLE();
		/* Enable SPI2 clock */
		SPIx_CLK_ENABLE();
		/* Enable DMA clock */
		DMAx_CLK_ENABLE();

		
		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStructure.Pin = SPIx_SCK_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(SPIx_SCK_GPIO_PORT,&GPIO_InitStructure);
		
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStructure.Pin = SPIx_MISO_PIN;
		HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT,&GPIO_InitStructure);
		
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStructure.Pin = SPIx_MOSI_PIN;
		HAL_GPIO_Init(SPIx_MISO_GPIO_PORT,&GPIO_InitStructure);
		
		/* SPI NSS GPIO pin configuration */
		GPIO_InitStructure.Pin = SPIx_NSS_PIN;
		HAL_GPIO_Init(SPIx_NSS_PORT,&GPIO_InitStructure);
		
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		g_hTxDma.Instance = SPIx_TX_DMA_STREAM;
		g_hTxDma.Init.Channel = SPIx_TX_DMA_CHANNEL;
		g_hTxDma.Init.Direction = DMA_MEMORY_TO_PERIPH;
		g_hTxDma.Init.PeriphInc = DMA_PINC_DISABLE;
		g_hTxDma.Init.MemInc = DMA_MINC_ENABLE;
		g_hTxDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		g_hTxDma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		g_hTxDma.Init.Mode = DMA_NORMAL;
		g_hTxDma.Init.Priority = DMA_PRIORITY_LOW;
		g_hTxDma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		g_hTxDma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
		g_hTxDma.Init.MemBurst = DMA_MBURST_SINGLE;
		g_hTxDma.Init.PeriphBurst = DMA_MBURST_SINGLE;
		
		HAL_DMA_Init(&g_hTxDma);
		
		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi,hdmatx,g_hTxDma);
		
		// Prepare DMA for Rx struct
		g_hRxDma.Instance = SPIx_RX_DMA_STREAM;
		g_hRxDma.Init.Channel = SPIx_RX_DMA_CHANNEL;
		g_hRxDma.Init.Direction = DMA_PERIPH_TO_MEMORY;
		g_hRxDma.Init.PeriphInc = DMA_PINC_DISABLE;
		g_hRxDma.Init.MemInc = DMA_MINC_ENABLE;
		g_hRxDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		g_hRxDma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		g_hRxDma.Init.Mode = DMA_NORMAL;
		g_hRxDma.Init.Priority = DMA_PRIORITY_HIGH;
		g_hRxDma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		g_hRxDma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
		g_hRxDma.Init.MemBurst = DMA_MBURST_SINGLE;
		g_hRxDma.Init.PeriphBurst = DMA_PBURST_SINGLE;
		
		HAL_DMA_Init(&g_hRxDma);
		
		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi,hdmarx,g_hRxDma);
		
		/*##-4- Configure the NVIC for DMA #########################################*/ 
		/* NVIC configuration for DMA transfer complete interrupt (SPI2_TX) */
		HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 1, 1);
		HAL_NVIC_EnableIRQ(SPIx_DMA_TX_IRQn);
		
		/* NVIC configuration for DMA transfer complete interrupt (SPI2_RX) */
		HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SPIx_DMA_RX_IRQn);
		
		/*##-5- Configure the NVIC for SPI #########################################*/
		/* NVIC for SPI */
		HAL_NVIC_SetPriority(SPIx_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SPIx_IRQn);
	}
}

void HAL_SPI_MspDeInit_IT(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPIx)
  {
    /*##-1- Reset peripherals ##################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Deconfigure SPI SCK */
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    /* Deconfigure SPI MISO */
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    /* Deconfigure SPI MOSI */
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);

    /*##-3- Disable the NVIC for SPI ###########################################*/
    HAL_NVIC_DisableIRQ(SPIx_IRQn);
  }
}

void HAL_SPI_MspDeInit_DMA(SPI_HandleTypeDef *hspi)
{
  if(hspi->Instance == SPIx)
  {
    /*##-1- Reset peripherals ##################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Deconfigure SPI SCK */
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    /* Deconfigure SPI MISO */
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    /* Deconfigure SPI MOSI */
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);

    /*##-3- Disable the DMA ####################################################*/
    /* De-Initialize the DMA associated to transmission process */
    HAL_DMA_DeInit(&g_hTxDma);
    /* De-Initialize the DMA associated to reception process */
    HAL_DMA_DeInit(&g_hRxDma);

    /*##-4- Disable the NVIC for DMA ###########################################*/
    HAL_NVIC_DisableIRQ(SPIx_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(SPIx_DMA_RX_IRQn);
		
    /*##-3- Disable the NVIC for SPI ###########################################*/
    HAL_NVIC_DisableIRQ(SPIx_IRQn);
  }
}
