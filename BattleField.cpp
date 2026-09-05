#include "BattleField.h"
#include "Button.h"
#include "Cell.h"
#include "Character.h"
#include "CharacterController.h"
#include "Control.h"
#include "Define.h"
#include "Dice.h"
#include "Graphs.h"
#include "Skill.h"
#include "Sound.h"

#include <algorithm>
#include <queue>


using namespace std;


/*
* BattleField用の音
*/
BattleFieldSoundHandle::BattleFieldSoundHandle() {
	m_decideSound = LoadSoundMem("sound/battleField/decide.mp3");
	m_overlapSound = LoadSoundMem("sound/battleField/overlap.mp3");
	m_skillPlaySound = LoadSoundMem("sound/battleField/スキル発動.wav");
	m_skillDescSound = LoadSoundMem("sound/battleField/スキル説明.wav");
	m_specialDescSound = LoadSoundMem("sound/battleField/必殺技説明.wav");
	m_hangingSound = LoadSoundMem("sound/battleField/hanging.mp3");
	m_setSkillSound = LoadSoundMem("sound/battleField/setSkill.mp3");
	m_moveSound = LoadSoundMem("sound/battleField/move.mp3");
}


BattleFieldSoundHandle::~BattleFieldSoundHandle() {
	DeleteSoundMem(m_decideSound);
	DeleteSoundMem(m_overlapSound);
	DeleteSoundMem(m_skillPlaySound);
	DeleteSoundMem(m_skillDescSound);
	DeleteSoundMem(m_specialDescSound);
	DeleteSoundMem(m_hangingSound);
	DeleteSoundMem(m_setSkillSound);
	DeleteSoundMem(m_moveSound);
}


/*
* スキル発動処理
*/
SkillPlayer::SkillPlayer(bool isSpecial, CharacterController* userController, SoundPlayer* soundPlayer_p, const BattleFieldSoundHandle* soundHandle_p) {
	m_isSpecial = isSpecial;
	m_userController_p = userController;
	m_soundPlayer_p = soundPlayer_p;
	m_soundHandle_p = soundHandle_p;

	m_skill_p = nullptr;
	m_skillOwner_p = nullptr;
	m_cnt = 0;
	m_x = m_userController_p->getCharacter()->getX();
	m_y = m_userController_p->getCharacter()->getY();
	m_turn = 0;
}


COMMAND_TO_BF SkillPlayer::play(vector<vector<Cell*> >& cells) {
	if (m_cnt == 0) {
		if (m_isSpecial) {
			m_skill_p = cells[m_y][m_x]->getCharacter()->getSpecialSkill();
			m_skillOwner_p = cells[m_y][m_x]->getCharacter();
		}
		else {
			m_skill_p = cells[m_y][m_x]->getSkill();
			m_skillOwner_p = cells[m_y][m_x]->getSkillOwner();
			m_turn = cells[m_y][m_x]->getSkillTurn();
		}
		m_soundPlayer_p->pushSoundQueue(m_soundHandle_p->getSkillPlaySound());
		cells[m_y][m_x]->setAnimation(SKILL_EFFECT, 6);
	}
	m_cnt++;

	if (m_cnt < PRE_PLAY_TIME) {

	}
	else if (m_cnt < PRE_PLAY_TIME + SKILL_GUIDE_TIME) {
		// スキルの説明を表示中
		if (m_cnt == PRE_PLAY_TIME) {
			m_soundPlayer_p->pushSoundQueue(m_soundHandle_p->getSkillDescSound());
		}
	}
	else {
		// スキルの発火
		COMMAND_TO_BF com = m_skill_p->fire(m_y, m_x, cells, m_userController_p, m_skillOwner_p);
		cells[m_y][m_x]->setSkill(nullptr, nullptr);
		return com;
	}
	return SKILL_PLAY_CONTINUE;
}


