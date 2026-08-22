#include "CharacterController.h"
#include "Cell.h"
#include "Character.h"
#include "Control.h"
#include "Dice.h"
#include "Define.h"

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
}


void CharacterController::initControl() {
	for (unsigned int i = 0; i < m_routeMemo.size(); i++) {
		m_routeMemo[i].clear();
	}
	m_routeMemo.clear();
	m_track.clear();
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
		cells[y][x]->setMarkingColor(LIGHT_BLUE);
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
	m_track.push_back(make_pair(sy, sx));

	// お掃除
	for (unsigned int i = 0; i < m_routeMemo.size(); i++) {
		m_routeMemo[i].clear();
	}
	m_routeMemo.clear();
}


/*
* 生徒操作
*/
StudentController::StudentController(Dice* dice_p):
	CharacterController(dice_p)
{
	m_ableFinish = false;
}


void StudentController::initControl() {
	m_ableFinish = false;
	CharacterController::initControl();
}


bool StudentController::play(int handX, int handY, std::vector<std::vector<Cell*> >& cells) {
	if (leftClick() == 1) {
		if (m_dice_p->overlap(handX, handY)) {
			m_dice_p->init(m_character_p->getCharacterStatus()->getSpeed(), 1, 60);
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


/*
* 敵操作
*/
EnemyController::EnemyController(Dice* dice_p):
	CharacterController(dice_p)
{
	m_state = INIT_DICE;
}


void EnemyController::initControl() {
	m_state = INIT_DICE;
	CharacterController::initControl();
}


bool EnemyController::play(int handX, int handY, std::vector<std::vector<Cell*> >& cells) {
	if (m_state == INIT_DICE) {
		m_dice_p->init(m_character_p->getCharacterStatus()->getSpeed(), 1, 10);
		m_state = PLAY_DICE;
	}
	else if (m_state == PLAY_DICE) {
		if (m_dice_p->play() && m_routeMemo.empty()) {
			// 幅優先探索で各マスへの最短ルートを検索
			searchAllTrack(m_dice_p->getValue(), cells);
			// 目標地点までの経路を設定
			vector<pair<int, int> > candidate;
			for (unsigned int y = 0; y < cells.size(); y++) {
				for (unsigned int x = 0; x < cells[y].size(); x++) {
					if (cells[y][x]->getMarkingColor() != -1) {
						candidate.push_back(make_pair(y, x));
					}
				}
			}
			if (candidate.size() == 0) {
				return true; // 動けないため終了
			}
			int r = GetRand((int)candidate.size() - 1); // 完全ランダムで移動
			searchGoalRoute(candidate[r].first, candidate[r].second, cells);
			m_state = MOVING;
		}
	}

	if (m_state == MOVING) {
		if (move(m_character_p, m_track[m_track.size() - 1].first, m_track[m_track.size() - 1].second, cells, false)) {
			m_track.pop_back();
		}
		if (m_track.empty()) {
			for (unsigned int y = 0; y < cells.size(); y++) {
				for (unsigned int x = 0; x < cells[y].size(); x++) {
					cells[y][x]->setMarkingColor(-1);
				}
			}
			return true;
		}
	}

	return false;
}
