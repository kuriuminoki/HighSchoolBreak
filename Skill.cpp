#include "Skill.h"
#include "Cell.h"
#include "Character.h"
#include "CharacterController.h"
#include "Graphs.h"


#include <sstream>


using namespace std;


/*
* スキルの基底クラス
*/
Skill::Skill() {
	m_skillName = "スキル名未設定";
	m_skillCategory = ATTACK;
	m_needSkillPoint = 100;
}


/*
* 追加で移動するスキル
*/
MoveWithoutDiceSkill::MoveWithoutDiceSkill(int needSkillPoint, int distance) {
	m_needSkillPoint = needSkillPoint;
	m_distance = distance;
	m_skillCategory = SKILL_CATEGORY::MOVE;
}


// スキルの説明文
string MoveWithoutDiceSkill::getSkillDesc() const {
	ostringstream oss;
	oss << m_distance << "マス以内の移動を行う。";
	return oss.str();
}


// 発火させる。y, xはこのスキルの発動場所。
COMMAND_TO_BF MoveWithoutDiceSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController) const {
	characterController->moveSpecificDistance(m_distance, cells);
	return RETRY_MOVE;
}


/*
* 追加で攻撃/回復するスキル
*/
AdditionalAttackSkill::AdditionalAttackSkill(int needSkillPoint, AttackInfo* attackInfo) {
	m_needSkillPoint = needSkillPoint;
	m_attackInfo = attackInfo;
	m_skillCategory = SKILL_CATEGORY::ATTACK;
	if (attackInfo->getTargets()[0].first < 0) {
		m_skillCategory = SKILL_CATEGORY::CURE;
	}
}


AdditionalAttackSkill::~AdditionalAttackSkill() {
	delete m_attackInfo;
}


// スキルの説明文
string AdditionalAttackSkill::getSkillDesc() const {
	const vector<pair<int, pair<int, int> > > targets = m_attackInfo->getTargets();
	int maxDamage = targets[0].first, minDamage = targets[0].first;
	for (unsigned int i = 1; i < targets.size(); i++) {
		maxDamage = max(maxDamage, targets[i].first);
		minDamage = min(minDamage, targets[i].first);
	}

	ostringstream oss;
	if (minDamage > 0) {
		if (maxDamage == minDamage) {
			oss << maxDamage << "ダメージの攻撃を行う。";
		}
		else {
			oss << minDamage << "～" << maxDamage << "ダメージの攻撃を行う。";
		}
	}
	else {
		if (maxDamage == minDamage) {
			oss << "HPを" << -maxDamage << "回復する。";
		}
		else {
			oss << "HPを" << -maxDamage << "～" << -minDamage << "回復する。";
		}
	}
	
	return oss.str();
}


// 発火させる。y, xはこのスキルの発動場所。
COMMAND_TO_BF AdditionalAttackSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* character_controller) const {
	putAttackInfoToCells(y, x, cells, cells[y][x]->getCharacter()->getGroupKind(), true);
	return NONE_REQUEST;
}


void AdditionalAttackSkill::setDamageCell(int y, int x, std::vector<std::vector<Cell*> >& cells) const {
	putAttackInfoToCells(y, x, cells, GROUP_KIND::STUDENT, false);
}


void AdditionalAttackSkill::putAttackInfoToCells(int y, int x, std::vector<std::vector<Cell*> >& cells, GROUP_KIND groupKind, bool attack) const {
	const vector<pair<int, pair<int, int> > > targets = m_attackInfo->getTargets();
	for (unsigned int i = 0; i < targets.size(); i++) {
		int ty = y + targets[i].second.first;
		int tx = x + targets[i].second.second;
		if (ty >= 0 && ty < cells.size() && tx >= 0 && tx < cells[0].size()) {
			cells[ty][tx]->setDamageValue(targets[i].first, groupKind);
			if (attack) {
				cells[ty][tx]->damageCharacter();
			}
		}
	}
}


/*
* 踏んだ敵にダメージを与える(罠)スキル
*/
DefenceSkill::DefenceSkill(int needSkillPoint, int damage) {
	m_needSkillPoint = needSkillPoint;
	m_damage = damage;
	m_skillCategory = SKILL_CATEGORY::DEFENCE;
}


// スキルの説明文
string DefenceSkill::getSkillDesc() const {
	ostringstream oss;
	oss << "止まったキャラが" << m_damage << "ダメージ受ける。";
	return oss.str();
}


// 発火させる。y, xはこのスキルの発動場所。
COMMAND_TO_BF DefenceSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* character_controller) const {
	cells[y][x]->setDamageValue(m_damage, NOT_ANY_GROUP);
	cells[y][x]->damageCharacter();
	return NONE_REQUEST;
}
