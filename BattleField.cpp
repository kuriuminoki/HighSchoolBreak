#include "BattleField.h"
#include "Button.h"
#include "Cell.h"
#include "Character.h"
#include "CharacterController.h"
#include "Control.h"
#include "Define.h"
#include "Dice.h"
#include "Skill.h"

#include <algorithm>
#include <queue>


using namespace std;


/*
* すごろくゲーム
*/
BattleField::BattleField() {
	double exX = 1.0, exY = 1.0;
	getGameEx(exX, exY);

	// マス
	m_columnSize = 20;
	m_rowSize = 10;
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

	// キャラ
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
		move(m_characters[i], y, x, m_cells, true, false);
	}

	m_dice = new Dice(GAME_WIDE - applyEx(350, exX), GAME_HEIGHT - applyEx(330, exY), GAME_WIDE - applyEx(50, exX), GAME_HEIGHT - applyEx(30, exY), applyEx(6, exX), LIGHT_YELLOW, RED);
	m_characterController = nullptr;
	m_activeCharacterIndex = 0;
	initController();

	m_cellInfoButton = new CellInfoButton(applyEx(900, exX), GAME_HEIGHT - INFO_HEIGHT - applyEx(30, exY), applyEx(1300, exX), GAME_HEIGHT - applyEx(30, exY), nullptr);
	m_skillInfoButton = new SkillInfoButton(applyEx(900, exX), GAME_HEIGHT - INFO_HEIGHT - applyEx(30, exY), applyEx(1300, exX), GAME_HEIGHT - applyEx(30, exY), nullptr);
	m_endActionButton = new TextButton("行動終了", applyEx(1350, exX), GAME_HEIGHT - applyEx(330, exY), applyEx(1550, exX), GAME_HEIGHT - applyEx(230, exY), applyEx(6, exX), LIGHT_RED, RED);

	m_alreadyAttack = false;
	m_hangingSkill_p = nullptr;
	m_hangingCharacterWithSkill_p = nullptr;
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
	delete m_cellInfoButton;
	delete m_skillInfoButton;
	delete m_endActionButton;
}


