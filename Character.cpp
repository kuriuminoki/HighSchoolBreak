#include "Character.h"
#include "Graphs.h"
#include "Skill.h"


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
	m_skill.push_back(new DefenceSkill(3, 10));
	m_skill.push_back(new AdditionalAttackSkill(5, new AttackInfo(1)));

}


Character::~Character() {
	delete m_characterProfile;
	delete m_characterStatus;
	delete m_attackInfo;
	for (unsigned int i = 0; i < m_skill.size(); i++) {
		delete m_skill[i];
	}
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
}
