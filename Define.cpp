#include "Define.h"
#include <cmath>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int GAME_WIDE = GAME_WIDE_MAX;
int GAME_HEIGHT = GAME_HEIGHT_MAX;
//int GAME_WIDE = GAME_WIDE_DEFAULT;
//int GAME_HEIGHT = GAME_HEIGHT_DEFAULT;


// âëúìxÇÃî{ó¶
void getGameEx(double& exX, double& exY) {
	exX = (double)GAME_WIDE / GAME_WIDE_DEFAULT;
	exY = (double)GAME_HEIGHT / GAME_HEIGHT_DEFAULT;
}


int applyEx(int v, double ex) {
	return (int)(v * ex);
}


// âëúìxïœçX
void ChangeGameResolution(int* screen) {
	InitGraph();
	InitSoundMem();
	InitFontToHandle();
	SetGraphMode(GAME_WIDE, GAME_HEIGHT, GAME_COLOR_BIT_NUM);
	ChangeWindowMode(WINDOW), DxLib_Init();
	*screen = MakeScreen(GAME_WIDE, GAME_HEIGHT, TRUE);
	SetDrawScreen(*screen);
	SetMouseDispFlag(TRUE);
}