/*
* すごろくゲーム
*/
BattleField::BattleField(SoundPlayer* soundPlayer_p) {
	m_soundPlayer_p = soundPlayer_p;

	m_soundHandle = new BattleFieldSoundHandle();

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
		move(m_characters[i], y, x, m_cells, true, false, m_soundPlayer_p, m_soundHandle);
	}
	for (unsigned int i = 0; i < m_characterInfoButton.size(); i++) {
		m_characterInfoButton[i]->updateCharacterInfo();
	}

	m_dice = new Dice(GAME_WIDE - applyEx(350, exX), GAME_HEIGHT - applyEx(330, exY), GAME_WIDE - applyEx(50, exX), GAME_HEIGHT - applyEx(30, exY), applyEx(6, exX), LIGHT_YELLOW, RED);
	m_characterController = nullptr;
	m_activeCharacterIndex = 0;
	initController();

	m_cellInfoButton = new CellInfoButton(applyEx(900, exX), GAME_HEIGHT - INFO_HEIGHT - applyEx(30, exY), applyEx(1300, exX), GAME_HEIGHT - applyEx(30, exY), nullptr);
	m_skillInfoButton = new SkillInfoButton(applyEx(900, exX), GAME_HEIGHT - INFO_HEIGHT - applyEx(30, exY), applyEx(1300, exX), GAME_HEIGHT - applyEx(30, exY), nullptr);
	m_endActionButton = new TextButton("行動終了", applyEx(1350, exX), GAME_HEIGHT - applyEx(330, exY), applyEx(1550, exX), GAME_HEIGHT - applyEx(230, exY), applyEx(6, exX), LIGHT_RED, RED);

	m_alreadyAttack = false;
	m_hangingSkill_pair = make_pair(nullptr, nullptr);
	m_skillPlayer = nullptr;
	m_freezeTime = 0;
}


BattleField::~BattleField() {
	delete m_soundHandle;
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
	if (m_skillPlayer != nullptr) {
		delete m_skillPlayer;
	}
}


void BattleField::initCells() {
	for (unsigned int y = 0; y < m_cells.size(); y++) {
		for (unsigned int x = 0; x < m_cells[y].size(); x++) {
			m_cells[y][x]->setDamageValue(0, STUDENT);
		}
	}
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
		for (unsigned int i = 0; i < m_characters.size(); i++) {
			m_characters[i]->nextTurn();
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
		m_characterController = new StudentController(m_dice, m_soundPlayer_p, m_soundHandle);
		break;
	default:
		m_characterController = new EnemyController(m_dice, m_soundPlayer_p, m_soundHandle);
		break;
	}
	m_characterController->setCharacter(m_characters[m_activeCharacterIndex]);
	m_characterController->initControl();
	m_dice->on();
}


bool BattleField::play() {
	GetMousePoint(&m_handX, &m_handY);
	m_endActionButton->off(DARK_RED);

	// 各マスのダメージリセット (最初にやらないとスキルのガイド用に付けていたダメージが適用される分二重になる)
	initCells();

	playCharacterMove();

	updateBattleField();

	m_soundPlayer_p->play();

	return false;
}


void BattleField::playCharacterMove() {
	if (m_freezeTime > 0) {
		m_freezeTime--;
	}
	else if (m_skillPlayer != nullptr) {
		// スキル・必殺技の発動中
		COMMAND_TO_BF ctb = m_skillPlayer->play(m_cells);
		switch (ctb) {
		case RETRY_MOVE:
			m_alreadyAttack = false;
			break;
		}
		if (ctb != SKILL_PLAY_CONTINUE) {
			delete m_skillPlayer;
			m_skillPlayer = nullptr;
		}
		initCells();
	}
	else if (m_characterController->play(m_handX, m_handY, m_cells)) {
		// キャラの操作
		Character* activeCharacter = m_characters[m_activeCharacterIndex];
		if (!m_alreadyAttack) {
			// 移動後の攻撃
			setDamageCell(activeCharacter->getY(), activeCharacter->getX(), activeCharacter);
			damageCharacterEachCell();
			m_alreadyAttack = true;
			initCells();
		}
		if (m_cells[activeCharacter->getY()][activeCharacter->getX()]->getSkill() != nullptr) {
			// スキルの発火
			m_skillPlayer = new SkillPlayer(false, m_characterController, m_soundPlayer_p, m_soundHandle);
		}
		else {
			if (activeCharacter->getGroupKind() == STUDENT) {
				m_endActionButton->on();
			}
			if (activeCharacter->getGroupKind() != STUDENT || leftClick() == 1 && m_endActionButton->overlap(m_handX, m_handY)) {
				nextTurn();
				m_soundPlayer_p->pushSoundQueue(m_soundHandle->getDecideSound());
			}
		}
	}
}


