#include "Button.h"
#include "BattleFieldDrawer.h"
#include "Character.h"
#include "Define.h"
#include "DxLib.h"



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
}


void Button::draw(int handX, int handY, bool fill) const {
	if (fill) {
		if (overlap(handX, handY)) {
			DrawBox(m_x1 - m_edgeLength, m_y1 - m_edgeLength, m_x2 + m_edgeLength, m_y2 + m_edgeLength, m_edgeColor, TRUE);
		}
		DrawBox(m_x1, m_y1, m_x2, m_y2, m_innerColor, TRUE);
	}
	else {
		if (overlap(handX, handY)) {
			DrawBox(m_x1, m_y1, m_x2, m_y2, m_edgeColor, fill);
		}
		else {
			DrawBox(m_x1, m_y1, m_x2, m_y2, m_innerColor, fill);
		}
	}
}


/*
* キャラ情報の領域
*/
CharacterInfoButton::CharacterInfoButton(int x1, int y1, int x2, int y2, const Character* character_p):
	Button(x1, y1, x2, y2, 10, LIGHT_BLUE, BLUE)
{
	m_character_p = character_p;
}

void CharacterInfoButton::draw(int handX, int handY, bool fill, const CharacterGraphs* characterGraphs) const {
	if (m_character_p == nullptr) {
		return;
	}
	Button::draw(handX, handY, true);
	int wide = 0, height = 0;
	int handle = characterGraphs->getCharacterIconGraphs(m_character_p->getCharacterProfile()->getCharacterIconGraphNum());
	DrawRotaGraph((m_x1 + m_x2) / 2, (m_y1 + m_y2) / 2, 0.3, 0.0, handle, TRUE);
}
