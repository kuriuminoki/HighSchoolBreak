#include "Skill.h"
#include "Cell.h"
#include "Character.h"
#include "CharacterBuff.h"
#include "CharacterController.h"
#include "Graphs.h"


#include <sstream>


using namespace std;


/*
* スキルの基底クラス
*/
Skill::Skill() {
	m_skillName = "スキル名未設定";
	m_skillCategory = ATTACK_SKILL;
	m_needSkillPoint = 100;
}


/*
* 追加で移動するスキル
*/
MoveWithoutDiceSkill::MoveWithoutDiceSkill(int needSkillPoint, int distance) {
	m_needSkillPoint = needSkillPoint;
	m_distance = distance;
	m_skillCategory = SKILL_CATEGORY::MOVE_SKILL;
}


string MoveWithoutDiceSkill::getSkillDesc() const {
	ostringstream oss;
	oss << m_distance << "マス以内の移動を行う。";
	return oss.str();
}


std::string MoveWithoutDiceSkill::getSkillBonusDesc(int turn) const {
	if (calcTurnBonus(turn) == 0) { return ""; }
	ostringstream oss;
	oss << "進めるマスが" << calcTurnBonus(turn) << "増加する。";
	return oss.str();
}


COMMAND_TO_BF MoveWithoutDiceSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController, const Character* skillOwner) const {
	characterController->moveSpecificDistance(m_distance, cells);
	return RETRY_MOVE;
}


int MoveWithoutDiceSkill::calcTurnBonus(int turn) const {
	return turn / 2;
}


/*
* 追加で攻撃/回復するスキル
*/
AdditionalAttackSkill::AdditionalAttackSkill(int needSkillPoint, AttackInfo* attackInfo) {
	m_needSkillPoint = needSkillPoint;
	m_attackInfo = attackInfo;
	m_skillCategory = SKILL_CATEGORY::ATTACK_SKILL;
	if (attackInfo->getTargets()[0].first < 0) {
		m_skillCategory = SKILL_CATEGORY::CURE_SKILL;
	}
}


AdditionalAttackSkill::~AdditionalAttackSkill() {
	delete m_attackInfo;
}


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


std::string AdditionalAttackSkill::getSkillBonusDesc(int turn) const {
	if (calcTurnBonus(turn) == 0) { return ""; }
	ostringstream oss;
	oss << "威力が" << calcTurnBonus(turn) << "増加する。";
	return oss.str();
}


COMMAND_TO_BF AdditionalAttackSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* character_controller, const Character* skillOwner) const {
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
			// TODO: スキルにバフ・デバフをかけるならここに処理を書く
			cells[ty][tx]->setDamageValue(targets[i].first, groupKind);
			if (attack) {
				cells[ty][tx]->damageCharacter();
			}
		}
	}
}


int AdditionalAttackSkill::calcTurnBonus(int turn) const {
	return turn * 10;
}


/*
* 踏んだ敵にダメージを与える(罠)スキル
*/
DefenceSkill::DefenceSkill(int needSkillPoint, int damage) {
	m_needSkillPoint = needSkillPoint;
	m_damage = damage;
	m_skillCategory = SKILL_CATEGORY::DEFENCE_SKILL;
}


string DefenceSkill::getSkillDesc() const {
	ostringstream oss;
	oss << "止まったキャラが" << m_damage << "ダメージ受ける。";
	return oss.str();
}


std::string DefenceSkill::getSkillBonusDesc(int turn) const {
	if (calcTurnBonus(turn) == 0) { return ""; }
	ostringstream oss;
	oss << "受けるダメージがさらに" << calcTurnBonus(turn) << "増加する。";
	return oss.str();
}


COMMAND_TO_BF DefenceSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* character_controller, const Character* skillOwner) const {
	cells[y][x]->setDamageValue(m_damage, NOT_ANY_GROUP);
	cells[y][x]->damageCharacter();
	return NONE_REQUEST;
}


int DefenceSkill::calcTurnBonus(int turn) const {
	return turn * 10;
}


/*
* 攻撃力のバフ・デバフを付与するスキル
*/
AttackBuffSkill::AttackBuffSkill(int needSkillPoint, int buffTurnSum, int attackValue) {
	m_needSkillPoint = needSkillPoint;
	m_buffTurnSum = buffTurnSum;
	m_attackValue = attackValue;
	if (m_attackValue > 0) {
		m_skillCategory = SKILL_CATEGORY::ATTACK_UP_SKILL;
	}
	else {
		m_skillCategory = SKILL_CATEGORY::ATTACK_DOWN_SKILL;
	}
}


