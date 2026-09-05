#include "BattleFieldDrawer.h"
#include "BattleField.h"
#include "Animation.h"
#include "Cell.h"
#include "Character.h"
#include "Define.h"
#include "Dice.h"
#include "DrawUtils.h"
#include "Graphs.h"
#include "Skill.h"
#include "DxLib.h"

#include <string>
#include <sstream>


using namespace std;


/*
* BattleFieldのDrawer
*/
BattleFieldDrawer::BattleFieldDrawer(BattleField* battleField_p) {
	m_battleField_p = battleField_p;
	getGameEx(m_exX, m_exY);
	m_characterGraphs = new CharacterGraphs();
	m_effectGraphs = new EffectGraphs();
	m_font = CreateFontToHandle(nullptr, applyEx(100, m_exX), 50);
	m_middleFont = CreateFontToHandle(nullptr, applyEx(50, m_exX), 10);
	m_smallFont = CreateFontToHandle(nullptr, applyEx(20, m_exX), 7);

	m_cnt = 0;
}


BattleFieldDrawer::~BattleFieldDrawer() {
	delete m_characterGraphs;
	delete m_effectGraphs;
	DeleteFontToHandle(m_font);
	DeleteFontToHandle(m_middleFont);
	DeleteFontToHandle(m_smallFont);
}


void BattleFieldDrawer::draw() {
	GetMousePoint(&m_handX, &m_handY);
	m_cnt++;

	DrawBox(0, 0, GAME_WIDE, GAME_HEIGHT, GRAY, TRUE);

	drawCellsAndCharacters();


	// キャラ情報の描画
	const vector<CharacterInfoButton*> characterInfoButton = m_battleField_p->getCharacterInfoButton();
	for (unsigned int i = 0; i < characterInfoButton.size(); i++) {
		characterInfoButton[i]->draw(m_handX, m_handY, m_characterGraphs, m_smallFont);
	}

	// マス情報の描画
	m_battleField_p->getCellInfoButton()->draw(m_handX, m_handY, m_characterGraphs, m_smallFont);

	// スキル情報の描画
	m_battleField_p->getSkillInfoButton()->draw(m_handX, m_handY, m_characterGraphs, m_smallFont);

	// サイコロの描画
	m_battleField_p->getDice()->draw(m_handX, m_handY, m_font, BLACK);

	// ボタンの描画
	m_battleField_p->getEndActionButton()->draw(m_handX, m_handY, true, m_middleFont, BLACK);

	if (m_battleField_p->getHangingSkill() != nullptr) {
		DrawRotaGraph(m_handX, m_handY, 0.5 * m_exX, 0.0, m_characterGraphs->getSkillIconGraphs(m_battleField_p->getHangingSkill()->getSkillCategory()), TRUE);
	}

	// スキル発動中
	const SkillPlayer* skillPlayer = m_battleField_p->getSkillPlayer();
	if (skillPlayer != nullptr) {
		bool isSpecial = skillPlayer->isSpecial();
		int cnt = skillPlayer->getCnt();
		if (skillPlayer->isPrePlaySpan()) {

		}
		else {
			int color = isSpecial ? LIGHT_RED : LIGHT_YELLOW;
			const int y1 = GAME_HEIGHT / 2 - applyEx(200, m_exY);
			const int y2 = GAME_HEIGHT / 2 + applyEx(200, m_exY);
			DrawBox(0, y1, GAME_WIDE, y2, color, TRUE);
			int x = applyEx(100, m_exX);
			int fontSize = 0;
			GetFontStateToHandle(NULL, &fontSize, NULL, m_middleFont);
			x = max(x, x + (30 - cnt) * 150);
			if (isSpecial) {
				const int dx = applyEx(300, m_exX);
				int handle = m_characterGraphs->getSpecialCharacterGraphs(skillPlayer->getSkillOwner()->getCharacterProfile()->getCharacterIconGraphNum());
				DrawRotaGraph(dx, GAME_HEIGHT / 2, 1.0, 0.0, handle, TRUE);
				GetGraphSize(handle, &x, NULL);
				x = x / 2 + dx;
				x = max(x, x + (30 - cnt) * 150);
			}
			DrawStringToHandle(x, y1 + fontSize, skillPlayer->getSkill()->getSkillName().c_str(), BLACK, m_middleFont);
			DrawStringToHandle(x + fontSize, y1 + applyEx(fontSize, 2.5), skillPlayer->getSkill()->getSkillDesc().c_str(), BLACK, m_middleFont);
			string bonusDesc = skillPlayer->getSkill()->getSkillBonusDesc(skillPlayer->getTurn());
			if (!bonusDesc.empty()) {
				DrawStringToHandle(x + fontSize, y1 + applyEx(fontSize, 4), ("ボーナス：" + bonusDesc).c_str(), RED, m_middleFont);
			}
			
		}
	}
}


