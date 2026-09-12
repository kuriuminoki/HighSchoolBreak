#include "Skill.h"
#include "AttackInfo.h"
#include "Cell.h"
#include "Character.h"
#include "CharacterBuff.h"
#include "CharacterController.h"
#include "CsvReader.h"
#include "Graphs.h"

#include "DxLib.h"

#include <algorithm>
#include <map>
#include <sstream>


using namespace std;


/*
* csvファイルから読み込んだ情報を受け取ってスキルを作成
*/
Skill* createSkill(string skillName, CsvReader* csvReader, AttackInfoCreator* attackInfoCreator) {
	Skill* res = nullptr;
	map<string, string> skillData = csvReader->findOne("name", skillName.c_str());
	if (skillData.empty()) {
		res = new MoveWithoutDiceSkill(3, 1);
		skillName = "skill not found: " + skillName;
		res->setSkillName(skillName);
		ErrorLogAdd(skillName.c_str());
		return res;
	}

	string className = skillData["class"];
	int needSkillPoint = stoi(skillData["needSkillPoint"]);
	if (className == "MoveWithoutDiceSkill") {
		res = new MoveWithoutDiceSkill(needSkillPoint, stoi(skillData["param1"]));
	}
	else if (className == "AdditionalAttackSkill") {
		string baseDamage = skillData["param1"];
		res = new AdditionalAttackSkill(needSkillPoint, attackInfoCreator->createAttackInfo(skillName, baseDamage.empty() ? 0 : stoi(baseDamage)));
	}
	else if (className == "DefenseSkill") {
		res = new DefenseSkill(needSkillPoint, stoi(skillData["param1"]));
	}
	else if (className == "AttackBuffSkill") {
		res = new AttackBuffSkill(needSkillPoint, stoi(skillData["param1"]), stoi(skillData["param2"]));
	}
	else if (className == "SpeedBuffSkill") {
		res = new SpeedBuffSkill(needSkillPoint, stoi(skillData["param1"]), stoi(skillData["param2"]));
	}
	else if (className == "DefenseBuffSkill") {
		res = new DefenseBuffSkill(needSkillPoint, stoi(skillData["param1"]), stoi(skillData["param2"]));
	}
	else {
		res = new MoveWithoutDiceSkill(3, 1);
		skillName = "class not found: " + className;
		ErrorLogAdd(skillName.c_str());
	}

	res->setSkillName(skillName);
	return res;
}


/*
* スキルの基底クラス
*/
Skill::Skill() {
	m_skillName = "スキル名未設定";
	m_skillCategory = ATTACK_SKILL;
	m_needSkillPoint = 100;
}


bool Skill::isGroupPenalty(int y, int x, std::vector<std::vector<Cell*> >& cells, const Character* skillOwner) const {
	if (cells[y][x]->getCharacter() == nullptr || skillOwner == nullptr) {
		return false;
	}
	return cells[y][x]->getCharacter()->getGroupKind() != skillOwner->getGroupKind();
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
	int distance = m_distance + calcTurnBonus(cells[y][x]->getSkillTurn());
	if (isGroupPenalty(y, x, cells, skillOwner)) {
		distance = max(1, distance / 2);
	}
	characterController->moveSpecificDistance(distance, cells);
	return RETRY_MOVE;
}


int MoveWithoutDiceSkill::calcTurnBonus(int turn) const {
	return min(turn / 2, 3);
}


/*
* 追加で攻撃/回復するスキル
*/
AdditionalAttackSkill::AdditionalAttackSkill(int needSkillPoint, AttackInfo* attackInfo) {
	m_needSkillPoint = needSkillPoint;
	m_attackInfo = attackInfo;
	m_skillCategory = SKILL_CATEGORY::ATTACK_SKILL;
	if (attackInfo->getAttackElement()[0]->getDamage() < 0) {
		m_skillCategory = SKILL_CATEGORY::CURE_SKILL;
	}
}


AdditionalAttackSkill::~AdditionalAttackSkill() {
	delete m_attackInfo;
}