void BattleField::nextTurn() {
	m_activeCharacterIndex++;

	// キャラが一巡したとき
	if (m_activeCharacterIndex == (int)m_characters.size()) {
		m_activeCharacterIndex = 0;
		for (unsigned int y = 0; y < m_cells.size(); y++) {
			for (unsigned int x = 0; x < m_cells[y].size(); x++) {
				m_cells[y][x]->nextTurn();
			}
		}
	}

	initController();
	m_alreadyAttack = false;
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
		if (!m_alreadyAttack) {
			// 移動後の攻撃
			setDamageCell(m_characters[m_activeCharacterIndex]->getY(), m_characters[m_activeCharacterIndex]->getX(), m_characters[m_activeCharacterIndex]);
			damageCharacterEachCell();
			m_alreadyAttack = true;
		}
		if (m_cells[m_characters[m_activeCharacterIndex]->getY()][m_characters[m_activeCharacterIndex]->getX()]->getSkill() != nullptr) {
			// スキルの発火
			const Skill* skill = m_cells[m_characters[m_activeCharacterIndex]->getY()][m_characters[m_activeCharacterIndex]->getX()]->getSkill();
			COMMAND_TO_BF com = skill->fire(m_characters[m_activeCharacterIndex]->getY(), m_characters[m_activeCharacterIndex]->getX(), m_cells, m_characterController);
			switch (com) {
			case RETRY_MOVE:
				m_alreadyAttack = false;
				break;
			}
			m_cells[m_characters[m_activeCharacterIndex]->getY()][m_characters[m_activeCharacterIndex]->getX()]->setSkill(nullptr);
		}
		else {
			if (m_characters[m_activeCharacterIndex]->getGroupKind() == STUDENT) {
				m_endActionButton->on();
			}
			if (m_characters[m_activeCharacterIndex]->getGroupKind() != STUDENT || leftClick() == 1 && m_endActionButton->overlap(m_handX, m_handY)) {
				nextTurn();
			}
		}
	}

	// 各マスの処理
	int overlapY = -1, overlapX = -1;
	for (unsigned int y = 0; y < m_cells.size(); y++) {
		for (unsigned int x = 0; x < m_cells[y].size(); x++) {
			m_cells[y][x]->setDamageValue(0, STUDENT);
			if (m_cells[y][x]->overlap(m_handX, m_handY)) {
				overlapY = y;
				overlapX = x;
				// スキルを設置する
				if (m_hangingSkill_p != nullptr && leftClick() == 1 && m_cells[y][x]->ableSetSkill()) {
					for (unsigned int i = 0; i < m_characters.size(); i++) {
						if (m_characters[i] == m_hangingCharacterWithSkill_p) {
							m_characters[i]->addSkillPoint(-m_hangingSkill_p->getNeedSkillPoint());
						}
					}
					m_cells[y][x]->setSkill(m_hangingSkill_p);
					m_hangingSkill_p = nullptr;
				}
			}
			m_cells[y][x]->playAnimation();
		}
	}

	// カーソルが重なっているマスの情報を表示する
	if (overlapX >= 0 && overlapY >= 0) {
		m_cellInfoButton->setCell(m_cells[overlapY][overlapX]);
	}
	else {
		m_cellInfoButton->setCell(nullptr);
	}

	// カーソルが重なっているスキルの情報を表示する
	Skill* overlapSkill = nullptr;
	const Character* overlapCharacter = nullptr;
	for (unsigned int i = 0; i < m_characterInfoButton.size(); i++) {
		overlapSkill = m_characterInfoButton[i]->getOverlapSkill(m_handX, m_handY);
		overlapCharacter = m_characterInfoButton[i]->getCharacter();
		if (overlapSkill != nullptr) { break; }
	}
	m_skillInfoButton->setSkill(overlapSkill, overlapCharacter);
	// スキルを手に掴む
	if (overlapSkill != nullptr && leftClick() == 1 && overlapSkill->getNeedSkillPoint() <= overlapCharacter->getCharacterStatus()->getSkillPoint()) {
		if (m_hangingSkill_p == overlapSkill) {
			m_hangingSkill_p = nullptr;
		}
		else {
			m_hangingSkill_p = overlapSkill;
			m_hangingCharacterWithSkill_p = overlapCharacter;
		}
	}
	else if (m_hangingSkill_p != nullptr && leftClick() == 1) {
		m_hangingSkill_p = nullptr;
	}
	if (m_characterController->isWatingGoalSelect()) {
		m_hangingSkill_p = nullptr;
	}
	if (m_hangingSkill_p != nullptr && overlapSkill == nullptr) {
		m_skillInfoButton->setSkill(m_hangingSkill_p, m_hangingCharacterWithSkill_p);
	}

	// 攻撃範囲のガイドを設定
	if (!m_alreadyAttack && getActiveCharacter()->getGroupKind() == STUDENT && overlapY >= 0 && overlapX >= 0 && m_cells[overlapY][overlapX]->getMarkingColor() != -1) {
		setDamageCell(overlapY, overlapX, getActiveCharacter());
	}
	if (overlapY >= 0 && overlapX >= 0 && m_hangingSkill_p != nullptr) {
		m_hangingSkill_p->setDamageCell(overlapY, overlapX, m_cells);
	}
	else if (overlapY >= 0 && overlapX >= 0 && m_cells[overlapY][overlapX]->getSkill() != nullptr) {
		m_cells[overlapY][overlapX]->getSkill()->setDamageCell(overlapY, overlapX, m_cells);
	}

	// 各キャラの状態更新
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->updateDispHp();
		if (overlapCharacter == m_characters[i] && overlapSkill != nullptr) {
			m_characters[i]->setNeedSkillPoint(overlapSkill->getNeedSkillPoint());
		}
		else if (m_hangingCharacterWithSkill_p == m_characters[i] && m_hangingSkill_p != nullptr) {
			m_characters[i]->setNeedSkillPoint(m_hangingSkill_p->getNeedSkillPoint());
		}
		else {
			m_characters[i]->setNeedSkillPoint(0);
		}
	}

	return false;
}


void BattleField::setDamageCell(int y, int x, const Character* character_p) {
	if (y < 0 || x < 0 || y >= (int)m_cells.size() || x >= (int)m_cells[0].size()) {
		return;
	}
	const vector<pair<int, pair<int, int> > > targets = character_p->getAttackInfo()->getTargets();
	for (unsigned int i = 0; i < targets.size(); i++) {
		int ty = y + targets[i].second.first;
		int tx = x + targets[i].second.second;
		if (ty >= 0 && ty < m_cells.size() && tx >= 0 && tx < m_cells[0].size()) {
			m_cells[ty][tx]->setDamageValue(targets[i].first, character_p->getGroupKind());
		}
	}
}


void BattleField::damageCharacterEachCell() {
	for (unsigned int y = 0; y < m_cells.size(); y++) {
		for (unsigned int x = 0; x < m_cells[y].size(); x++) {
			m_cells[y][x]->damageCharacter();
		}
	}
}
