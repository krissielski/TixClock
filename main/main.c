
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
//#include "driver/gpio.h"
#include "led_strip.h"



//***  PINS ******
#define PIN_LED_RGB 	8

#define LED_RGB_RMT_CHANNEL 0

static const char *TAG = "MAIN";


static led_strip_t *pRGBled;


//LED Init
static void LED_Init(void)
{
    //RGB LED
    pRGBled = led_strip_init(LED_RGB_RMT_CHANNEL, PIN_LED_RGB, 1);
    pRGBled->clear(pRGBled, 50);
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


    }	//END Infinite while

}
