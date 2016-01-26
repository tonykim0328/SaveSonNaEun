#include "background.h"
#include "banana.h"
#include "egg.h"
#include "enemy.h"
#include "gameover.h"
#include "heart.h"
#include "introduction.h"
#include "player.h"
#include "poop.h"
#include "rock.h"
#include "title.h"
#include "win.h"
//#include "mylib.h"

#define REG_DISPCNT *(unsigned short*) 0x4000000
#define MODE_3 3
#define BG2_EN (1 << 10)
#define RGB(R,G,B) (((R) & 0x1F) | (((G) & 0x1F) << 5) | (((B) & 0x1F) << 10))

#define BUTTONS       *(volatile unsigned short*) 0x4000130
#define BUTTON_A      (1 << 0)
#define BUTTON_B      (1 << 1)
#define BUTTON_START  (1 << 3)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_RIGHT  (1 << 4)
#define BUTTON_LEFT   (1 << 5)
#define BUTTON_UP     (1 << 6)
#define BUTTON_DOWN   (1 << 7)
#define BUTTON_R      (1 << 8)
#define BUTTON_L      (1 << 9)
#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

typedef unsigned short u16;
extern u16* videoBuffer;

//Function prototypes
void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int width, int height, u16 color);
void drawHollowRect(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16* image);
void waitForVblank();
int play();
void drawPlayer();
void fillScreen(u16 color);
void drawEnemy();

enum GBAState {
	START,
	INSTRUCT,
	GAME,
	WIN,
	GAMEOVER
};

typedef struct player {
	int x;
	int y;
	int score;
}Object;

typedef struct enemy {
	int x;
	int y;
}Object2;

Object naeun = {120, 135, 0};
Object2 fan = {5, 0};
Object2 fan4 = {53, 0};
Object2 fan2 = {101, 0};
Object2 fan5 = {149, 0};
Object2 fan3 = {197, 0};

Object2 egg1 = {37, -10};
Object2 rock1 = {83, -73};
Object2 heart1 = {129, -145};
Object2 poop1 = {175, -45};
Object2 banana1 = {221, -85};

int main(void) {
	REG_DISPCNT = MODE_3 | BG2_EN;

	enum GBAState state = START;
	
	while(1) {
		waitForVblank();
		switch(state) {
		case START:
			drawImage3(0, 0, TITLE_WIDTH, TITLE_HEIGHT, title);
			naeun.x = 120;
			naeun.y = 135;
			naeun.score = 0;
			egg1.x = 37;
			egg1.y = -10;
			rock1.x = 83;
			rock1.y = -73;
			heart1.x = 129;
			heart1.y = -145;
			poop1.x = 175;
			poop1.y = -45;
			banana1.x = 221;
			banana1.y = -85;
			
			
			if(KEY_DOWN_NOW(BUTTON_START)) {
				state = INSTRUCT;
			}
			break;

		case INSTRUCT:
			drawImage3(0, 0, INTRODUCTION_WIDTH, INTRODUCTION_HEIGHT, introduction);
			if(KEY_DOWN_NOW(BUTTON_A)) {
				state = GAME;
				fillScreen(RGB(31, 31, 31));
			} else if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = START;
			}
			break;

		case GAME:

			state = play();
			break;
		
		case WIN:
			drawImage3(0, 0, WIN_WIDTH, WIN_HEIGHT, win);
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = START;
			}
			break;
		
		case GAMEOVER:
			drawImage3(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameover);
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = START;
			}
			break;
		}
	}
	
	return 0;
}

