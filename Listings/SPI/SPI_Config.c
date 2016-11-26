/* Includes */
#include "main.h"
#include "stm32f7xx_hal_conf.h"         // Keil::Device:STM32Cube Framework:Classic

#include <stdlib.h>
#define SPI_BUFFER 8

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

SPI_HandleTypeDef g_hspi;
DMA_HandleTypeDef htx_dma, hrx_dma;

HAL_StatusTypeDef Init_spi()
{
	HAL_StatusTypeDef ERR_CODE;
	// Locals
	
	// Prepare SPI2 struct
	g_hspi.Instance = SPIx;
	g_hspi.Init.Mode = SPI_MODE_MASTER;
	g_hspi.Init.Direction = SPI_DIRECTION_2LINES;
	g_hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	g_hspi.Init.CLKPolarity = SPI_POLARITY_HIGH;
	g_hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
	g_hspi.Init.NSS = SPI_NSS_SOFT;
	g_hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	g_hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	g_hspi.Init.TIMode = SPI_TIMODE_DISABLE;
	g_hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	
	// Initialize SPI2
	HAL_SPI_Init(&g_hspi);
	if ( ERR_CODE != HAL_OK)
		return ERR_CODE;
	
	return HAL_OK;
}

// Implementation of __weak HAL_SPI_MspInit
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi){
	// Locals
	GPIO_InitTypeDef GPIO_InitStructure;

  if (hspi->Instance == SPIx)
		{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		SPIx_SCK_GPIO_CLK_ENABLE();
		SPIx_MISO_GPIO_CLK_ENABLE();
		SPIx_MOSI_GPIO_CLK_ENABLE();
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
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT,&GPIO_InitStructure);
		
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStructure.Pin = SPIx_MOSI_PIN;
		GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(SPIx_MISO_GPIO_PORT,&GPIO_InitStructure);
		
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		htx_dma.Instance = SPIx_TX_DMA_STREAM;
		htx_dma.Init.Channel = SPIx_TX_DMA_CHANNEL;
		htx_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
		htx_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		htx_dma.Init.MemInc = DMA_MINC_ENABLE;
		htx_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		htx_dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		htx_dma.Init.Mode = DMA_NORMAL;
		htx_dma.Init.Priority = DMA_PRIORITY_LOW;
		htx_dma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		htx_dma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
		htx_dma.Init.MemBurst = DMA_MBURST_SINGLE;
		htx_dma.Init.PeriphBurst = DMA_MBURST_SINGLE;
		
		HAL_DMA_Init(&htx_dma);
		
		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi,hdmatx,htx_dma);
		
		// Prepare DMA for Rx struct
		hrx_dma.Instance = SPIx_RX_DMA_STREAM;
		hrx_dma.Init.Channel = SPIx_RX_DMA_CHANNEL;
		hrx_dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hrx_dma.Init.PeriphInc = DMA_PINC_DISABLE;
		hrx_dma.Init.MemInc = DMA_MINC_ENABLE;
		hrx_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hrx_dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hrx_dma.Init.Mode = DMA_NORMAL;
		hrx_dma.Init.Priority = DMA_PRIORITY_HIGH;
		hrx_dma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hrx_dma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
		hrx_dma.Init.MemBurst = DMA_MBURST_SINGLE;
		hrx_dma.Init.PeriphBurst = DMA_PBURST_SINGLE;
		
		HAL_DMA_Init(&hrx_dma);
		
		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi,hdmarx,hrx_dma);
		
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

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
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
    HAL_DMA_DeInit(&htx_dma);
    /* De-Initialize the DMA associated to reception process */
    HAL_DMA_DeInit(&hrx_dma);

    /*##-4- Disable the NVIC for DMA ###########################################*/
    HAL_NVIC_DisableIRQ(SPIx_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(SPIx_DMA_RX_IRQn);
  }
}
