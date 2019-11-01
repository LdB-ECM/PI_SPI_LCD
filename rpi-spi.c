#include <stdbool.h>			// Standard C library needed for bool
#include <stdint.h>				// Standard C library needed for uint8_t, uint32_t etc
#include "rpi-SmartStart.h"     // SmartStart system need for many command
#include "windows.h"
#include "bmpfont.h"

#define AUX_ENABLES (RPi_IO_Base_Addr + 0x215004)
#define GPFSEL0  (RPi_IO_Base_Addr + 0x200000)
#define GPFSEL1  (RPi_IO_Base_Addr + 0x200004)
#define GPFSEL2  (RPi_IO_Base_Addr + 0x200008)

#define AUX_SPI0_CS     (RPi_IO_Base_Addr + 0x204000)
#define AUX_SPI0_FIFO   (RPi_IO_Base_Addr + 0x204004)
#define AUX_SPI0_CLK    (RPi_IO_Base_Addr + 0x204008)
#define AUX_SPI0_DLEN   (RPi_IO_Base_Addr + 0x20400C)
#define AUX_SPI0_LTOH   (RPi_IO_Base_Addr +  0x204010)
#define AUX_SPI0_DC		(RPi_IO_Base_Addr + 0x204014)

#define GPSET0    (RPi_IO_Base_Addr + 0x20001C)
#define GPCLR0    (RPi_IO_Base_Addr + 0x200028)
#define GPPUD     (RPi_IO_Base_Addr + 0x200094)
#define GPPUDCLK0 (RPi_IO_Base_Addr + 0x200098)

/* Level 1 Commands (from the display Datasheet) */
#define ILI9341_CMD_NOP					0x00
// 0x02
#define ILI9341_CMD_SOFTWARE_RESET			0x01
#define ILI9341_CMD_READ_DISP_ID			0x04
#define ILI9341_CMD_READ_DISP_STATUS			0x09
#define ILI9341_CMD_READ_DISP_POWER_MODE		0x0A
#define ILI9341_CMD_READ_DISP_MADCTRL			0x0B
#define ILI9341_CMD_READ_DISP_PIXEL_FORMAT		0x0C
#define ILI9341_CMD_READ_DISP_IMAGE_FORMAT		0x0D
#define ILI9341_CMD_READ_DISP_SIGNAL_MODE		0x0E
#define ILI9341_CMD_READ_DISP_SELF_DIAGNOSTIC		0x0F
#define ILI9341_CMD_ENTER_SLEEP_MODE			0x10
#define ILI9341_CMD_SLEEP_OUT				0x11
#define ILI9341_CMD_PARTIAL_MODE_ON			0x12
#define ILI9341_CMD_NORMAL_DISP_MODE_ON			0x13
#define ILI9341_CMD_DISP_INVERSION_OFF			0x20
#define ILI9341_CMD_DISP_INVERSION_ON			0x21
#define ILI9341_CMD_GAMMA_SET				0x26
#define ILI9341_CMD_DISPLAY_OFF				0x28
#define ILI9341_CMD_DISPLAY_ON				0x29
#define ILI9341_CMD_COLUMN_ADDRESS_SET			0x2A
#define ILI9341_CMD_PAGE_ADDRESS_SET			0x2B
#define ILI9341_CMD_MEMORY_WRITE			0x2C
#define ILI9341_CMD_COLOR_SET				0x2D
#define ILI9341_CMD_MEMORY_READ				0x2E
#define ILI9341_CMD_PARTIAL_AREA			0x30
#define ILI9341_CMD_VERT_SCROLL_DEFINITION		0x33
#define ILI9341_CMD_TEARING_EFFECT_LINE_OFF		0x34
#define ILI9341_CMD_TEARING_EFFECT_LINE_ON		0x35
#define ILI9341_CMD_MEMORY_ACCESS_CONTROL		0x36
#define ILI9341_CMD_VERT_SCROLL_START_ADDRESS		0x37
#define ILI9341_CMD_IDLE_MODE_OFF			0x38
#define ILI9341_CMD_IDLE_MODE_ON			0x39
#define ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET		0x3A
#define ILI9341_CMD_WRITE_MEMORY_CONTINUE		0x3C
#define ILI9341_CMD_READ_MEMORY_CONTINUE		0x3E
#define ILI9341_CMD_SET_TEAR_SCANLINE			0x44
#define ILI9341_CMD_GET_SCANLINE			0x45
#define ILI9341_CMD_WRITE_DISPLAY_BRIGHTNESS		0x51
#define ILI9341_CMD_READ_DISPLAY_BRIGHTNESS		0x52
#define ILI9341_CMD_WRITE_CTRL_DISPLAY			0x53
#define ILI9341_CMD_READ_CTRL_DISPLAY			0x54
#define ILI9341_CMD_WRITE_CONTENT_ADAPT_BRIGHTNESS	0x55
#define ILI9341_CMD_READ_CONTENT_ADAPT_BRIGHTNESS	0x56
#define ILI9341_CMD_WRITE_MIN_CAB_LEVEL			0x5E
#define ILI9341_CMD_READ_MIN_CAB_LEVEL			0x5F
#define ILI9341_CMD_READ_ID1				0xDA
#define ILI9341_CMD_READ_ID2				0xDB
#define ILI9341_CMD_READ_ID3				0xDC

