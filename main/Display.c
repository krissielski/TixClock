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


//Colors
static color_t cRed   = {255,   0,   0};
static color_t cGreen = {  0, 255,   0};
static color_t cBlue  = {  0,   0, 255};
static color_t cOff   = {  0,   0,   0};

//Pattern holder
static uint8_t pattern[9];


//The LIST
pixel_t pixelList[] = {

//		{ &pattern[0], &cOff   },

		{ &pattern[0], &cGreen },
		{ &pattern[0], &cGreen },

		{ &pattern[1], &cBlue  },
		{ &pattern[1], &cBlue  },

		{ &pattern[2], &cRed   },
		{ &pattern[2], &cRed   },

		{ &pattern[3], &cGreen },
		{ &pattern[3], &cGreen },

		{ &pattern[4], &cBlue  },
		{ &pattern[4], &cBlue  },

//		{ &pattern[0], &cOff   },

};


//Pixel List Size
#define PIXEL_LIST_SIZE  (sizeof(pixelList)/sizeof(pixel_t))


//Init
void Display_Init()
{
	ESP_LOGI(TAG, "In Display Init");

	for(int i =0; i<9; i++)
		pattern[i] = 0;

}

//Send pixel
void Display_SendPixel(color_t *color)
{
	uint8_t spidata[4] = {0,0,0,0};

	spidata[0] = 0xE0 | 0x03;		//LED Frame header
	spidata[1] = color->b;			//Blue
	spidata[2] = color->g;			//Green
	spidata[3] = color->r;			//Red

	LedSPI_Write(spidata,4);
}

void Display_SendStartFrame(void)
{
	const uint8_t start[] = { 0x00,0x00,0x00,0x00 };
	LedSPI_Write( start, 4);
}
void Display_SendEndFrame(void)
{
	const uint8_t end[]   = { 0xFF,0xFF,0xFF,0xFF };
	LedSPI_Write( end, 4);
}



//Display Task loop
void Display_Task( void *parameters )
{

	static int off_pos = 0;

	ESP_LOGI(TAG, "In Display Task");


	Display_Init();


	//Infinite Task Loop
    while (true)
    {
    	int i;


    	vTaskDelay(configTICK_RATE_HZ / 2);


    	//Process:
    	//  Get new time


    	//  Generate pattern/digits
    	for(i=0; i<9; i++)
    		pattern[i] = 1;

    	pattern[off_pos] = 0;

    	off_pos++;
    	if( off_pos == 5) off_pos=0;



    	//  Write pattern to SPI Leds
		Display_SendStartFrame();

		for(i=0; i<PIXEL_LIST_SIZE; i++ )
		{
			if( *pixelList[i].pixel > 0 )
				Display_SendPixel( pixelList[i].color );
			else
				Display_SendPixel( &cOff );

		}

		Display_SendEndFrame();




//        {
//        	uint8_t i;
//        	uint8_t start[4] = { 0x00,0x00,0x00,0x00 };
//        	uint8_t end[4]   = { 0xFF,0xFF,0xFF,0xFF };
//
//        	uint8_t off[4]   = { 0xFF,0x00,0x00,0x00 };
//
//
//        	uint8_t dataA[4] = { 0xE3,0x7F,0x00,0x00 };
//        	uint8_t dataB[4] = { 0xE3,0x00,0x7F,0x00 };
//        	uint8_t dataC[4] = { 0xE3,0x00,0x00,0x7F };
//
//        	uint8_t dataD[4] = { 0xE3,0xFF,0x00,0x00 };
//        	uint8_t dataE[4] = { 0xE3,0x00,0xFF,0x00 };
//        	uint8_t dataF[4] = { 0xE3,0x00,0x00,0xFF };
//
//
//        	LedSPI_Write( start, 4);
//
//        	LedSPI_Write( dataA, 4);
//        	LedSPI_Write( dataB, 4);
//        	LedSPI_Write( dataC, 4);
//
//        	LedSPI_Write( dataD, 4);
//        	LedSPI_Write( dataE, 4);
//        	LedSPI_Write( dataF, 4);
//
//        	LedSPI_Write( dataA, 4);
//        	LedSPI_Write( dataB, 4);
//        	LedSPI_Write( dataC, 4);
//
//        	LedSPI_Write( dataA, 4);
//
//
////        	//zero out remaining LEDs, i.e "off"
////        	for(i=0; i<8;i++)
////            	SPI_Write( off, 4);
//
//
//        	LedSPI_Write( end, 4);
//
//
//        }



    }	//END Infinite while

}



