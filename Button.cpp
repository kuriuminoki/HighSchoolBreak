#include "Button.h"
#include "BattleFieldDrawer.h"
#include "Cell.h"
#include "Character.h"
#include "Define.h"
#include "Graphs.h"
#include "Skill.h"
#include "DxLib.h"


#include <sstream>


using namespace std;


/*
* ボタンの基底クラス
*/
Button::Button(int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor) {
	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
	m_edgeLength = edgeLength;
	m_innerColor = innerColor;
	m_edgeColor = edgeColor;

	getGameEx(m_exX, m_exY);
	m_ableClick = true;
	m_offColor = GRAY3;
}


void Button::draw(int handX, int handY, bool fill) const {
	int color = m_ableClick ? m_innerColor : m_offColor;
	if (fill) {
		if (overlap(handX, handY)) {
			DrawBox(m_x1 - m_edgeLength, m_y1 - m_edgeLength, m_x2 + m_edgeLength, m_y2 + m_edgeLength, m_edgeColor, TRUE);
		}
		DrawBox(m_x1, m_y1, m_x2, m_y2, color, TRUE);
	}
	else {
		if (overlap(handX, handY)) {
			DrawBox(m_x1, m_y1, m_x2, m_y2, m_edgeColor, fill);
		}
		else {
			DrawBox(m_x1, m_y1, m_x2, m_y2, color, fill);
		}
	}
}


/*
* テキスト付きのボタン
*/
TextButton::TextButton(string dispText, int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor) :
	Button(x1, y1, x2, y2, edgeLength, innerColor, edgeColor)
{
	m_dispText = dispText;
}


void TextButton::draw(int handX, int handY, bool fill, int font, int textColor) const {
	Button::draw(handX, handY, fill);
	int fontSize = 0;
	GetFontStateToHandle(NULL, &fontSize, NULL, font);
	DrawStringToHandle((m_x1 + m_x2) / 2 - fontSize * ((int)m_dispText.length() / 4), (m_y1 + m_y2) / 2 - fontSize / 2, m_dispText.c_str(), textColor, font);
}


/*
* 画像付きのボタン
*/
GraphButton::GraphButton(int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor) :
	Button(x1, y1, x2, y2, edgeLength, innerColor, edgeColor)
{

}


void GraphButton::draw(int handX, int handY, bool fill, int graphHandle) const {
	Button::draw(handX, handY, fill);
	int wide = 0, height = 0;
	GetGraphSize(graphHandle, &wide, &height);
	double ex = min((double)(m_x2 - m_x1) / wide, (double)(m_y2 - m_y1) / height);
	DrawRotaGraph((m_x1 + m_x2) / 2, (m_y1 + m_y2) / 2, ex, 0.0, graphHandle, TRUE);
}


/*
* キャラ情報の領域
*/
CharacterInfoButton::CharacterInfoButton(int x1, int y1, int x2, int y2, const Character* character_p):
	Button(x1, y1, x2, y2, 10, LIGHT_BLUE, BLUE)
{
	m_character_p = character_p;

	double exX = 1, exY = 1;
	getGameEx(exX, exY);
	const int dx = applyEx(10, exX);
	const int dy = applyEx(100, exY);
	const int SKILL_WIDE = min((x2 - x1) / 2, (y2 - y1) / 2) - applyEx(15, exX);
	for (unsigned int i = 0; i < m_character_p->getSkill().size(); i++) {
		int sx1 = i % 2 == 0 ? x1 + dx : x1 + dx + SKILL_WIDE + dx;
		int sy1 = i / 2 == 0 ? y1 + dy : y1 + dy + SKILL_WIDE + dx;
		m_skillButton.push_back(new GraphButton(sx1, sy1, sx1 + SKILL_WIDE, sy1 + SKILL_WIDE, applyEx(3, exX), WHITE, RED));
	}
}


CharacterInfoButton::~CharacterInfoButton() {
	for (unsigned int i = 0; i < m_skillButton.size(); i++) {
		delete m_skillButton[i];
	}
}


