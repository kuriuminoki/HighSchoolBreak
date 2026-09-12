#include "Character.h"
#include "AttackInfo.h"
#include "CharacterBuff.h"
#include "CsvReader.h"
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
CharacterStatus::CharacterStatus(string lastName, CsvReader* csvReader) {
	map<string, string> data = csvReader->findOne("lastName", lastName.c_str());
	if (data.empty()) {
		data = csvReader->findOne("lastName", "アカツキ");
	}
	m_maxHp = stoi(data["hp"]);
	m_power = stoi(data["power"]);
	m_defense = stoi(data["defense"]);
	m_speed = stoi(data["speed"]);
	m_intelligence = stoi(data["intelligence"]);
	m_teamWork = stoi(data["teamWork"]);

	m_dispHp = m_maxHp;
	m_hp = m_maxHp;
	m_skillPoint = 0;
	m_maxSkillPoint = 30;
	m_specialPoint = 0;
	m_maxSpecialPoint = 20;
}


/*
* キャラクターの基底クラス
*/
Character::Character(int x, int y, GROUP_KIND groupKind) {
	m_x = x;
	m_y = y;
	m_groupKind = groupKind;
	m_dispHpCnt = 0;
	m_needSkillPoint = 0;
}


Character::Character(int id, std::string lastName, int x, int y, GROUP_KIND groupKind) :
	Character(x, y, groupKind)
{
	CsvReader* csvReader = new CsvReader("data/csv/characterInfo.csv");
	map<string, string> params = csvReader->findOne("lastName", lastName.c_str());

	// キャラの基本情報を設定
	m_characterProfile = new CharacterProfile(id, lastName, params["firstName"]);
	m_characterStatus = new CharacterStatus(lastName, csvReader);

	// キャラの通常攻撃を設定
	AttackInfoCreator* attackInfoCreator = new AttackInfoCreator();
	m_attackInfo = attackInfoCreator->createAttackInfo(lastName, 0);

	// スキルを設定
	CsvReader* skillReader = new CsvReader("data/csv/skillInfo.csv");
	m_skill.push_back(createSkill(params["skill1"], skillReader, attackInfoCreator));
	m_skill.push_back(createSkill(params["skill2"], skillReader, attackInfoCreator));
	m_skill.push_back(createSkill(params["skill3"], skillReader, attackInfoCreator));
	m_skill.push_back(createSkill(params["skill4"], skillReader, attackInfoCreator));
	delete skillReader;

	// 必殺技を設定
	CsvReader* specialReader = new CsvReader("data/csv/specialInfo.csv");
	m_specialSkill = createSpecial(params["special"], specialReader, attackInfoCreator);
	delete specialReader;

	delete attackInfoCreator;
	delete csvReader;
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
