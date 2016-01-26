#define RGB(r,g,b) ((r) & 0x1F | (((g) & 0x1F) <<5) | (((b) & 0x1F) <<10))
#define OFFSET(x,y) ((x)*240+(y))
#define SCANLINECOUNTER *(volatile u16*)0x4000006

//#include "mylib.h"

typedef unsigned short u16;

u16* videoBuffer = (u16*) 0x6000000;

void setPixel(int x, int y, u16 color)
{
	videoBuffer[OFFSET(x, y)] = color;
}

void drawRect(int x, int y, int width, int height, u16 color)
{
	int i,j;
	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			setPixel(i+x, j+y, color);
		}
	}
}

void drawHollowRect(int x, int y, int width, int height, u16 color)
{
	int i, j;
	for (i = y; i <= y + width; i++) {
			setPixel(x, i, color);
		}
		for (i = y; i <= y + width; i++) {
			setPixel(x+height, i, color);
		}
		for (j = x; j <= x + height; j++) {
			setPixel(j, y, color);
		}
		for (j = x; j <= x + height; j++) {
			setPixel(j, y + width, color);
		}

}

void drawImage3(int x, int y, int width, int height, const u16* image)
{
	int i,j;
 	for(i = x; i < width+x; i++) {
		for(j = y; j < height+y; j++) {
			videoBuffer[j * 240 + i] = image[(j-y) * width + (i-x)];
		} 
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void fillScreen(u16 color) {
	drawRect(0, 0, 240, 160, color);
}



