#ifndef DICE_H_INCLUDED
#define DICE_H_INCLUDED


#include "Button.h"


/*
* サイコロ
*/
class Dice : public Button {
private:
	int m_value;
	int m_minValue;
	int m_maxValue;

	int m_cnt;

public:
	Dice(int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor);

	// ゲッタ
	inline int getValue() const { return m_value; }
	inline int isPlaying() const { return m_cnt > 0; }

	void init(int maxValue, int minValue, int goalCount); // 回し始める
	bool play(); // 回し終わるとtrue
	void draw(int handX, int handY, int font, int color) const;
};


#endif