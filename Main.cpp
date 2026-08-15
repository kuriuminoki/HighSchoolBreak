#include "Control.h"
#include "Define.h"
#include "DxLib.h"

#include "BattleField.h"
#include "BattleFieldDrawer.h"

///////fpsの調整///////////////
static int mStartTime;
static int mCount;
static int debug = FALSE;
static float mFps;
static const int N = FPS_N;
static const int FPS = FPS_N;

bool Update() {
	if (mCount == 0) {
		mStartTime = GetNowCount();
	}
	if (mCount == N) {
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void Draw(int x, int y, int color) {
	DrawFormatString(0, 0, BLUE, "デバッグモード：%.1f FPS, 解像度：%d*%d", mFps, GAME_WIDE, GAME_HEIGHT);
}

void Wait() {
	int tookTime = GetNowCount() - mStartTime;
	int waitTime = mCount * 1000 / FPS - tookTime;
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}

//////////メイン関数///////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	SetGraphMode(GAME_WIDE, GAME_HEIGHT, GAME_COLOR_BIT_NUM);
	ChangeWindowMode(WINDOW), DxLib_Init();
	int screen = MakeScreen(GAME_WIDE, GAME_HEIGHT, TRUE);
	SetDrawScreen(screen);

	SetWindowSizeChangeEnableFlag(TRUE);//windowサイズ変更可能
	SetMainWindowText("High School Break!!"); // ウィンドウの名前
	SetUseDirectInputFlag(TRUE);
	//SetMousePoint(320, 240);//マウスカーソルの初期位置
	SetAlwaysRunFlag(TRUE);//画面を常にアクティブ

	// 画像の拡大処理方式
	//const int DRAW_MODE = DX_DRAWMODE_BILINEAR;
	const int DRAW_MODE = DX_DRAWMODE_NEAREST;
	SetDrawMode(DRAW_MODE);
	SetFullScreenScalingMode(DRAW_MODE);
	SetMouseDispFlag(TRUE);//マウス表示

	// ゲーム本体
	BattleField* battleField = new BattleField();
	BattleFieldDrawer* battleFieldDrawer = new BattleFieldDrawer(battleField);

	while (SetDrawScreen(screen) == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		updateKey();
		mouseClick();

		/////メイン////
		battleField->play();
		battleFieldDrawer->draw();
		///////////////

		//FPS操作
		if (controlDebug() == TRUE) {
			if (debug == FALSE) { debug = TRUE; }
			else { debug = FALSE; }
		}
		Update();
		// デバッグ
		if (debug == TRUE) {
			Draw(0, 0, BLACK);
		}
		Wait();
		if (controlEsc() == TRUE) { DxLib_End(); }
		//FPS操作ここまで

		// 表画面に描画
		SetDrawScreen(DX_SCREEN_FRONT);
		DrawGraph(0, 0, screen, FALSE);
	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}