/* Level 2 Commands (from the display Datasheet) */
#define ILI9341_CMD_RGB_SIGNAL_CONTROL			0xB0
#define ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL		0xB1
#define ILI9341_CMD_FRAME_RATE_CONTROL_IDLE_8COLOR	0xB2
#define ILI9341_CMD_FRAME_RATE_CONTROL_PARTIAL		0xB3
#define ILI9341_CMD_DISPLAY_INVERSION_CONTROL		0xB4
#define ILI9341_CMD_BLANKING_PORCH_CONTROL		0xB5
#define ILI9341_CMD_DISPLAY_FUNCTION_CONTROL		0xB6
#define ILI9341_CMD_ENTRY_MODE_SET			0xB7
#define ILI9341_CMD_BACKLIGHT_CONTROL_1			0xB8
#define ILI9341_CMD_BACKLIGHT_CONTROL_2			0xB9
#define ILI9341_CMD_BACKLIGHT_CONTROL_3			0xBA
#define ILI9341_CMD_BACKLIGHT_CONTROL_4			0xBB
#define ILI9341_CMD_BACKLIGHT_CONTROL_5			0xBC
#define ILI9341_CMD_BACKLIGHT_CONTROL_6			0xBD
#define ILI9341_CMD_BACKLIGHT_CONTROL_7			0xBE
#define ILI9341_CMD_BACKLIGHT_CONTROL_8			0xBF
#define ILI9341_CMD_POWER_CONTROL_1			0xC0
#define ILI9341_CMD_POWER_CONTROL_2			0xC1
#define ILI9341_CMD_VCOM_CONTROL_1			0xC5
#define ILI9341_CMD_VCOM_CONTROL_2			0xC7
#define ILI9341_CMD_POWER_ON_SEQ_CONTROL		0xCB
#define ILI9341_CMD_POWER_CONTROL_A			0xCD
#define ILI9341_CMD_POWER_CONTROL_B			0xCF
#define ILI9341_CMD_NVMEM_WRITE				0xD0
#define ILI9341_CMD_NVMEM_PROTECTION_KEY		0xD1
#define ILI9341_CMD_NVMEM_STATUS_READ			0xD2
#define ILI9341_CMD_READ_ID4				0xD3
#define ILI9341_CMD_POSITIVE_GAMMA_CORRECTION		0xE0
#define ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION		0xE1
#define ILI9341_CMD_DIGITAL_GAMMA_CONTROL_1		0xE2
#define ILI9341_CMD_DIGITAL_GAMMA_CONTROL_2		0xE3
#define ILI9341_CMD_DRIVER_TIMING_CONTROL_A		0xE8
#define ILI9341_CMD_DRIVER_TIMING_CONTROL_B		0xEA
#define ILI9341_CMD_ENABLE_3_GAMMA_CONTROL		0xF2
#define ILI9341_CMD_INTERFACE_CONTROL			0xF6
#define ILI9341_CMD_PUMP_RATIO_CONTROL	0xF7