int play() {
	drawPlayer();
	while(1) {

		drawEnemy();	

		if (naeun.score == 30) {
			return WIN;
		}

		if (KEY_DOWN_NOW(BUTTON_LEFT)) {
			naeun.x = naeun.x - 1;
			drawPlayer();
		} else if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
			naeun.x = naeun.x + 1;
			drawPlayer();
		} else if (KEY_DOWN_NOW(BUTTON_UP)) {
			naeun.y = naeun.y - 1;
			drawPlayer();
		} else if (KEY_DOWN_NOW(BUTTON_DOWN)) {
			naeun.y = naeun.y + 1;
			drawPlayer();
		}
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			return START;
		}
		
		

		egg1.y %= 160;
		rock1.y %= 160;
		heart1.y %= 160;
		poop1.y %= 160;
		banana1.y %= 160;

		if ((((naeun.x <= egg1.x) && (naeun.x+20 >= egg1.x)) && (naeun.y == egg1.y+17)) || (((naeun.x <= egg1.x+16) && 			(naeun.x+20 >= egg1.x+16)) && (naeun.y == egg1.y+17)) || (((naeun.y <= egg1.y+17) && (naeun.y+25 >= egg1.y+17)) && 			(naeun.x == egg1.x+16)) || (((naeun.y <= egg1.y) && (naeun.y+25 >= egg1.y)) && (naeun.x == egg1.x+16)) || (((naeun.y 			<= egg1.y+17) && (naeun.y+25 >= egg1.y+17)) && (naeun.x+20 == egg1.x)) || (((naeun.y <= egg1.y) && (naeun.y+25 >= 			egg1.y)) && (naeun.x+20 == egg1.x)) || (((naeun.x <= egg1.x) && (naeun.x+20 >= egg1.x)) && (naeun.y+25 == egg1.y)) || 			(((naeun.x <= egg1.x+16) && (naeun.x+20 >= egg1.x+16)) && (naeun.y+25 == egg1.y)) ||

		(((naeun.x <= rock1.x) && (naeun.x+20 >= rock1.x)) && (naeun.y == rock1.y+17)) || (((naeun.x <= rock1.x+16) && 			(naeun.x+20 >= rock1.x+16)) && (naeun.y == rock1.y+17)) || (((naeun.y <= rock1.y+17) && (naeun.y+25 >= rock1.y+17)) 			&& (naeun.x == rock1.x+16)) || (((naeun.y <= rock1.y) && (naeun.y+25 >= rock1.y)) && (naeun.x == rock1.x+16)) || 			(((naeun.y <= rock1.y+17) && (naeun.y+25 >= rock1.y+17)) && (naeun.x+20 == rock1.x)) || (((naeun.y <= rock1.y) && 			(naeun.y+25 >= rock1.y)) && (naeun.x+20 == rock1.x)) || (((naeun.x <= rock1.x) && (naeun.x+20 >= rock1.x)) && 			(naeun.y+25 == rock1.y)) || (((naeun.x <= rock1.x+16) && (naeun.x+20 >= rock1.x+16)) && (naeun.y+25 == rock1.y)) ||

		(((naeun.x <= poop1.x) && (naeun.x+20 >= poop1.x)) && (naeun.y == poop1.y+17)) || (((naeun.x <= poop1.x+16) && 			(naeun.x+20 >= poop1.x+16)) && (naeun.y == poop1.y+17)) || (((naeun.y <= poop1.y+17) && (naeun.y+25 >= poop1.y+17)) 			&& (naeun.x == poop1.x+16)) || (((naeun.y <= poop1.y) && (naeun.y+25 >= poop1.y)) && (naeun.x == poop1.x+16)) || 			(((naeun.y <= poop1.y+17) && (naeun.y+25 >= poop1.y+17)) && (naeun.x+20 == poop1.x)) || (((naeun.y <= poop1.y) && 			(naeun.y+25 >= poop1.y)) && (naeun.x+20 == poop1.x)) || (((naeun.x <= poop1.x) && (naeun.x+20 >= poop1.x)) && 			(naeun.y+25 == poop1.y)) || (((naeun.x <= poop1.x+16) && (naeun.x+20 >= poop1.x+16)) && (naeun.y+25 == poop1.y)) ||

		(((naeun.x <= banana1.x) && (naeun.x+20 >= banana1.x)) && (naeun.y == banana1.y+17)) || (((naeun.x <= banana1.x+16) 			&& (naeun.x+20 >= banana1.x+16)) && (naeun.y == banana1.y+17)) || (((naeun.y <= banana1.y+17) && (naeun.y+25 >= 		banana1.y+17)) && (naeun.x == banana1.x+16)) || (((naeun.y <= banana1.y) && (naeun.y+25 >= banana1.y)) && (naeun.x == 			banana1.x+16)) || (((naeun.y <= banana1.y+17) && (naeun.y+25 >= banana1.y+17)) && (naeun.x+20 == banana1.x)) || 		(((naeun.y <= banana1.y) && (naeun.y+25 >= banana1.y)) && (naeun.x+20 == banana1.x)) || (((naeun.x <= banana1.x) && 			(naeun.x+20 >= banana1.x)) && (naeun.y+25 == banana1.y)) || (((naeun.x <= banana1.x+16) && (naeun.x+20 >= 			banana1.x+16)) && (naeun.y+25 == banana1.y))) {

			return GAMEOVER;
		}

		if ((((naeun.x <= heart1.x) && (naeun.x+20 >= heart1.x)) && (naeun.y == heart1.y+17)) || (((naeun.x <= heart1.x+16) 			&& (naeun.x+20 >= heart1.x+16)) && (naeun.y == heart1.y+17)) || (((naeun.y <= heart1.y+17) && 			  			(naeun.y+25 >= heart1.y+17)) && (naeun.x == heart1.x+16)) || (((naeun.y <= heart1.y) && (naeun.y+25 >= heart1.y)) && 			(naeun.x == heart1.x+16)) || (((naeun.y <= heart1.y+17) && (naeun.y+25 >= heart1.y+17)) && (naeun.x+20 == heart1.x)) 			|| (((naeun.y <= heart1.y) && (naeun.y+25 >= heart1.y)) && (naeun.x+20 == heart1.x)) || (((naeun.x <= heart1.x) && 			(naeun.x+20 >= heart1.x)) && (naeun.y+25 == heart1.y)) || (((naeun.x <= heart1.x+16) && (naeun.x+20 >= heart1.x+16)) 			&& (naeun.y+25 == heart1.y))) {
			
			naeun.score = naeun.score + 1;
			if (naeun.score == 19) {
				return WIN;
			}
		}

	}
	return 0;
}