string AdditionalAttackSkill::getSkillDesc() const {
	const vector<AttackElement*> targets = m_attackInfo->getAttackElement();
	int maxDamage = targets[0]->getDamage(), minDamage = targets[0]->getDamage();
	for (unsigned int i = 1; i < targets.size(); i++) {
		maxDamage = max(maxDamage, targets[i]->getDamage());
		minDamage = min(minDamage, targets[i]->getDamage());
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
	putAttackInfoToCells(y, x, cells, cells[y][x]->getCharacter()->getGroupKind(), true, skillOwner);
	return NONE_REQUEST;
}


void AdditionalAttackSkill::setDamageCell(int y, int x, std::vector<std::vector<Cell*> >& cells) const {
	putAttackInfoToCells(y, x, cells, GROUP_KIND::STUDENT, false, nullptr);
}


void AdditionalAttackSkill::putAttackInfoToCells(int y, int x, std::vector<std::vector<Cell*> >& cells, GROUP_KIND groupKind, bool attack, const Character* skillOwner) const {
	const vector<AttackElement*> targets = m_attackInfo->getAttackElement();
	int bonus = calcTurnBonus(cells[y][x]->getSkillTurn());
	if (m_skillCategory == CURE_SKILL) {
		bonus *= -1;
	}
	int penalty = 1;
	if (isGroupPenalty(y, x, cells, skillOwner)) {
		penalty = 2; // 相手のスキルなら威力半減
	}
	for (unsigned int i = 0; i < targets.size(); i++) {
		int ty = y + targets[i]->getDy();
		int tx = x + targets[i]->getDx();
		if (ty >= 0 && ty < cells.size() && tx >= 0 && tx < cells[0].size()) {
			// TODO: スキルにバフ・デバフをかけるならここに処理を書く
			int damage = (targets[i]->getDamage() + bonus) / penalty;
			if (damage == 0) {
				damage = targets[i]->getDamage() / abs(targets[i]->getDamage()); // 攻撃なら1, 回復なら-1 が最低効果
			}
			cells[ty][tx]->setDamageValue(damage, groupKind);
			if (attack) {
				cells[ty][tx]->damageCharacter();
			}
		}
	}
}


int AdditionalAttackSkill::calcTurnBonus(int turn) const {
	return min(turn / 2, 10);
}


/*
* 踏んだ敵にダメージを与える(罠)スキル
*/
DefenseSkill::DefenseSkill(int needSkillPoint, int damage) {
	m_needSkillPoint = needSkillPoint;
	m_damage = damage;
	m_skillCategory = SKILL_CATEGORY::DEFENSE_SKILL;
}


string DefenseSkill::getSkillDesc() const {
	ostringstream oss;
	oss << "止まったキャラが" << m_damage << "ダメージ受ける。";
	return oss.str();
}


std::string DefenseSkill::getSkillBonusDesc(int turn) const {
	if (calcTurnBonus(turn) == 0) { return ""; }
	ostringstream oss;
	oss << "受けるダメージがさらに" << calcTurnBonus(turn) << "増加する。";
	return oss.str();
}


COMMAND_TO_BF DefenseSkill::fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* character_controller, const Character* skillOwner) const {
	int damage = m_damage + calcTurnBonus(cells[y][x]->getSkillTurn());
	if (!isGroupPenalty(y, x, cells, skillOwner)) {
		// 自チームのスキルならダメージ半減(攻撃系とは逆)
		damage = max(1, damage / 2);
	}
	cells[y][x]->setDamageValue(damage, NOT_ANY_GROUP);
	cells[y][x]->damageCharacter();
	return NONE_REQUEST;
}


int DefenseSkill::calcTurnBonus(int turn) const {
	return min(turn, m_damage * 2);
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
	int attackValue = m_attackValue + calcTurnBonus(cells[y][x]->getSkillTurn());
	if (isGroupPenalty(y, x, cells, skillOwner)) {
		attackValue = max(1, attackValue / 2);
	}
	cells[y][x]->getCharacter()->addBuff(new AttackBuff(m_buffTurnSum, attackValue));
	return NONE_REQUEST;
}


int AttackBuffSkill::calcTurnBonus(int turn) const {
	return min(turn, m_attackValue * 2);
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
	int speedValue = m_speedValue + calcTurnBonus(cells[y][x]->getSkillTurn());
	if (isGroupPenalty(y, x, cells, skillOwner)) {
		speedValue = max(1, speedValue / 2);
	}
	cells[y][x]->getCharacter()->addBuff(new SpeedBuff(m_buffTurnSum, speedValue));
	return NONE_REQUEST;
}


int SpeedBuffSkill::calcTurnBonus(int turn) const {
	return min(turn / 2, 6);
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
	int defenseValue = m_defenseValue + calcTurnBonus(cells[y][x]->getSkillTurn());
	if (isGroupPenalty(y, x, cells, skillOwner)) {
		defenseValue = max(1, defenseValue / 2);
	}
	cells[y][x]->getCharacter()->addBuff(new DefenseBuff(m_buffTurnSum, defenseValue));
	return NONE_REQUEST;
}


int DefenseBuffSkill::calcTurnBonus(int turn) const {
	return min(turn, m_defenseValue * 2);
}