#define PORTRAIT	0
#define LANDSCAPE	1

#define black	0x0000	// 
#define white	0xFFFF	// RGB
#define red	0xF800	// R
#define green	0x07E0	// G
#define blue	0x001F	// B
#define yellow	0xFFE0  // RG
#define cyan	0x07FF	// GB
#define magenta	0xF81F	// RB
#define gray	0x0821	// 00001 000001 00001


//------------------------------------------------------------------------
void spi_one_byte (bool is_data, uint8_t x)
{
	gpio_output(25, is_data);		// D/C = 0 for cmd  1 for data
	PUT32(AUX_SPI0_CS, 0x000000B0); //TA=1 cs asserted
	while (1)
	{
		if (GET32(AUX_SPI0_CS) & (1 << 18)) break; //TXD
	}
	PUT32(AUX_SPI0_FIFO, x);
	while (1) if (GET32(AUX_SPI0_CS) & (1 << 16)) break;
	//while(1) if(GET32(AUX_SPI0_CS)&(1<<17)) break; //should I wait for this?
	PUT32(AUX_SPI0_CS, 0x00000000); //cs0 comes back up
}



void spi_data16 (uint16_t data)
{
	uint8_t data1 = data >> 8;
	uint8_t data2 = data & 0xff;
	spi_one_byte(true, data1);
	spi_one_byte(true, data2);
}

unsigned int glcd_orientation = 1;
uint16_t max_x = 320;
uint16_t max_y = 240;

void glcd_setX (uint16_t x0, uint16_t x1)
{
	spi_one_byte(false, ILI9341_CMD_COLUMN_ADDRESS_SET);
	spi_data16(x0);
	spi_data16(x1);
}

void glcd_setY(uint16_t y0, uint16_t y1)
{
	spi_one_byte(false, ILI9341_CMD_PAGE_ADDRESS_SET);
	spi_data16(y0);
	spi_data16(y1);
}

void glcd_hline(uint16_t x0, uint16_t y0, uint16_t length,uint16_t color)
{
	if (glcd_orientation == PORTRAIT)
	{
		glcd_setX(x0, x0 + length);
		glcd_setY(y0, y0);
		spi_one_byte(false, ILI9341_CMD_MEMORY_WRITE);
		int i = 0;
		for (i = 0; i < length; i++)
			spi_data16(color);
	}
	else if (glcd_orientation == LANDSCAPE)
	{
		glcd_setX(y0, y0);
		glcd_setY(max_x - x0 - length, max_x - x0);
		spi_one_byte(false, ILI9341_CMD_MEMORY_WRITE);
		int i = 0;
		for (i = 0; i < length; i++)
			spi_data16(color);
	}
}

void glcd_fillRectangle (uint16_t x0, uint16_t y0, uint16_t length, uint16_t width, uint16_t color)
{
	for (int i = 0; i < width; i++)
		glcd_hline(x0, y0 + i, length, color);
}


#if !defined(__STRINGIFY2)
#define __STRINGIFY2(__p) #__p
#define __STRINGIFY(__p) __STRINGIFY2(__p)
#endif