Skill* CharacterInfoButton::getOverlapSkill(int handX, int handY) const {
	for (unsigned int i = 0; i < m_skillButton.size(); i++) {
		if (m_skillButton[i]->overlap(handX, handY)) {
			return m_character_p->getSkill()[i];
		}
	}
	return nullptr;
}


void CharacterInfoButton::draw(int handX, int handY, const CharacterGraphs* characterGraphs, int font) const {
	if (m_character_p == nullptr) {
		return;
	}
	Button::draw(handX, handY, true);
	int height = 0;
	int handle = characterGraphs->getCharacterIconGraphs(m_character_p->getCharacterProfile()->getCharacterIconGraphNum());
	GetGraphSize(handle, NULL, &height);
	const double EX = 0.28;
	DrawRotaGraph((m_x1 + m_x2) / 2, m_y2 - (int)(height * EX / 2), EX, 0.0, handle, TRUE);

	// 名前
	DrawStringToHandle(m_x1, m_y1, m_character_p->getCharacterProfile()->getFullName().c_str(), WHITE, font);

	// HP
	int fontSize = 0;
	GetFontStateToHandle(NULL, &fontSize, NULL, font);
	int hp = m_character_p->getCharacterStatus()->getHp();
	int dispHp = m_character_p->getCharacterStatus()->getDispHp();
	int maxHp = m_character_p->getCharacterStatus()->getMaxHp();
	drawHpBar(m_x1 + applyEx(5, m_exX), m_y1 + fontSize + applyEx(5, m_exY), m_x1 + applyEx(200, m_exX), m_y1 + fontSize + applyEx(15, m_exY), hp, dispHp, maxHp);

	// キャラが保持するスキルの各ボタン
	if (overlap(handX, handY)) {
		for (unsigned int i = 0; i < m_skillButton.size(); i++) {
			m_skillButton[i]->draw(handX, handY, true, characterGraphs->getSkillIconGraphs(m_character_p->getSkill()[i]->getSkillCategory()));
		}
	}
}


/*
* マス情報の領域
*/
CellInfoButton::CellInfoButton(int x1, int y1, int x2, int y2, const Cell* cell_p) :
	Button(x1, y1, x2, y2, 10, GRAY2, RED)
{
	m_cell_p = cell_p;
}


void CellInfoButton::draw(int handX, int handY, const CharacterGraphs* characterGraphs, int font) const {
	if (m_cell_p == nullptr) {
		return;
	}
	Button::draw(handX, handY, true);

	ostringstream oss;
	switch(m_cell_p->getCellKind()) {
	case NORMAL:
		oss << "何もないマス";
		break;
	}
	DrawStringToHandle(m_x1, m_y1, oss.str().c_str(), BLACK, font);

	int fontSize = 0;
	GetFontStateToHandle(NULL, &fontSize, NULL, font);
	int indentSize = fontSize / 2;
	const Character* c = m_cell_p->getCharacter();
	if (c != nullptr) {
		DrawStringToHandle(m_x1 + indentSize, m_y1 + fontSize, m_cell_p->getCharacter()->getCharacterProfile()->getFullName().c_str(), WHITE, font);
		int hp = c->getCharacterStatus()->getHp();
		int dispHp = c->getCharacterStatus()->getDispHp();
		int maxHp = c->getCharacterStatus()->getMaxHp();
		drawHpBar(m_x1 + indentSize + applyEx(5, m_exX), m_y1 + fontSize * 2 + applyEx(5, m_exY), m_x1 + indentSize + applyEx(200, m_exX), m_y1 + fontSize * 2 + applyEx(15, m_exY), hp, dispHp, maxHp);
	}
}


/*
* スキル情報の領域
*/
SkillInfoButton::SkillInfoButton(int x1, int y1, int x2, int y2, const Skill* skill_p) :
	Button(x1, y1, x2, y2, 10, GRAY2, RED)
{
	m_skill_p = skill_p;
}


void SkillInfoButton::draw(int handX, int handY, const CharacterGraphs* characterGraphs, int font) const {
	if (m_skill_p == nullptr) {
		return;
	}
	Button::draw(handX, handY, true);

	DrawStringToHandle(m_x1, m_y1, m_skill_p->getSkillDesc().c_str(), BLACK, font);
}
