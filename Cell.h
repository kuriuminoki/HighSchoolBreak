#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED


#include "Button.h"


class EffectAnimation;
class Character;
class CharacterGraphs;
class Skill;

enum GROUP_KIND;


enum CELL_KIND {
	BLOCK, // 通れない
	NORMAL,	// 何もない普通のマス
};


const int CELL_WIDE = 80;
const int CELL_HEIGHT = 50;
const int CELL_DISTANCE = 10;


class Cell : public Button {
private:
	CELL_KIND m_cellKind;

	Character* m_character_p; // このマスにいるキャラ いなければnull
	const Skill* m_skill_p;
	const Character* m_skillOwner_p; // 他グループがスキルを使うとマイナスの補正がかかる用
	EffectAnimation* m_effectAnimation;

	int m_markingColor; // マーキングしていないなら-1

	int m_damageValue; // 攻撃範囲となっていないなら0 (生徒の攻撃)
	GROUP_KIND m_damageGroupKind; // 攻撃したキャラの所属
	int m_skillTurn; // スキルが設置されてから経過したターン

public:
	Cell(CELL_KIND cellKind, int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor);

	// ゲッタ
	inline CELL_KIND getCellKind() const { return m_cellKind; }
	inline Character* getCharacter() const { return m_character_p; }
	inline const Skill* getSkill() const { return m_skill_p; }
	inline const Character* getSkillOwner() const { return m_skillOwner_p; }
	inline const EffectAnimation* getEffectAnimation() const { return m_effectAnimation; }
	inline int getMarkingColor() const { return m_markingColor; }
	inline int getSkillTurn() const { return m_skillTurn; }

	// セッタ
	inline void setCharacter(Character* character_p) { m_character_p = character_p; }
	inline void setSkill(const Skill* skill_p, const Character* skillOwner) {
		m_skill_p = skill_p;
		m_skillTurn = 0;
		m_skillOwner_p = skillOwner;
	}
	inline void setMarkingColor(int markingColor) { m_markingColor = markingColor; }
	void setDamageValue(int damageValue, GROUP_KIND damageGroupKind);

	void playAnimation();

	void draw(int handX, int handY, bool fill, const CharacterGraphs* characterGraphs, int font) const;

	bool ableMoving(); // 通過できるマスかどうか

	bool ableSetSkill(); // スキルを設置できるマスかどうか

	void damageCharacter();

	void nextTurn(); // 次のターンにする

};


#endif