string AttackBuffSkill::getSkillDesc() const {
	ostringstream oss;
	oss << m_buffTurnSum << "ターンの間攻撃力が";
	if (m_attackValue > 0) {
		oss << m_attackValue << "上がる。";
	}
	else {
		oss << -m_attackValue << "下がる。";
	}
	return oss.str();
}


std::string AttackBuffSkill::getSkillBonusDesc(int turn) const {
	if (calcTurnBonus(turn) == 0) { return ""; }
	ostringstream oss;
	oss << "攻撃力がさらに" << calcTurnBonus(turn);
	if (m_attackValue > 0) {
		oss << "上がる。";
	}
	else {
		oss << "下がる。";
	}
	return oss.str();
}


COMMAND_TO_BF AttackBuffSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* character_controller, const Character* skillOwner) const {
	cells[y][x]->getCharacter()->addBuff(new AttackBuff(m_buffTurnSum, m_attackValue));
	return NONE_REQUEST;
}


int AttackBuffSkill::calcTurnBonus(int turn) const {
	return turn * 10;
}


/*
* スピードのバフ・デバフを付与するスキル
*/
SpeedBuffSkill::SpeedBuffSkill(int needSkillPoint, int buffTurnSum, int speedValue) {
	m_needSkillPoint = needSkillPoint;
	m_buffTurnSum = buffTurnSum;
	m_speedValue = speedValue;
	if (m_speedValue > 0) {
		m_skillCategory = SKILL_CATEGORY::SPEED_UP_SKILL;
	}
	else {
		m_skillCategory = SKILL_CATEGORY::SPEED_DOWN_SKILL;
	}
}


string SpeedBuffSkill::getSkillDesc() const {
	ostringstream oss;
	oss << m_buffTurnSum << "ターンの間スピードが";
	if (m_speedValue > 0) {
		oss << m_speedValue << "上がる。";
	}
	else {
		oss << -m_speedValue << "下がる。";
	}
	return oss.str();
}


std::string SpeedBuffSkill::getSkillBonusDesc(int turn) const {
	if (calcTurnBonus(turn) == 0) { return ""; }
	ostringstream oss;
	oss << "スピードがさらに" << calcTurnBonus(turn);
	if (m_speedValue > 0) {
		oss << "上がる。";
	}
	else {
		oss << "下がる。";
	}
	return oss.str();
}


COMMAND_TO_BF SpeedBuffSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* character_controller, const Character* skillOwner) const {
	cells[y][x]->getCharacter()->addBuff(new SpeedBuff(m_buffTurnSum, m_speedValue));
	return NONE_REQUEST;
}


int SpeedBuffSkill::calcTurnBonus(int turn) const {
	return turn;
}


/*
* 防御力のバフ・デバフを付与するスキル
*/
DefenseBuffSkill::DefenseBuffSkill(int needSkillPoint, int buffTurnSum, int defenseValue) {
	m_needSkillPoint = needSkillPoint;
	m_buffTurnSum = buffTurnSum;
	m_defenseValue = defenseValue;
	if (m_defenseValue > 0) {
		m_skillCategory = SKILL_CATEGORY::DEFENSE_UP_SKILL;
	}
	else {
		m_skillCategory = SKILL_CATEGORY::DEFENSE_DOWN_SKILL;
	}
}


string DefenseBuffSkill::getSkillDesc() const {
	ostringstream oss;
	oss << m_buffTurnSum << "ターンの間防御力が";
	if (m_defenseValue > 0) {
		oss << m_defenseValue << "上がる。";
	}
	else {
		oss << -m_defenseValue << "下がる。";
	}
	return oss.str();
}


std::string DefenseBuffSkill::getSkillBonusDesc(int turn) const {
	if (calcTurnBonus(turn) == 0) { return ""; }
	ostringstream oss;
	oss << "防御力がさらに" << calcTurnBonus(turn);
	if (m_defenseValue > 0) {
		oss << "上がる。";
	}
	else {
		oss << "下がる。";
	}
	return oss.str();
}


COMMAND_TO_BF DefenseBuffSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* character_controller, const Character* skillOwner) const {
	cells[y][x]->getCharacter()->addBuff(new DefenseBuff(m_buffTurnSum, m_defenseValue));
	return NONE_REQUEST;
}


int DefenseBuffSkill::calcTurnBonus(int turn) const {
	return turn * 10;
}
