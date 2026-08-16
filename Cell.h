#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED


#include "Button.h"


class Character;


enum CELL_KIND {
	NONE,	// 通れない
	NORMAL,	// 何もない普通のマス
};


const int CELL_WIDE = 80;
const int CELL_HEIGHT = 50;
const int CELL_DISTANCE = 10;


class Cell : public Button {
private:
	CELL_KIND m_cellKind;

	Character* m_character_p; // このマスにいるキャラ いなければnull

	int m_markingColor; // マーキングしていないなら-1

	int m_damageValue; // 攻撃範囲となっていないなら0 (生徒の攻撃)
	// int m_enemyDamageValu;

public:
	Cell(CELL_KIND cellKind, int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor);

	inline CELL_KIND getCellKind() const { return m_cellKind; }

	// ゲッタ
	inline Character* getCharacter() const { return m_character_p; }
	inline int getMarkingColor() const { return m_markingColor; }

	// セッタ
	inline void setCharacter(Character* character_p) { m_character_p = character_p; }
	inline void setMarkingColor(int markingColor) { m_markingColor = markingColor; }
	inline void setDamageValue(int damageValue) { m_damageValue = damageValue; }

	void draw(int handX, int handY, bool fill) const;

	bool ableMoving(); // 通過できるマスかどうか

};


#endif