#define INCLUDE_BINARY_FILE(__variable, __fileName, __section)					 \
__asm__ (                                                                        \
    ".pushsection " __section                                          "\n\t"    \
    ".globl " __STRINGIFY(__variable) "_start;"                        "\n\t"    \
    ".balign 4"                                                        "\n\t"    \
    __STRINGIFY(__variable) "_start: .incbin \"" __fileName "\""       "\n\t"    \
    ".globl " __STRINGIFY(__variable) "_end;"		                   "\n\t"    \
    __STRINGIFY(__variable) "_end: .4byte 0;"                          "\n\t"    \
    ".balign 4"                                                        "\n\t"    \
    ".popsection"                                                      "\n\t"    \
);\
extern uint8_t profacture_start;

INCLUDE_BINARY_FILE(profacture, "profacture.bmp", ".rodata.profacture");

/*--------------------------------------------------------------------------}
{                        BITMAP FILE HEADER DEFINITION                      }
{--------------------------------------------------------------------------*/
typedef struct __attribute__((__packed__, aligned(1))) tagBITMAPFILEHEADER {
	uint16_t  bfType; 												// Bitmap type should be "BM"
	uint32_t  bfSize; 												// Bitmap size in bytes
	uint16_t  bfReserved1; 											// reserved short1
	uint16_t  bfReserved2; 											// reserved short2
	uint32_t  bfOffBits; 											// Offset to bmp data
} BITMAPFILEHEADER;

/*--------------------------------------------------------------------------}
{                    BITMAP FILE INFO HEADER DEFINITION						}
{--------------------------------------------------------------------------*/
typedef struct __attribute__((__packed__, aligned(1))) tagBITMAPINFOHEADER {
	uint32_t biSize; 												// Bitmap file size
	uint32_t biWidth; 												// Bitmap width
	uint32_t biHeight;												// Bitmap height
	uint16_t biPlanes; 												// Planes in image
	uint16_t biBitCount; 											// Bits per byte
	uint32_t biCompression; 										// Compression technology
	uint32_t biSizeImage; 											// Image byte size
	uint32_t biXPelsPerMeter; 										// Pixels per x meter
	uint32_t biYPelsPerMeter; 										// Pixels per y meter
	uint32_t biClrUsed; 											// Number of color indexes needed
	uint32_t biClrImportant; 										// Min colours needed
} BITMAPINFOHEADER;

void CvtBmpLine(uint16_t cX,
	            uint16_t imgDepth,
	            uint8_t* imgSrc,
	            RGB565* imageDest)
{
	if (imgDepth == 24) {
		RGB565 out;
		RGB565* __attribute__((aligned(2))) video_wr_ptrD = imageDest;
		RGB* __attribute__((aligned(1))) video_wr_ptrS = (RGB*)(uintptr_t)imgSrc;
		for (uint16_t i = 0; i < cX; i++) {
			RGB val = *video_wr_ptrS++;
			out.R = val.rgbRed >> 3;
			out.G = val.rgbGreen >> 2;
			out.B = val.rgbBlue >> 3;
			*video_wr_ptrD++ = out;
		}
	}
	else {
		RGB565 out;
		RGB565* __attribute__((aligned(2))) video_wr_ptrD = imageDest;
		RGBA* __attribute__((aligned(4))) video_wr_ptrS = (RGBA*)(uintptr_t)imgSrc;
		for (uint16_t i = 0; i < cX; i++) {
			RGBA val = *video_wr_ptrS++;
			out.R = val.rgbRed >> 3;
			out.G = val.rgbGreen >> 2;
			out.B = val.rgbBlue >> 3;
			*video_wr_ptrD++ = out;
		}
	}
}


