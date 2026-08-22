#include "Define.h"
#include "DxLib.h"
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


// 解像度の倍率
void getGameEx(double& exX, double& exY) {
	exX = (double)GAME_WIDE / GAME_WIDE_DEFAULT;
	exY = (double)GAME_HEIGHT / GAME_HEIGHT_DEFAULT;
}


int applyEx(int v, double ex) {
	return (int)(v * ex);
}


// 解像度変更
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


// HPバーを表示
void drawHpBar(int x1, int y1, int x2, int y2, int hp, int nowHp, int maxHp) {
	DrawBox(x1, y1, x2, y2, BLACK, TRUE);
	int wide = x2 - x1;
	int hpWide = wide * hp / maxHp;
	int nowHpWide = wide * nowHp / maxHp;
	DrawBox(x1, y1, x1 + nowHpWide, y2, RED, TRUE);
	DrawBox(x1, y1, x1 + hpWide, y2, GREEN, TRUE);
}

