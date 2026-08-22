#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED


enum EFFECT_KIND;


/*
* エフェクトアニメ
*/
class EffectAnimation {
private:

	EFFECT_KIND m_effectKind;

	// 座標
	int m_x, m_y;

	// 画像１枚の表示時間
	int m_frameCnt;

	// カウント
	int m_cnt;

public:
	EffectAnimation(EFFECT_KIND kind, int x, int y, int frameCnt);

	// ゲッタ
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline EFFECT_KIND getKind() const { m_effectKind; }

	// セッタ
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }

	// 初期化
	void init();

	// カウント
	void count();

	// 今何枚目か
	int getAnimeNum() const;
};



#endif