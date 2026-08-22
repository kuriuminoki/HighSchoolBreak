#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED


#include "Button.h"


class Character;

enum GROUP_KIND;


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
	GROUP_KIND m_damageGroupKind; // 攻撃したキャラの所属
	// int m_enemyDamageValu;

public:
	Cell(CELL_KIND cellKind, int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor);

	// ゲッタ
	inline CELL_KIND getCellKind() const { return m_cellKind; }
	inline Character* getCharacter() const { return m_character_p; }
	inline int getMarkingColor() const { return m_markingColor; }

	// セッタ
	inline void setCharacter(Character* character_p) { m_character_p = character_p; }
	inline void setMarkingColor(int markingColor) { m_markingColor = markingColor; }
	inline void setDamageValue(int damageValue, GROUP_KIND damageGroupKind) { m_damageValue = damageValue; m_damageGroupKind = damageGroupKind; }

	void draw(int handX, int handY, bool fill) const;

	bool ableMoving(); // 通過できるマスかどうか

	void damageCharacter();

};


#endif