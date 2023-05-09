/**************************************************************************
*
*   File:                   Display.c
*
*   Written by:             K. Sielski
*   Date:                   5/9/2023
*
*    Description:
*       Functions to operate Display
*
**************************************************************************/
#include "Global.h"

static const char *TAG = "DISPLAY";



//Init
void Display_Init()
{
	ESP_LOGI(TAG, "In Display Init");

}







//Display Task loop
void Display_Task( void *parameters )
{

	ESP_LOGI(TAG, "In Display Task");


	Display_Init();


	//Infinite Task Loop
    while (true)
    {

    	vTaskDelay(configTICK_RATE_HZ / 2);


    	//Process:
    	//  Get new time
    	//  Generate pattern/digits
    	//  Write pattern to SPI Leds





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


        	LedSPI_Write( start, 4);

        	LedSPI_Write( dataA, 4);
        	LedSPI_Write( dataB, 4);
        	LedSPI_Write( dataC, 4);

        	LedSPI_Write( dataD, 4);
        	LedSPI_Write( dataE, 4);
        	LedSPI_Write( dataF, 4);

        	LedSPI_Write( dataA, 4);
        	LedSPI_Write( dataB, 4);
        	LedSPI_Write( dataC, 4);

        	LedSPI_Write( dataA, 4);


//        	//zero out remaining LEDs, i.e "off"
//        	for(i=0; i<8;i++)
//            	SPI_Write( off, 4);


        	LedSPI_Write( end, 4);


        }



    }	//END Infinite while

}



