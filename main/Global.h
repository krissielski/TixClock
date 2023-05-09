/**************************************************************************
*
*   File:                   Global.h
*
*   Written by:             K. Sielski
*   Date:                   5/9/2023
*
*    Description:
*       Global Include
*
**************************************************************************/
#ifndef GLOBAL_H_
#define GLOBAL_H_


/**************************
    Includes
**************************/
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


/**************************
    Defines
**************************/


//---- Onboard RGB LED
#define PIN_LED_RGB 	    8
#define LED_RGB_RMT_CHANNEL 0

//---- LED SPI I/F
#define SPI_HOST        SPI2_HOST
#define PIN_SPI_MOSI    2 //green
#define PIN_SPI_CLK     3 //yellow
#define SPI_NOT_USED   -1

//Unused SPI
#define PIN_SPI_MISO    0
#define PIN_SPI_CS      1




/**************************
	Typedefs
***************************/

typedef struct
{
	uint8_t  r;
	uint8_t  g;
	uint8_t  b;
}color_t;


typedef struct
{
	uint8_t *pixel;
	color_t *color;
}pixel_t;



/**************************
    Function Prototypes
**************************/


/*
 * Display.c
 */
extern void Display_Task( void *parameters );

/*
 * LedSPI.c
 */
extern esp_err_t LedSPI_Init(void);
extern int32_t   LedSPI_Write(const uint8_t *bufp, uint16_t len);


#endif /* GLOBAL_H_ */
