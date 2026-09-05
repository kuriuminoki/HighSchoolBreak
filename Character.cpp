#include "Character.h"
#include "CharacterBuff.h"
#include "Graphs.h"
#include "Skill.h"
#include "SpecialSkill.h"


using namespace std;


/*
* プロフィール
*/
CharacterProfile::CharacterProfile(int id, std::string lastName, std::string firstName) {
	m_id = id;
	m_lastName = lastName;
	m_firstName = firstName;

	if (lastName == "アカツキ") {
		m_characterNum = AKATSUKI;
	}
	else if (lastName == "トウノ") {
		m_characterNum = TOUNO;
	}
	else if (lastName == "タキノ") {
		m_characterNum = TAKINO;
	}
	else if (lastName == "カイバラ") {
		m_characterNum = KAIBARA;
	}
	else if (lastName == "カンエイ") {
		m_characterNum = KANEI;
	}
	else {
		m_characterNum = AKATSUKI;
	}
}


string CharacterProfile::getFullName() const {
	return m_lastName + " " + m_firstName;
}


/*
* ステータス
*/
CharacterStatus::CharacterStatus() {
	m_maxHp = 100;
	m_dispHp = m_maxHp;
	m_hp = m_maxHp;
	m_speed = 6;
	m_skillPoint = 0;
	m_maxSkillPoint = 30;
	m_specialPoint = 18;
	m_maxSpecialPoint = 20;
}


/*
* 攻撃情報
*/
AttackInfo::AttackInfo(int n) {
	if (n == 0) {
		m_targets.push_back(make_pair(-10, make_pair(0, 0)));
	}
	else {
		m_targets.push_back(make_pair(10, make_pair(-1, -1)));
		m_targets.push_back(make_pair(10, make_pair(-1, 0)));
		m_targets.push_back(make_pair(10, make_pair(-1, 1)));
		m_targets.push_back(make_pair(10, make_pair(0, 1)));
		m_targets.push_back(make_pair(10, make_pair(1, 1)));
		m_targets.push_back(make_pair(10, make_pair(1, 0)));
		m_targets.push_back(make_pair(10, make_pair(1, -1)));
		m_targets.push_back(make_pair(10, make_pair(0, -1)));
	}
}


/*
* キャラクターの基底クラス
*/
Character::Character(CharacterProfile* characterProfile, CharacterStatus* characterStatus, int x, int y, GROUP_KIND groupKind) {
	m_characterProfile = characterProfile;
	m_characterStatus = characterStatus;
	m_attackInfo = new AttackInfo(1);
	m_x = x;
	m_y = y;
	m_groupKind = groupKind;
	m_dispHpCnt = 0;
	m_needSkillPoint = 0;
	m_skill.push_back(new MoveWithoutDiceSkill(3, 1));
	m_skill.push_back(new AdditionalAttackSkill(5, new AttackInfo(0)));
	//m_skill.push_back(new DefenceSkill(3, 10));
	m_skill.push_back(new AdditionalAttackSkill(5, new AttackInfo(1)));
	m_skill.push_back(new AttackBuffSkill(3, 2, 20));
	//m_skill.push_back(new AttackBuffSkill(3, 2, -20));
	//m_skill.push_back(new SpeedBuffSkill(3, 2, 2));
	//m_skill.push_back(new SpeedBuffSkill(3, 2, -2));
	//m_skill.push_back(new DefenseBuffSkill(3, 3, 20));
	//m_skill.push_back(new DefenseBuffSkill(3, 3, -20));
	m_specialSkill = new AttackSpecialSkill(new AttackInfo(1));

}


Character::~Character() {
	delete m_characterProfile;
	delete m_characterStatus;
	delete m_attackInfo;
	for (unsigned int i = 0; i < m_skill.size(); i++) {
		delete m_skill[i];
	}
	for (int i = 0; i < m_buffs.size(); i++) {
		delete m_buffs[i];
	}
	delete m_specialSkill;
}


void Character::damage(int damageValue) {
	if (damageValue != 0) {
		m_dispHpCnt = DISP_HP_COUNT;
	}
	m_characterStatus->setHp(min(m_characterStatus->getMaxHp(), max(0, m_characterStatus->getHp() - damageValue)));
}


void Character::updateDispHp() {
	if (m_characterStatus->getDispHp() > m_characterStatus->getHp()) {
		m_characterStatus->setDispHp(m_characterStatus->getDispHp() - 1);
	}
	else if (m_characterStatus->getDispHp() < m_characterStatus->getHp()) {
		m_characterStatus->setDispHp(m_characterStatus->getDispHp() + 1);
	}
	else {
		m_dispHpCnt--;
	}
}


void Character::addSkillPoint(int addValue) {
	int skillPoint = m_characterStatus->getSkillPoint() + addValue;
	skillPoint = max(0, min(m_characterStatus->getMaxSkillPoint(), skillPoint));
	m_characterStatus->setSkillPoint(skillPoint);

	if (addValue < 0) {
		addSpecialPoint(-addValue);
	}
}


void Character::addSpecialPoint(int addValue) {
	int specialPoint = m_characterStatus->getSpecialPoint() + addValue;
	specialPoint = max(0, min(m_characterStatus->getMaxSpecialPoint(), specialPoint));
	m_characterStatus->setSpecialPoint(specialPoint);
}


void Character::addBuff(CharacterBuff* buff) {
	m_buffs.push_back(buff);
}


void Character::nextTurn() {
	int len = (int)m_buffs.size();
	for (int i = 0; i < len; i++) {
		m_buffs[i]->nextTurn();
		if (m_buffs[i]->isEnd()) {
			delete m_buffs[i];
			m_buffs[i] = m_buffs.back();
			m_buffs.pop_back();
			len--;
		}
	}
}


int Character::calcAttackBuffValue() const {
	int attackSumValue = 0;
	for (int i = 0; i < m_buffs.size(); i++) {
		attackSumValue += m_buffs[i]->getAttackBuf();
	}
	return attackSumValue;
}


int Character::calcSpeedBuffValue() const {
	int speedSumValue = 0;
	for (int i = 0; i < m_buffs.size(); i++) {
		speedSumValue += m_buffs[i]->getSpeedBuf();
	}
	return speedSumValue;
}


int Character::calcDefenseBuffValue() const {
	int defenseSumValue = 0;
	for (int i = 0; i < m_buffs.size(); i++) {
		defenseSumValue += m_buffs[i]->getDefenseBuf();
	}
	return defenseSumValue;
}
