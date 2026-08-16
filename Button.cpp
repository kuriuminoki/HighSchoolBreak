#include "Button.h"
#include "BattleFieldDrawer.h"
#include "Character.h"
#include "Define.h"
#include "DxLib.h"


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
* キャラ情報の領域
*/
CharacterInfoButton::CharacterInfoButton(int x1, int y1, int x2, int y2, const Character* character_p):
	Button(x1, y1, x2, y2, 10, LIGHT_BLUE, BLUE)
{
	m_character_p = character_p;
}

void CharacterInfoButton::draw(int handX, int handY, bool fill, const CharacterGraphs* characterGraphs, int font) const {
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
}
