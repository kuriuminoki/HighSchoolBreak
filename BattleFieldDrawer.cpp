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
	m_middleFont = CreateFontToHandle(nullptr, applyEx(50, m_exX), 10);
	m_smallFont = CreateFontToHandle(nullptr, applyEx(20, m_exX), 7);

	m_cnt = 0;
}


BattleFieldDrawer::~BattleFieldDrawer() {
	delete m_characterGraphs;
	DeleteFontToHandle(m_font);
	DeleteFontToHandle(m_middleFont);
	DeleteFontToHandle(m_smallFont);
}


void BattleFieldDrawer::draw() {
	GetMousePoint(&m_handX, &m_handY);
	m_cnt++;

	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY, TRUE);

	vector<const Character*> dispCharacter;

	// �e�}�X�̕`��
	const vector<vector<Cell*> > cells = m_battleField_p->getCells();
	for (unsigned int y = 0; y < cells.size(); y++) {
		for (unsigned int x = 0; x < cells[y].size(); x++) {
			// �}�X�̕`��
			cells[y][x]->draw(m_handX, m_handY, true);
			// �}�X��ɂ���L����
			if (cells[y][x]->getCharacter() != nullptr) {
				dispCharacter.push_back(cells[y][x]->getCharacter());
			}
		}
	}

	// �e�L�����̕`��
	for (unsigned int i = 0; i < dispCharacter.size(); i++) {
		int handle = m_characterGraphs->getCharacterIconGraphs(dispCharacter[i]->getCharacterProfile()->getCharacterIconGraphNum());
		int x = dispCharacter[i]->getDispX();
		int y = dispCharacter[i]->getDispY();
		int wide = 0, height = 0;
		GetGraphSize(handle, &wide, &height);
		if (m_battleField_p->getActiveCharacter()->getCharacterProfile()->getId() == dispCharacter[i]->getCharacterProfile()->getId()
			&& m_cnt / 3 % 2 == 0) {
			SetDrawBright(100, 100, 100);
		}
		DrawRotaGraph(x, y - (int)(height * 0.1 / 4), 0.1, 0.0, handle, TRUE);
		SetDrawBright(255, 255, 255);
	}

	// �L�������̕`��
	const vector<CharacterInfoButton*> characterInfoButton = m_battleField_p->getCharacterInfoButton();
	for (unsigned int i = 0; i < characterInfoButton.size(); i++) {
		characterInfoButton[i]->draw(m_handX, m_handY, false, m_characterGraphs, m_smallFont);
	}

	// サイコロの描画
	m_battleField_p->getDice()->draw(m_handX, m_handY, m_font, BLACK);

	// �{�^���̕`��
	m_battleField_p->getEndActionButton()->draw(m_handX, m_handY, true, m_middleFont, BLACK);
}