void BattleField::updateBattleField() {
	// 各マスの処理
	int overlapY = -1, overlapX = -1;
	for (unsigned int y = 0; y < m_cells.size(); y++) {
		for (unsigned int x = 0; x < m_cells[y].size(); x++) {
			if (m_cells[y][x]->overlap(m_handX, m_handY)) {
				overlapY = y;
				overlapX = x;
				// スキルを設置する
				if (m_hangingSkill_pair.first != nullptr && leftClick() == 1 && m_cells[y][x]->ableSetSkill() && !m_characterController->isWatingGoalSelect()) {
					for (unsigned int i = 0; i < m_characters.size(); i++) {
						if (m_characters[i] == m_hangingSkill_pair.second) {
							m_characters[i]->addSkillPoint(-m_hangingSkill_pair.second->getNeedSkillPoint());
						}
					}
					for (unsigned int i = 0; i < m_characterInfoButton.size(); i++) {
						m_characterInfoButton[i]->updateCharacterInfo();
					}
					m_cells[y][x]->setSkill(m_hangingSkill_pair.first, m_hangingSkill_pair.second);
					m_hangingSkill_pair = make_pair(nullptr, nullptr);
					m_soundPlayer_p->pushSoundQueue(m_soundHandle->getSetSkillSound());
				}
			}
			m_cells[y][x]->playAnimation();
		}
	}

	// カーソルが重なっているマスの情報を表示する
	if (overlapX >= 0 && overlapY >= 0) {
		if (m_cellInfoButton->getCell() != m_cells[overlapY][overlapX]) {
			m_soundPlayer_p->pushSoundQueue(m_soundHandle->getOverlapSound());
		}
		m_cellInfoButton->setCell(m_cells[overlapY][overlapX]);
	}
	else {
		m_cellInfoButton->setCell(nullptr);
	}

	// カーソルが重なっているスキルを特定
	pair<Skill*, const Character*> overlapSkill_pair = make_pair(nullptr, nullptr);
	bool isSpecial = false;
	for (unsigned int i = 0; i < m_characterInfoButton.size() && overlapSkill_pair.first == nullptr; i++) {
		Skill* overlapSkill = m_characterInfoButton[i]->getOverlapSkill(m_handX, m_handY);
		if (overlapSkill != nullptr) {
			overlapSkill_pair = make_pair(overlapSkill, m_characterInfoButton[i]->getCharacter());
		}
		Skill* specialSkill = m_characterInfoButton[i]->getOverlapSpecial(m_handX, m_handY);
		if (overlapSkill_pair.first == nullptr && specialSkill != nullptr) {
			overlapSkill_pair = make_pair(specialSkill, m_characterInfoButton[i]->getCharacter());
			isSpecial = true;
			// 必殺技発動
			if (leftClick() == 1 && ableSpecialSkill(overlapSkill_pair.second)) {
				for (unsigned int i = 0; i < m_characters.size(); i++) {
					if (m_characters[i] == overlapSkill_pair.second) {
						m_characters[i]->addSpecialPoint(-m_characters[i]->getCharacterStatus()->getMaxSpecialPoint());
						m_characterInfoButton[i]->updateCharacterInfo();
						break;
					}
				}
				m_skillPlayer = new SkillPlayer(true, m_characterController, m_soundPlayer_p, m_soundHandle);
			}
		}
	}
	// スキルを手に掴む
	if (overlapSkill_pair.first != nullptr && !isSpecial && leftClick() == 1 && overlapSkill_pair.first->getNeedSkillPoint() <= overlapSkill_pair.second->getCharacterStatus()->getSkillPoint() && !m_characterController->isWatingGoalSelect()) {
		if (m_hangingSkill_pair.first == overlapSkill_pair.first) {
			m_hangingSkill_pair = make_pair(nullptr, nullptr);
			m_soundPlayer_p->pushSoundQueue(m_soundHandle->getDecideSound());
		}
		else {
			m_hangingSkill_pair = make_pair(overlapSkill_pair.first, overlapSkill_pair.second);
			m_soundPlayer_p->pushSoundQueue(m_soundHandle->getHangingSound());
		}
	}
	else if (m_hangingSkill_pair.first != nullptr && (leftClick() == 1 || (m_characterController->isWatingGoalSelect()))) {
		m_hangingSkill_pair = make_pair(nullptr, nullptr);
		m_soundPlayer_p->pushSoundQueue(m_soundHandle->getDecideSound());
	}
	// スキルの情報を表示する
	if (m_hangingSkill_pair.first != nullptr && overlapSkill_pair.first == nullptr && m_skillInfoButton->getSkill() != m_hangingSkill_pair.first) {
		// 持っているスキルを表示するパターン
		m_skillInfoButton->setSkill(m_hangingSkill_pair.first, m_hangingSkill_pair.second);
	}
	else if (overlapSkill_pair.first != nullptr && m_skillInfoButton->getSkill() != overlapSkill_pair.first) {
		// 重なっているスキルを表示するパターン
		m_soundPlayer_p->pushSoundQueue(m_soundHandle->getOverlapSound());
		m_skillInfoButton->setSkill(overlapSkill_pair.first, overlapSkill_pair.second);
	}
	else if (m_hangingSkill_pair.first == nullptr && overlapSkill_pair.first == nullptr) {
		m_skillInfoButton->setSkill(nullptr, nullptr);
	}

	// 攻撃範囲のガイドを設定
	if (!m_alreadyAttack && getActiveCharacter()->getGroupKind() == STUDENT && overlapY >= 0 && overlapX >= 0 && m_cells[overlapY][overlapX]->getMarkingColor() != -1) {
		// キャラの移動先
		setDamageCell(overlapY, overlapX, getActiveCharacter());
	}
	if (overlapY >= 0 && overlapX >= 0 && m_hangingSkill_pair.first != nullptr) {
		// 設置しようとしているスキル
		m_hangingSkill_pair.first->setDamageCell(overlapY, overlapX, m_cells);
	}
	else if (overlapY >= 0 && overlapX >= 0 && m_cells[overlapY][overlapX]->getSkill() != nullptr) {
		// カーソルが重なっているスキル
		m_cells[overlapY][overlapX]->getSkill()->setDamageCell(overlapY, overlapX, m_cells);
	}
	else if (overlapY >= 0 && overlapX >= 0 && m_cells[overlapY][overlapX]->getCharacter() != nullptr) {
		// カーソルが重なっているキャラ
		setDamageCell(overlapY, overlapX, m_cells[overlapY][overlapX]->getCharacter());
	}

	// 各キャラの状態更新
	for (unsigned int i = 0; i < m_characters.size(); i++) {
		m_characters[i]->updateDispHp();
		if (overlapSkill_pair.second == m_characters[i] && overlapSkill_pair.first != nullptr) {
			m_characters[i]->setNeedSkillPoint(overlapSkill_pair.first->getNeedSkillPoint());
		}
		else if (m_hangingSkill_pair.second == m_characters[i] && m_hangingSkill_pair.first != nullptr) {
			m_characters[i]->setNeedSkillPoint(m_hangingSkill_pair.first->getNeedSkillPoint());
		}
		else {
			m_characters[i]->setNeedSkillPoint(0);
		}
	}
}