void DisplayBitmap (uint16_t xoff, uint16_t yoff, uint8_t* bmp) 
{
	BITMAPFILEHEADER* bmpHeader = (BITMAPFILEHEADER *)bmp;
	if (bmpHeader->bfType == 19778) 
	{
		BITMAPINFOHEADER* bmpInfo = (BITMAPINFOHEADER*)(bmp + sizeof(BITMAPFILEHEADER));
		uint32_t xferWidth, bytesPerPixel;
		RGB565 buf[640] = { 0 };
		if (bmpInfo->biBitCount == 24) bytesPerPixel = 3;
			else bytesPerPixel = 4;
		xferWidth = bmpInfo->biWidth * bytesPerPixel;
		int p2Mod = xferWidth % 4;
		if (p2Mod != 0) xferWidth += (4 - p2Mod);
		uint8_t* src = (uint8_t*)(bmp + bmpHeader->bfOffBits);	
		for (int y = 0; y < bmpInfo->biHeight; y++)
		{
			CvtBmpLine(xferWidth, 24, &src[xferWidth*y], &buf[0]);
			glcd_setX(max_y - yoff + y - bmpInfo->biHeight, max_y - yoff + y - bmpInfo->biHeight);
			glcd_setY(xoff, xoff+bmpInfo->biWidth);
			spi_one_byte(false, ILI9341_CMD_MEMORY_WRITE);
			int i;
			for (i = 0; i < bmpInfo->biWidth; i++) {
				spi_data16(buf[i].Raw16);
			}
		}
	}
}

static void SPI_WriteChar(uint16_t xoff, uint16_t yoff, char ch, RGB565 fc, RGB565 bc, font_t* font) 
{
	uint8_t uchar = (uint8_t)ch;										// Convert char to uint8_t for safety for compares
	if (font && uchar >= font->first_char && uchar <= font->last_char)  // Make sure character in font
	{
		int y;
		uint8_t* bp;
		int pos = (uchar - font->first_char);
		pos *= font->char_pitch;
		bp = &font->font_bitmap[pos];									// Pointer to font data	
		for (y = font->char_height-1; y >= 0; y--)
		{
			int x;
			uint8_t b = *(bp++);										// Load first byte of line
			glcd_setX(max_y - yoff + y - font->char_height, max_y - yoff + y - font->char_height);
			glcd_setY(xoff, xoff + font->char_width);
			spi_one_byte(false, ILI9341_CMD_MEMORY_WRITE);
			for (x = 0; x < font->char_width; x++)						// For each bit
			{
				RGB565 col = ((b & 0x80) != 0) ? fc : bc;				// If bit set take fc else take bc
				spi_data16(col.Raw16);									// Write the pixel colour
				b <<= 1;												// Roll font bits left
				if ((x % 8 == 0) && (x != 0)) b = *(bp++);				// Load next byte line is wider than this byte
			}
		}
	}
}

static void SPI_WriteText(uint16_t xoff, uint16_t yoff, char* txt, RGB565 fc, RGB565 bc, font_t* font)
{
	while (txt && *txt != 0)
	{
		SPI_WriteChar(xoff, yoff, *(txt++), fc, bc, font);
		xoff += font->char_width;
	}
}

