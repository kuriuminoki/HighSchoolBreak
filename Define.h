#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

#include "DxLib.h"
#include <string>
#include <vector>
#include <utility>

class RGBColor;

// フルスクリーンならFALSE
static int WINDOW = TRUE;
// マウスを表示するならFALSE
static int MOUSE_DISP = FALSE;

// FPS
#define FPS_N 60

// 円周率
#define PI 3.14

//画面の大きさ
#define GAME_WIDE_MAX 3840
#define GAME_HEIGHT_MAX 2160
#define GAME_WIDE_MIN 640
#define GAME_HEIGHT_MIN 480
#define GAME_WIDE_DEFAULT 1920
#define GAME_HEIGHT_DEFAULT 1080
extern int GAME_WIDE;
extern int GAME_HEIGHT;

// 解像度の倍率
void getGameEx(double& exX, double& exY);

int applyEx(int v, double ex);

// 解像度変更
void ChangeGameResolution(int* screen);

#define GAME_COLOR_BIT_NUM 16

// DrawFormatString関数で表示される文字の大きさは20くらい
#define DRAW_FORMAT_STRING_SIZE 20

//色
const int BLACK = GetColor(0, 0, 0);
const int GRAY3 = GetColor(50, 50, 50);
const int GRAY = GetColor(70, 70, 70);
const int GRAY2 = GetColor(130, 130, 130);
const int WHITE = GetColor(200, 200, 200);
const int RED = GetColor(200, 0, 0);
const int LIGHT_RED = GetColor(200, 100, 100);
const int DARK_RED = GetColor(50, 0, 0);
const int BLUE = GetColor(0, 0, 200);
const int LIGHT_BLUE = GetColor(100, 100, 200);
const int LIGHT_SKY = GetColor(100, 200, 200);
const int ORANGE = GetColor(200, 125, 0);
const int DARK_ORANGE = GetColor(80, 50, 0);
const int YELLOW = GetColor(200, 200, 0);
const int LIGHT_YELLOW = GetColor(150, 150, 50);
const int DARK_YELLOW = GetColor(100, 100, 0);
const int LIGHT_GREEN = GetColor(90, 200, 90);
const int GREEN = GetColor(0, 200, 0);

#endif