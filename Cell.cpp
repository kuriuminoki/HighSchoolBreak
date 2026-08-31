#include "Cell.h"
#include "Animation.h"
#include "Character.h"
#include "Define.h"
#include "Graphs.h"
#include "Skill.h"


#include <algorithm>
#include <sstream>


using namespace std;


Cell::Cell(CELL_KIND cellKind, int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor) :
	Button(x1, y1, x2, y2, edgeLength, innerColor, edgeColor)
{
	m_cellKind = cellKind;

	m_character_p = nullptr;
	m_skill_p = nullptr;
	m_skillOwner_p = nullptr;
	m_effectAnimation = nullptr;
	m_markingColor = -1;
	m_damageValue = 0;
	m_skillTurn = 0;
}


void Cell::setDamageValue(int damageValue, GROUP_KIND damageGroupKind) {
	m_damageValue = damageValue;
	m_damageGroupKind = damageGroupKind;
}


void Cell::playAnimation() {
	if (m_effectAnimation == nullptr) {
		return;
	}
	m_effectAnimation->count();
	if (m_effectAnimation->isEndAnimation()) {
		delete m_effectAnimation;
		m_effectAnimation = nullptr;
	}
}


void Cell::draw(int handX, int handY, bool fill, const CharacterGraphs* characterGraphs, int font) const {
	if (overlap(handX, handY)) {
		DrawBox(m_x1 - m_edgeLength, m_y1 - m_edgeLength, m_x2 + m_edgeLength, m_y2 + m_edgeLength, m_edgeColor, TRUE);
	}
	int color = m_markingColor != -1 ? m_markingColor : m_innerColor;
	DrawBox(m_x1, m_y1, m_x2, m_y2, color, TRUE);

	int cx = (m_x1 + m_x2) / 2;
	int cy = (m_y1 + m_y2) / 2;

	if (m_damageValue > 0) {
		DrawCircle(cx, cy, (m_y2 - m_y1) / 2, LIGHT_RED);
	}
	else if (m_damageValue < 0) {
		DrawCircle(cx, cy, (m_y2 - m_y1) / 2, PINK);
	}

	if (m_skill_p != nullptr) {
		int handle = characterGraphs->getSkillIconGraphs(m_skill_p->getSkillCategory());
		int wide = 0;
		const double ex = 0.2;
		GetGraphSize(handle, &wide, NULL);
		wide = (int)(wide * ex);
		DrawRotaGraph(cx, cy, ex * m_exX, 0.0, handle, FALSE);

		DrawCircle(cx + wide, cy + wide, (int)(wide * 0.7), BLACK);
		ostringstream oss;
		oss << m_skillTurn;
		int fontSize = GetDrawStringWidthToHandle(oss.str().c_str(), oss.str().size(), font);
		DrawStringToHandle(cx + wide - fontSize / 2, cy + wide / 2, oss.str().c_str(), WHITE, font);
	}
}


bool Cell::ableMoving() {
	return m_character_p == nullptr;
}


bool Cell::ableSetSkill() {
	return m_character_p == nullptr && m_skill_p == nullptr;
}


void Cell::damageCharacter() {
	if (m_damageValue == 0 || m_character_p == nullptr) {
		return;
	}
	if (m_damageValue < 0 && m_character_p->getGroupKind() != m_damageGroupKind) {
		return;
	}
	if (m_damageValue > 0 && m_character_p->getGroupKind() == m_damageGroupKind) {
		return;
	}
	// ここで防御力を考慮
	if (m_damageValue > 0) { // 回復にバフ・デバフはかからない
		m_damageValue -= m_character_p->calcDefenseBuffValue();
		if (m_damageValue < 0) {
			m_damageValue = 0;
		}
	}
	m_character_p->damage(m_damageValue);
	if (m_effectAnimation != nullptr) {
		delete m_effectAnimation;
	}
	m_effectAnimation = new EffectAnimation(ATARI, (m_x1 + m_x2) / 2, (m_y1 + m_y2) / 2, 6);
}


void Cell::nextTurn() {
	if (m_skill_p != nullptr) {
		m_skillTurn++;
	}
}