void spi_init(void)
{
	uint32_t ra;

	ra = GET32(AUX_ENABLES);
	ra |= 2;					//enable spi0
	PUT32(AUX_ENABLES, ra);

	gpio_setup(9, GPIO_ALTFUNC0);   // MISO
	gpio_setup(8, GPIO_ALTFUNC0);   // CS0
	gpio_setup(7, GPIO_ALTFUNC0);   // CS1 .. USE as RESET
	gpio_setup(10, GPIO_ALTFUNC0);  // MOSI
	gpio_setup(11, GPIO_ALTFUNC0);  // SCLK
	gpio_setup(25, GPIO_OUTPUT);	// USE as CMD_DATA

	PUT32(AUX_SPI0_CS, 0x0000030);  // Clear FIFO's
	//BCM2835_SPI_CLOCK_DIVIDER_512 = 512,     ///< 512 = 2.048us = 488.28125kHz
	//BCM2835_SPI_CLOCK_DIVIDER_256 = 256,     ///< 256 = 1.024us = 976.5625MHz
	//BCM2835_SPI_CLOCK_DIVIDER_128 = 128,     ///< 128 = 512ns = = 1.953125MHz
	//BCM2835_SPI_CLOCK_DIVIDER_64 = 64,      ///< 64 = 256ns = 3.90625MHz
	//BCM2835_SPI_CLOCK_DIVIDER_32 = 32,      ///< 32 = 128ns = 7.8125MHz
	//BCM2835_SPI_CLOCK_DIVIDER_16 = 16,      ///< 16 = 64ns = 15.625MHz
	//BCM2835_SPI_CLOCK_DIVIDER_8 = 8,       ///< 8 = 32ns = 31.25MHz
	//BCM2835_SPI_CLOCK_DIVIDER_4 = 4, ///< 4 = 16ns = 62.5MHz
	PUT32(AUX_SPI0_CLK, 32); 
	//PUT32(AUX_SPI0_CLK, 26);

	gpio_output(25, true); // D/C = 1 for data
	gpio_output(7, false); // reset low
	timer_wait(10000);
	gpio_output(7, true);  // reset high


	spi_one_byte(false, ILI9341_CMD_POWER_ON_SEQ_CONTROL);
	spi_one_byte(true, ILI9341_CMD_IDLE_MODE_ON);
	spi_one_byte(true, ILI9341_CMD_MEMORY_WRITE);
	spi_one_byte(true, ILI9341_CMD_NOP);
	spi_one_byte(true, ILI9341_CMD_TEARING_EFFECT_LINE_OFF);
	spi_one_byte(true, 0x02); 	// XXX 

	spi_one_byte(false, ILI9341_CMD_POWER_CONTROL_B);
	spi_one_byte(true, ILI9341_CMD_NOP);
	spi_one_byte(true, ILI9341_CMD_POWER_CONTROL_2);
	spi_one_byte(true, ILI9341_CMD_PARTIAL_AREA);

	spi_one_byte(false, ILI9341_CMD_DRIVER_TIMING_CONTROL_A);
	spi_one_byte(true, 0x85); 	// XXX
	spi_one_byte(true, ILI9341_CMD_NOP);
	spi_one_byte(true, 0x78); 	// XXX

	spi_one_byte(false, ILI9341_CMD_DRIVER_TIMING_CONTROL_B);
	spi_one_byte(true, ILI9341_CMD_NOP);
	spi_one_byte(true, ILI9341_CMD_NOP);

	spi_one_byte(false, 0xED);	// XXX
	spi_one_byte(true, 0x64); 	// XXX
	spi_one_byte(true, 0x03);	// XXX
	spi_one_byte(true, ILI9341_CMD_PARTIAL_MODE_ON);
	spi_one_byte(true, 0X81); 	// XXX

	spi_one_byte(false, ILI9341_CMD_PUMP_RATIO_CONTROL);
	spi_one_byte(true, ILI9341_CMD_DISP_INVERSION_OFF);

	spi_one_byte(false, ILI9341_CMD_POWER_CONTROL_1);
	spi_one_byte(true, 0x23);	//VRH[5:0] 	// XXX

	spi_one_byte(false, ILI9341_CMD_POWER_CONTROL_2);
	spi_one_byte(true, ILI9341_CMD_ENTER_SLEEP_MODE);

	spi_one_byte(false, ILI9341_CMD_VCOM_CONTROL_1);
	spi_one_byte(true, ILI9341_CMD_READ_MEMORY_CONTINUE);
	spi_one_byte(true, ILI9341_CMD_DISPLAY_OFF);

	spi_one_byte(false, ILI9341_CMD_VCOM_CONTROL_2);
	spi_one_byte(true, 0x86);	//--	// XXX

	spi_one_byte(false, ILI9341_CMD_MEMORY_ACCESS_CONTROL);
	spi_one_byte(true, 0x48);	//C8	//48 68gal.gal.gal.//28 E8 gal.gal.gal.	// XXX

	spi_one_byte(false, ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET);
	spi_one_byte(true, ILI9341_CMD_WRITE_CONTENT_ADAPT_BRIGHTNESS);

	spi_one_byte(false, ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL);
	spi_one_byte(true, ILI9341_CMD_NOP);
	spi_one_byte(true, 0x18); 	// XXX

	spi_one_byte(false, ILI9341_CMD_DISPLAY_FUNCTION_CONTROL);
	spi_one_byte(true, 0x08); 	// XXX
	spi_one_byte(true, 0x82);	// XXX
	spi_one_byte(true, 0x27);	// XXX

	spi_one_byte(false, ILI9341_CMD_ENABLE_3_GAMMA_CONTROL);
	spi_one_byte(true, ILI9341_CMD_NOP);

	spi_one_byte(false, 0x26);	//Gamma curve selected 	// XXX
	spi_one_byte(true, ILI9341_CMD_SOFTWARE_RESET);

	spi_one_byte(false, ILI9341_CMD_POSITIVE_GAMMA_CORRECTION);
	spi_one_byte(true, 0x0F); 	// XXX
	spi_one_byte(true, 0x31);	// XXX
	spi_one_byte(true, ILI9341_CMD_PAGE_ADDRESS_SET);
	spi_one_byte(true, ILI9341_CMD_READ_DISP_PIXEL_FORMAT);
	spi_one_byte(true, ILI9341_CMD_READ_DISP_SIGNAL_MODE);
	spi_one_byte(true, 0x08); 	// XXX
	spi_one_byte(true, 0x4E); 	// XXX
	spi_one_byte(true, 0xF1); 	// XXX
	spi_one_byte(true, ILI9341_CMD_VERT_SCROLL_START_ADDRESS);
	spi_one_byte(true, 0x07); 	// XXX
	spi_one_byte(true, ILI9341_CMD_ENTER_SLEEP_MODE);
	spi_one_byte(true, 0x03);	// XXX
	spi_one_byte(true, ILI9341_CMD_READ_DISP_SIGNAL_MODE);
	spi_one_byte(true, ILI9341_CMD_READ_DISP_STATUS);
	spi_one_byte(true, ILI9341_CMD_NOP);

	spi_one_byte(false, ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION);
	spi_one_byte(true, ILI9341_CMD_NOP);
	spi_one_byte(true, ILI9341_CMD_READ_DISP_SIGNAL_MODE);
	spi_one_byte(true, 0x14); 	// XXX
	spi_one_byte(true, 0x03);	// XXX
	spi_one_byte(true, ILI9341_CMD_SLEEP_OUT);
	spi_one_byte(true, 0x07); 	// XXX
	spi_one_byte(true, 0x31); 	// XXX
	spi_one_byte(true, ILI9341_CMD_POWER_CONTROL_2);
	spi_one_byte(true, 0x48); 	// XXX
	spi_one_byte(true, 0x08); 	// XXX
	spi_one_byte(true, 0x0F); 	// XXX
	spi_one_byte(true, ILI9341_CMD_READ_DISP_PIXEL_FORMAT);
	spi_one_byte(true, 0x31); 	// XXX
	spi_one_byte(true, ILI9341_CMD_MEMORY_ACCESS_CONTROL);
	spi_one_byte(true, ILI9341_CMD_READ_DISP_SELF_DIAGNOSTIC);

	spi_one_byte(false, ILI9341_CMD_SLEEP_OUT);
	timer_wait(120000);

	spi_one_byte(false, ILI9341_CMD_DISPLAY_ON);
	spi_one_byte(false, ILI9341_CMD_MEMORY_WRITE);
	//glcd_bg(bgcolor);
	glcd_fillRectangle(0, 0, 320, 240, blue);

	DisplayBitmap(50, 50, &profacture_start);
	SPI_WriteText(50, 100, "Upside down is fine Ingo", (RGB565) { .Raw16 = yellow }, (RGB565) { .Raw16 = blue }, (font_t*)GetFont(2));
}