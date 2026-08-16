#include "Character.h"


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
	m_hp = m_maxHp;
	m_speed = 6;
}


/*
* キャラクターの基底クラス
*/
Character::Character(CharacterProfile* characterProfile, CharacterStatus* characterStatus, int x, int y, GROUP_KIND groupKind) {
	m_characterProfile = characterProfile;
	m_characterStatus = characterStatus;
	m_x = x;
	m_y = y;
	m_groupKind = groupKind;
}


Character::~Character() {
	delete m_characterProfile;
	delete m_characterStatus;
}
