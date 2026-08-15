#include "BattleFieldDrawer.h"
#include "BattleField.h"
#include "Cell.h"
#include "Character.h"
#include "Define.h"
#include "Dice.h"
#include "DxLib.h"

#include <string>


using namespace std;


/*
* キャラの画像
*/
CharacterGraphs::CharacterGraphs() {
	string commonPath = "picture/icon/";
	for (int i = 0; i < 5; i++) {
		m_characterIconGraphs.push_back(-1);
	}
	m_characterIconGraphs[0] = LoadGraph((commonPath + "アカツキ.png").c_str());
	m_characterIconGraphs[1] = LoadGraph((commonPath + "トウノ.png").c_str());
	m_characterIconGraphs[2] = LoadGraph((commonPath + "タキノ.png").c_str());
	m_characterIconGraphs[3] = LoadGraph((commonPath + "カイバラ.png").c_str());
	m_characterIconGraphs[4] = LoadGraph((commonPath + "カンエイ.png").c_str());
}


CharacterGraphs::~CharacterGraphs() {

}


int CharacterGraphs::getCharacterIconGraphs(CHARACTER_NUM characterNum) const {
	return m_characterIconGraphs[characterNum];
}


/*
* BattleFieldのDrawer
*/
BattleFieldDrawer::BattleFieldDrawer(BattleField* battleField_p) {
	m_battleField_p = battleField_p;
	getGameEx(m_exX, m_exY);
	m_characterGraphs = new CharacterGraphs();
	m_font = CreateFontToHandle(nullptr, applyEx(100, m_exX), 50);
}


BattleFieldDrawer::~BattleFieldDrawer() {
	delete m_characterGraphs;
	DeleteFontToHandle(m_font);
}


void BattleFieldDrawer::draw() {
	GetMousePoint(&m_handX, &m_handY);

	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY, TRUE);

	// 各マスの描画
	const vector<vector<Cell*> > cells = m_battleField_p->getCells();
	for (unsigned int i = 0; i < cells.size(); i++) {
		for (unsigned int j = 0; j < cells[i].size(); j++) {
			// マスの描画
			cells[i][j]->draw(m_handX, m_handY, true);
			// マス上にいるキャラ
			if (cells[i][j]->getCharacter() != nullptr) {
				int handle = m_characterGraphs->getCharacterIconGraphs(cells[i][j]->getCharacter()->getCharacterProfile()->getCharacterIconGraphNum());
				int x = (cells[i][j]->getX1() + cells[i][j]->getX2()) / 2;
				int y = (cells[i][j]->getY1() + cells[i][j]->getY2()) / 2;
				int wide = 0, height = 0;
				GetGraphSize(handle, &wide, &height);
				DrawRotaGraph(x, cells[i][j]->getY2() - (int)(height * 0.1 / 2), 0.1, 0.0, handle, TRUE);
			}
		}
	}

	// キャラ情報の描画
	const vector<CharacterInfoButton*> characterInfoButton = m_battleField_p->getCharacterInfoButton();
	for (unsigned int i = 0; i < characterInfoButton.size(); i++) {
		characterInfoButton[i]->draw(m_handX, m_handY, false, m_characterGraphs);
	}

	// サイコロの描画
	m_battleField_p->getDice()->draw(m_handX, m_handY, m_font, BLACK);
}
