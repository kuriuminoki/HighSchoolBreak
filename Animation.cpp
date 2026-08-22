#include "Animation.h"
#include "Graphs.h"


using namespace std;


/*
* エフェクトアニメクラス
*/
EffectAnimation::EffectAnimation(EFFECT_KIND kind, int x, int y, int frameCnt) {
	m_effectKind = kind;
	m_x = x;
	m_y = y;
	m_frameCnt = frameCnt;
}


// 初期化
void EffectAnimation::init() {
	m_cnt = 0;
}


// カウント
void EffectAnimation::count() {
	m_cnt++;
}


// 今何枚目か
int EffectAnimation::getAnimeNum() const { 
	return m_cnt / m_frameCnt;
}
