#include "BattleField.h"
#include "Button.h"
#include "Cell.h"
#include "Character.h"
#include "Control.h"
#include "Define.h"
#include "Dice.h"


using namespace std;


/*
* すごろくゲーム
*/
BattleField::BattleField() {

	m_columnSize = 20;
	m_rowSize = 10;
	double exX = 1.0, exY = 1.0;
	getGameEx(exX, exY);
	m_cells.assign(m_rowSize, vector<Cell*>(m_columnSize, nullptr));
	const int START_X = applyEx(30, exX);
	const int START_Y = applyEx(70, exY);
	const int EDGE = applyEx(10, exX);
	for (int i = 0; i < m_rowSize; i++) {
		for (int j = 0; j < m_columnSize; j++) {
			int x1 = START_X + applyEx(j * (CELL_WIDE + CELL_DISTANCE), exX);
			int y1 = START_Y + applyEx(i * (CELL_HEIGHT + CELL_DISTANCE), exY);
			m_cells[i][j] = new Cell(NORMAL, x1, y1, x1 + applyEx(CELL_WIDE, exX), y1 + applyEx(CELL_HEIGHT, exY), EDGE, WHITE, RED);
		}
	}

	int characterSize = 6;
	const char* lastNames[] = { "アカツキ", "トウノ", "タキノ", "モンスター", "モンスター", "モンスター" };
	const char* firstNames[] = { "リョウヤ", "ヒナミ", "エイリ", "A", "B", "C" };
	int infoNow = 0;
	const int INFO_WIDE = applyEx(300, exX);
	const int INFO_HEIGHT = applyEx(360, exX);
	for (int i = 0; i < characterSize; i++) {
		CharacterProfile* profile = new CharacterProfile(lastNames[i], firstNames[i]);
		CharacterStatus* status = new CharacterStatus();
		int x = i > 2 ? 17 + (i % 3) : i % 3;
		int y = i > 2 ? 9 : 0;
		GROUP_KIND groupKind = i > 2 ? MONSTER : STUDENT;
		m_characters.push_back(new Character(profile, status, x, y, groupKind));
		m_cells[y][x]->setCharacter(m_characters[i]);
		if (groupKind == STUDENT) {
			int x1 = applyEx(30, exX) + (INFO_WIDE + applyEx(30, exX)) * i;
			int y1 = GAME_HEIGHT - INFO_HEIGHT - applyEx(30, exY);
			m_characterInfoButton.push_back(new CharacterInfoButton(x1, y1, x1 + INFO_WIDE, y1 + INFO_HEIGHT, m_characters[i]));
			infoNow++;
		}
	}

	m_turnNum = 0;
	m_activeCharacterIndex = 0;
	m_dice = new Dice(GAME_WIDE - applyEx(350, exX), GAME_HEIGHT - applyEx(330, exY), GAME_WIDE - applyEx(50, exX), GAME_HEIGHT - applyEx(30, exY), applyEx(6, exX), LIGHT_YELLOW, RED);
}


BattleField::~BattleField() {
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		delete m_characters[i];
	}
	for (int i = 0; i < m_rowSize; i++) {
		for (int j = 0; j < m_columnSize; j++) {
			delete m_cells[i][j];
		}
	}
	for (unsigned int i = 0; i < m_characterInfoButton.size(); i++) {
		delete m_characterInfoButton[i];
	}
	delete m_dice;
}


bool BattleField::play() {
	GetMousePoint(&m_handX, &m_handY);
	if (leftClick() == 1) {
		if (m_dice->overlap(m_handX, m_handY)) {
			m_dice->init(1, 6, 60);
		}
	}
	m_dice->play();
	return false;
}