void BattleField::setDamageCell(int y, int x, const Character* character_p) {
	if (y < 0 || x < 0 || y >= (int)m_cells.size() || x >= (int)m_cells[0].size()) {
		return;
	}
	const vector<pair<int, pair<int, int> > > targets = character_p->getAttackInfo()->getTargets();
	int attackBuffValue = character_p->calcAttackBuffValue();
	for (unsigned int i = 0; i < targets.size(); i++) {
		int ty = y + targets[i].second.first;
		int tx = x + targets[i].second.second;
		if (ty >= 0 && ty < m_cells.size() && tx >= 0 && tx < m_cells[0].size()) {
			int attackValue = targets[i].first;
			if (attackValue > 0) { // 回復にバフ・デバフはかからない
				attackValue += attackBuffValue;
				if (attackValue < 0) {
					attackValue = 1; // 攻撃がデバフによって回復になることはない
				}
			}
			m_cells[ty][tx]->setDamageValue(attackValue, character_p->getGroupKind());
		}
	}
}


void BattleField::damageCharacterEachCell() {
	for (unsigned int y = 0; y < m_cells.size(); y++) {
		for (unsigned int x = 0; x < m_cells[y].size(); x++) {
			if (m_cells[y][x]->damageCharacter()) {
				m_freezeTime = ATTACK_FREEZE_TIME;
			}
		}
	}
}


bool BattleField::ableSpecialSkill(const Character* character) {
	if (m_skillPlayer != nullptr) {
		return false;
	}
	if (m_characterController->isWatingGoalSelect() || m_characterController->getCharacter() != character) {
		return false;
	}
	if (character->getCharacterStatus()->getSpecialPoint() < character->getCharacterStatus()->getMaxSpecialPoint()) {
		return false;
	}
	return true;
}