void drawPlayer() {
	if (naeun.x < 0) {
		naeun.x = 0;
	} else if (naeun.x > 220) {
		naeun.x = 220;
	}
	
	if (naeun.y < 0) {
		naeun.y = 0;
	} else if (naeun.y > 135) {
		naeun.y = 135;
	}
	drawImage3(naeun.x, naeun.y, PLAYER_WIDTH, PLAYER_HEIGHT, player);
}	

void drawEnemy() {
	egg1.y = egg1.y + 1;

	if (egg1.x == 37 && egg1.y == 160) {
		egg1.x = 200;
	} else if (egg1.x == 200 && egg1.y == 160) {
		egg1.x = 70;
	} else if (egg1.x == 70 && egg1.y == 160) {
		egg1.x = 130;
	} else if (egg1.x == 130 && egg1.y == 160) {
		egg1.x = 155;
	} else if (egg1.x == 155 && egg1.y == 160) {
		egg1.x = 37;
	}
	
	
	rock1.y = rock1.y + 1;

	if (rock1.x == 83 && rock1.y == 160) {
		rock1.x = 20;
	} else if (rock1.x == 20 && rock1.y == 160) {
		rock1.x = 114;
	} else if (rock1.x == 114 && rock1.y == 160) {
		rock1.x = 182;
	} else if (rock1.x == 182 && rock1.y == 160) {
		rock1.x = 217;
	} else if (rock1.x == 217 && rock1.y == 160) {
		rock1.x = 83;
	}


	heart1.y = heart1.y + 1;

	if (heart1.x == 129 && heart1.y == 160) {
		heart1.x = 78;
	} else if (heart1.x == 78 && heart1.y == 160) {
		heart1.x = 33;
	} else if (heart1.x == 33 && heart1.y == 160) {
		heart1.x = 216;
	} else if (heart1.x == 216 && heart1.y == 160) {
		heart1.x = 111;
	} else if (heart1.x == 111 && heart1.y == 160) {
		heart1.x = 129;
	}

	poop1.y = poop1.y + 1;

	if (poop1.x == 175 && poop1.y == 160) {
		poop1.x = 100;
	} else if (poop1.x == 100 && poop1.y == 160) {
		poop1.x = 210;
	} else if (poop1.x == 210 && poop1.y == 160) {
		poop1.x = 35;
	} else if (poop1.x == 35 && poop1.y == 160) {
		poop1.x = 77;
	} else if (poop1.x == 77 && poop1.y == 160) {
		poop1.x = 175;
	}

	banana1.y = banana1.y + 1; 
	
	if (banana1.x == 221 && banana1.y == 160) {
		banana1.x = 122;
	} else if (banana1.x == 122 && banana1.y == 160) {
		banana1.x = 166;
	} else if (banana1.x == 166 && banana1.y == 160) {
		banana1.x = 58;
	} else if (banana1.x == 58 && banana1.y == 160) {
		banana1.x = 29;
	} else if (banana1.x == 29 && banana1.y == 160) {
		banana1.x = 221;
	}
	
	drawImage3(fan.x, fan.y, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
	drawImage3(fan2.x, fan2.y, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
	drawImage3(fan3.x, fan3.y, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
	drawImage3(fan4.x, fan4.y, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
	drawImage3(fan5.x, fan5.y, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
	
	drawImage3(egg1.x, egg1.y, EGG_WIDTH, EGG_HEIGHT, egg);
	drawImage3(rock1.x, rock1.y, ROCK_WIDTH, ROCK_HEIGHT, rock);
	drawImage3(heart1.x, heart1.y, HEART_WIDTH, HEART_HEIGHT, heart);
	drawImage3(poop1.x, poop1.y, POOP_WIDTH, POOP_HEIGHT, poop);
	drawImage3(banana1.x, banana1.y, BANANA_WIDTH, BANANA_HEIGHT, banana);
}

