#include "Skill.h"
#include "Cell.h"
#include "Character.h"
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
COMMAND_TO_BF MoveWithoutDiceSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells) {
	return NONE;
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
	ostringstream oss;
	oss << "特殊攻撃を行う。";
	return oss.str();
}


// 発火させる。y, xはこのスキルの発動場所。
COMMAND_TO_BF AdditionalAttackSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells) {
	return NONE;
}
