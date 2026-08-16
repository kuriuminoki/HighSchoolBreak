#include "BattleField.h"
#include "Button.h"
#include "Cell.h"
#include "Character.h"
#include "CharacterController.h"
#include "Control.h"
#include "Define.h"
#include "Dice.h"

#include <algorithm>
#include <queue>


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
	const int INFO_WIDE = applyEx(250, exX);
	const int INFO_HEIGHT = applyEx(360, exX);
	for (int i = 0; i < characterSize; i++) {
		CharacterProfile* profile = new CharacterProfile(i, lastNames[i], firstNames[i]);
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
		move(m_characters[i], y, x, m_cells, true);
	}

	m_dice = new Dice(GAME_WIDE - applyEx(350, exX), GAME_HEIGHT - applyEx(330, exY), GAME_WIDE - applyEx(50, exX), GAME_HEIGHT - applyEx(30, exY), applyEx(6, exX), LIGHT_YELLOW, RED);
	m_characterController = nullptr;
	m_activeCharacterIndex = 0;
	initController();

	m_endActionButton = new TextButton("行動終了", applyEx(1300, exX), GAME_HEIGHT - applyEx(330, exY), applyEx(1500, exX), GAME_HEIGHT - applyEx(230, exY), applyEx(6, exX), LIGHT_RED, RED);
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
	delete m_characterController;
	delete m_endActionButton;
}


void BattleField::initController() {
	if (m_characterController != nullptr) {
		delete m_characterController;
	}
	switch (m_characters[m_activeCharacterIndex]->getGroupKind()) {
	case STUDENT:
		m_characterController = new StudentController(m_dice);
		break;
	default:
		m_characterController = new EnemyController(m_dice);
		break;
	}
	m_characterController->setCharacter(m_characters[m_activeCharacterIndex]);
	m_characterController->initControl();
	m_dice->on();
}


bool BattleField::play() {
	GetMousePoint(&m_handX, &m_handY);
	m_endActionButton->off(DARK_RED);

	// キャラの操作
	if (m_characterController->play(m_handX, m_handY, m_cells)) {
		m_endActionButton->on();
		if (m_characters[m_activeCharacterIndex]->getGroupKind() != STUDENT || leftClick() == 1 && m_endActionButton->overlap(m_handX, m_handY)) {
			m_activeCharacterIndex++;
			m_activeCharacterIndex %= (int)m_characters.size();
			initController();
		}
	}

	// 各マスの初期化
	for (unsigned int y = 0; y < m_cells.size(); y++) {
		for (unsigned int x = 0; x < m_cells[y].size(); x++) {
			m_cells[y][x]->setDamageValue(0);
		}
	}

	// 攻撃範囲を設定
	if (getActiveCharacter()->getGroupKind() == STUDENT) {
		const vector<pair<int, pair<int, int> > > targets = getActiveCharacter()->getAttackInfo()->getTargets();
		for (unsigned int y = 0; y < m_cells.size(); y++) {
			for (unsigned int x = 0; x < m_cells[y].size(); x++) {
				if (!m_cells[y][x]->overlap(m_handX, m_handY) || m_cells[y][x]->getMarkingColor() == -1) {
					continue;
				}
				for (unsigned int i = 0; i < targets.size(); i++) {
					int ty = y + targets[i].second.first;
					int tx = x + targets[i].second.second;
					if (ty >= 0 && ty < m_cells.size() && tx >= 0 && tx < m_cells[0].size()) {
						m_cells[ty][tx]->setDamageValue(targets[i].first);
					}
				}
			}
		}
	}

	return false;
}

