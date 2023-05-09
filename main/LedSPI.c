/**************************************************************************
*
*   File:                   LedSPI.c
*
*   Written by:             K. Sielski
*   Date:                   5/9/2023
*
*    Description:
*       Functions to initialize and run SPI connected to SK9822 LEDs
*
**************************************************************************/
#include "Global.h"

#include "driver/spi_master.h"

static spi_device_handle_t  spi;

static const char *TAG = "SPI";




// Initialize SPI Bus
esp_err_t LedSPI_Init(void)
{
    esp_err_t ret;

	ESP_LOGI(TAG, "In LedSPI_Init");

    spi_bus_config_t buscfg={
        .miso_io_num     = SPI_NOT_USED,
        .mosi_io_num     = PIN_SPI_MOSI,
        .sclk_io_num     = PIN_SPI_CLK,
        .quadwp_io_num   = -1,
        .quadhd_io_num   = -1,
        .max_transfer_sz = 32
    };

    spi_device_interface_config_t devcfg={
        .clock_speed_hz = 1000000,           //Clock out at 1 MHz
        .mode           = 0,                 //SPI mode 0
        .spics_io_num   = SPI_NOT_USED,      //CS pin
        .queue_size     = 1                  //Queue 1 transaction
    };

    //Initialize the SPI bus
    //ret=spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ret=spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_DISABLED);

    ESP_ERROR_CHECK(ret);

    //Attach the HOST to the SPI bus
    ret=spi_bus_add_device(SPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);

	return ret;
}



// Write to LEDs via SPI
int32_t LedSPI_Write(const uint8_t *bufp, uint16_t len)
{
	spi_transaction_t t;

	memset(&t, 0, sizeof(t));

	t.length    = 8*len;
	t.tx_buffer = bufp;
	return spi_device_polling_transmit(spi, &t);
}



