#include "BattleField.h"
#include "Button.h"
#include "Cell.h"
#include "Character.h"
#include "Control.h"
#include "Define.h"
#include "Dice.h"

#include <algorithm>
#include <queue>


using namespace std;


// キャラの移動 (warp=trueなら瞬間移動)
bool move(Character* character_p, int gy, int gx, vector<vector<Cell*> >& cells, bool warp) {
	int goalDispX = (cells[gy][gx]->getX1() + cells[gy][gx]->getX2()) / 2;
	int goalDispY = (cells[gy][gx]->getY1() + cells[gy][gx]->getY2()) / 2;

	if (character_p->getX() != gx || character_p->getY() != gy) {
		cells[character_p->getY()][character_p->getX()]->setCharacter(nullptr);
		cells[gy][gx]->setCharacter(character_p);
		character_p->setY(gy);
		character_p->setX(gx);
	}

	if (warp) {
		character_p->setDispY(goalDispY);
		character_p->setDispX(goalDispX);
		return true;
	}
	else {
		int moveSpeedY = character_p->getDispY() < goalDispY ? 10 : -10;
		int moveSpeedX = character_p->getDispX() < goalDispX ? 10 : -10;

		if (character_p->getDispY() != goalDispY) {
			if (abs(character_p->getDispY() - goalDispY) < abs(moveSpeedY)) {
				character_p->setDispY(goalDispY);
			}
			else {
				character_p->setDispY(character_p->getDispY() + moveSpeedY);
			}
		}
		if (character_p->getDispX() != goalDispX) {
			if (abs(character_p->getDispX() - goalDispX) < abs(moveSpeedX)) {
				character_p->setDispX(goalDispX);
			}
			else {
				character_p->setDispX(character_p->getDispX() + moveSpeedX);
			}
		}

		if (character_p->getDispY() == goalDispY && character_p->getDispX() == goalDispX) {
			return true;
		}
	}
	return false;
}


/*
* キャラ操作
*/
CharacterController::CharacterController(Dice* dice_p) {
	m_dice_p = dice_p;
	m_ableFinish = false;
}


void CharacterController::initControl() {
	m_ableFinish = false;
	m_dice_p->on();
	for (unsigned int i = 0; i < m_routeMemo.size(); i++) {
		m_routeMemo[i].clear();
	}
	m_routeMemo.clear();
	m_track.clear();
}


bool CharacterController::play(int handX, int handY, std::vector<std::vector<Cell*> >& cells) {
	if (leftClick() == 1) {
		if (m_dice_p->overlap(handX, handY)) {
			m_dice_p->init(1, 6, 60);
			m_dice_p->off(DARK_YELLOW);
		}
	}
	if (!m_dice_p->getAbleClick() && !m_ableFinish) {
		if (m_dice_p->play() && m_routeMemo.empty()) {
			// 幅優先探索で各マスへの最短ルートを検索
			searchAllTrack(m_dice_p->getValue(), cells);
		}
		else if (leftClick() == 1 && m_track.empty() && !m_routeMemo.empty()) {
			// 目標地点までの経路を設定
			for (unsigned int y = 0; y < cells.size(); y++) {
				for (unsigned int x = 0; x < cells[y].size(); x++) {
					if (cells[y][x]->overlap(handX, handY) && cells[y][x]->getMarkingColor() != -1) {
						searchGoalRoute(y, x, cells);
					}
				}
			}
		}
	}

	if (!m_track.empty()) {
		if (move(m_character_p, m_track[m_track.size() - 1].first, m_track[m_track.size() - 1].second, cells, false)) {
			m_track.pop_back();
		}
		if (m_track.empty()) {
			for (unsigned int y = 0; y < cells.size(); y++) {
				for (unsigned int x = 0; x < cells[y].size(); x++) {
					cells[y][x]->setMarkingColor(-1);
				}
			}
			m_ableFinish = true;
		}
	}

	if (m_ableFinish) {
		return true; // 行動終了できる
	}

	return false;
}


void CharacterController::searchAllTrack(int maxDistance, std::vector<std::vector<Cell*> >& cells) {
	m_routeMemo.assign(cells.size(), vector<DIRECTION>(cells[0].size(), LEFT));

	queue<pair<int, pair<int, int> > > que; // 距離、YX座標
	que.push(make_pair(0, make_pair(m_character_p->getY(), m_character_p->getX())));
	while (!que.empty()) {
		pair<int, pair<int, int> > distance = que.front();
		que.pop();
		int d = distance.first;
		if (d > maxDistance) {
			continue;
		}
		int y = distance.second.first;
		int x = distance.second.second;
		cells[y][x]->setMarkingColor(LIGHT_RED);
		if (x > 0 && cells[y][x - 1]->getMarkingColor() == -1 && cells[y][x - 1]->ableMoving()) {
			que.push(make_pair(d + 1, make_pair(y, x - 1)));
			m_routeMemo[y][x - 1] = LEFT;
		}
		if (y > 0 && cells[y - 1][x]->getMarkingColor() == -1 && cells[y - 1][x]->ableMoving()) {
			que.push(make_pair(d + 1, make_pair(y - 1, x)));
			m_routeMemo[y - 1][x] = UP;
		}
		if (x < (int)cells[0].size() - 1 && cells[y][x + 1]->getMarkingColor() == -1 && cells[y][x + 1]->ableMoving()) {
			que.push(make_pair(d + 1, make_pair(y, x + 1)));
			m_routeMemo[y][x + 1] = RIGHT;
		}
		if (y < (int)cells.size() - 1 && cells[y + 1][x]->getMarkingColor() == -1 && cells[y + 1][x]->ableMoving()) {
			que.push(make_pair(d + 1, make_pair(y + 1, x)));
			m_routeMemo[y + 1][x] = DOWN;
		}
	}
}


void CharacterController::searchGoalRoute(int gy, int gx, vector<vector<Cell*> >& cells) {
	m_track.clear();
	int sy = m_character_p->getY();
	int sx = m_character_p->getX();
	while (sy != gy || sx != gx) {
		m_track.push_back(make_pair(gy, gx));
		switch (m_routeMemo[gy][gx]) {
		case LEFT:
			gx++;
			break;
		case RIGHT:
			gx--;
			break;
		case UP:
			gy++;
			break;
		case DOWN:
			gy--;
			break;
		}
	}
	//reverse(m_track.begin(), m_track.end());

	// お掃除
	for (unsigned int i = 0; i < m_routeMemo.size(); i++) {
		m_routeMemo[i].clear();
	}
	m_routeMemo.clear();
}


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
	m_characterController = new CharacterController(m_dice);
	m_activeCharacterIndex = 0;
	m_characterController->setCharacter(m_characters[m_activeCharacterIndex]);

	m_endActionButton = new TextButton("行動終了", applyEx(1200, exX), GAME_HEIGHT - applyEx(330, exY), applyEx(1400, exX), GAME_HEIGHT - applyEx(230, exY), applyEx(6, exX), LIGHT_RED, RED);
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
	delete m_endActionButton;
}


bool BattleField::play() {
	GetMousePoint(&m_handX, &m_handY);

	if (m_characters[m_activeCharacterIndex]->getGroupKind() == STUDENT) {
		if (m_characterController->play(m_handX, m_handY, m_cells)) {
			if (leftClick() == 1 && m_endActionButton->overlap(m_handX, m_handY)) {
				m_activeCharacterIndex++;
				m_activeCharacterIndex %= (int)m_characters.size();
				m_characterController->setCharacter(m_characters[m_activeCharacterIndex]);
				m_characterController->initControl();
			}
		}
	}

	return false;
}