void BattleFieldDrawer::drawCellsAndCharacters() {
	vector<const Character*> dispCharacter;
	vector<const Character*> dispHpBarCharacter;
	vector<const EffectAnimation*> dispEffect;

	// 各マスの描画
	const vector<vector<Cell*> > cells = m_battleField_p->getCells();
	for (unsigned int y = 0; y < cells.size(); y++) {
		for (unsigned int x = 0; x < cells[y].size(); x++) {
			// マスの描画
			cells[y][x]->draw(m_handX, m_handY, true, m_characterGraphs, m_smallFont);
			// マス上にいるキャラ
			if (cells[y][x]->getCharacter() != nullptr) {
				dispCharacter.push_back(cells[y][x]->getCharacter());
				if (cells[y][x]->getCharacter()->dispHpBar()) {
					dispHpBarCharacter.push_back(cells[y][x]->getCharacter());
				}
			}
			if (cells[y][x]->getEffectAnimation() != nullptr) {
				dispEffect.push_back(cells[y][x]->getEffectAnimation());
			}
		}
	}

	// マス上の各キャラの描画
	for (unsigned int i = 0; i < dispCharacter.size(); i++) {
		int handle = m_characterGraphs->getCharacterIconGraphs(dispCharacter[i]->getCharacterProfile()->getCharacterIconGraphNum());
		int x = dispCharacter[i]->getDispX();
		int y = dispCharacter[i]->getDispY();
		int wide = 0, height = 0;
		GetGraphSize(handle, &wide, &height);
		wide = (int)(wide * 0.1);
		height = (int)(height * 0.1);
		if (m_battleField_p->getActiveCharacter()->getCharacterProfile()->getId() == dispCharacter[i]->getCharacterProfile()->getId()
			&& m_cnt / 3 % 2 == 0) {
			SetDrawBright(100, 100, 100);
		}
		int reverseX = dispCharacter[i]->getGroupKind() == STUDENT ? FALSE : TRUE;
		DrawRotaGraph(x, y - height / 4, 0.1, 0.0, handle, TRUE, reverseX);
		SetDrawBright(255, 255, 255);
		drawBuffs(x - wide / 2, y, 0.8, dispCharacter[i]->getBuffs(), m_characterGraphs, 5);
	}

	// マス上のエフェクトの描画
	for (unsigned int i = 0; i < dispEffect.size(); i++) {
		DrawRotaGraph(dispEffect[i]->getX(), dispEffect[i]->getY(), 0.5, 0.0, m_effectGraphs->getEffectGraphs(dispEffect[i]->getKind(), dispEffect[i]->getAnimeNum()), TRUE);
	}

	// マス上のHPバー描画
	for (unsigned int i = 0; i < dispHpBarCharacter.size(); i++) {
		int handle = m_characterGraphs->getCharacterIconGraphs(dispHpBarCharacter[i]->getCharacterProfile()->getCharacterIconGraphNum());
		int x = dispHpBarCharacter[i]->getDispX();
		int y = dispHpBarCharacter[i]->getDispY();
		int wide = 0, height = 0;
		GetGraphSize(handle, &wide, &height);
		int hp = dispHpBarCharacter[i]->getCharacterStatus()->getHp();
		int dispHp = dispHpBarCharacter[i]->getCharacterStatus()->getDispHp();
		int maxHp = dispHpBarCharacter[i]->getCharacterStatus()->getMaxHp();
		drawHpBar(x - (int)(wide * 0.1 / 2) + applyEx(10, m_exX), y, x - (int)(wide * 0.1 / 2) + applyEx(80, m_exX), y + applyEx(10, m_exY), hp, dispHp, maxHp);
	}
}
