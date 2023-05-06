
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
//#include "driver/gpio.h"
#include "led_strip.h"
#include "driver/spi_master.h"



// Onboard RGB LED
#define PIN_LED_RGB 	    8
#define LED_RGB_RMT_CHANNEL 0

// LED SPI I/F
#define SPI_HOST        SPI2_HOST
#define PIN_SPI_MOSI    2 //green
#define PIN_SPI_CLK     3 //yellow
#define SPI_NOT_USED   -1

//Unused
#define PIN_SPI_MISO    0
#define PIN_SPI_CS      1






static const char *TAG = "MAIN";


led_strip_t          *pRGBled;
spi_device_handle_t  spi;

//LED Init
static void LED_Init(void)
{
    //RGB LED
    pRGBled = led_strip_init(LED_RGB_RMT_CHANNEL, PIN_LED_RGB, 1);
    pRGBled->clear(pRGBled, 50);
}


//-----------------------------------------------
static esp_err_t SPI_Master_Init(void)
{
    esp_err_t ret;

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

    printf("INIT SPI START\n");


    //Initialize the SPI bus
    //ret=spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ret=spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_DISABLED);

    ESP_ERROR_CHECK(ret);

    //Attach the HOST to the SPI bus
    ret=spi_bus_add_device(SPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);

    printf("INIT SPI END\n");

	return ret;
}

int32_t SPI_Write(const uint8_t *bufp, uint16_t len)
{
	spi_transaction_t t;

	memset(&t, 0, sizeof(t));

	t.length    = 8*len;
	t.tx_buffer = bufp;
	return spi_device_polling_transmit(spi, &t);
}


//Chip_Info
static void Chip_Info(void)
{

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;

    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
}


void Task1( void *parameters )
{
    printf("Task1 Start\n");
	while(1)
	{
		vTaskDelay(configTICK_RATE_HZ * 2);
		printf("1\n");
	}
}

void Task2( void *parameters )
{
    printf("Task2 Start\n");

	while(1)
	{
		vTaskDelay(configTICK_RATE_HZ * 2);
		printf("2\n");
	}
}


//*****************************
//          MAIN
//*****************************
void app_main(void)
{
	static uint8_t s_rgb_state = 0;

	static TaskHandle_t hTask1;
	static TaskHandle_t hTask2;


	ESP_LOGI(TAG, "TixClock");

    printf("Hello Kris!!!!\n");


    Chip_Info();

    LED_Init();
    SPI_Master_Init();

    //Delay
	vTaskDelay(configTICK_RATE_HZ * 1);

	//Create Tasks
	xTaskCreate( Task1, "Task1", 1024, NULL, 10, &hTask1 );
	xTaskCreate( Task2, "Task2", 1024, NULL,  9, &hTask2 );


	//Infinite Task Loop
    while (true)
    {

    	vTaskDelay(configTICK_RATE_HZ / 2);

    	//RGB Blink
    	switch(s_rgb_state)
    	{

		case 0:	pRGBled->set_pixel(pRGBled, 0,  16, 16, 16);	break;
		case 1:	pRGBled->set_pixel(pRGBled, 0,  50,  0,  0);	break;
		case 2:	pRGBled->set_pixel(pRGBled, 0,  0,  50,  0);	break;
		case 3:	pRGBled->set_pixel(pRGBled, 0,  0,  0,  50);	break;
		case 4:
			pRGBled->set_pixel(pRGBled, 0,  0,  0,  0);
			s_rgb_state = 0;
			break;
    	}
        pRGBled->refresh(pRGBled, 100);
        s_rgb_state++;

        {
        	uint8_t i;
        	uint8_t start[4] = { 0x00,0x00,0x00,0x00 };
        	uint8_t end[4]   = { 0xFF,0xFF,0xFF,0xFF };

        	uint8_t off[4]   = { 0xFF,0x00,0x00,0x00 };


        	uint8_t dataA[4] = { 0xE3,0x7F,0x00,0x00 };
        	uint8_t dataB[4] = { 0xE3,0x00,0x7F,0x00 };
        	uint8_t dataC[4] = { 0xE3,0x00,0x00,0x7F };

        	uint8_t dataD[4] = { 0xE3,0xFF,0x00,0x00 };
        	uint8_t dataE[4] = { 0xE3,0x00,0xFF,0x00 };
        	uint8_t dataF[4] = { 0xE3,0x00,0x00,0xFF };


        	SPI_Write( start, 4);

        	SPI_Write( dataA, 4);
        	SPI_Write( dataB, 4);
        	SPI_Write( dataC, 4);

        	SPI_Write( dataD, 4);
        	SPI_Write( dataE, 4);
        	SPI_Write( dataF, 4);

        	SPI_Write( dataA, 4);
        	SPI_Write( dataB, 4);
        	SPI_Write( dataC, 4);

        	SPI_Write( dataA, 4);


//        	//zero out remaining LEDs, i.e "off"
//        	for(i=0; i<8;i++)
//            	SPI_Write( off, 4);


        	SPI_Write( end, 4);


        }



    }	//END Infinite while

}
