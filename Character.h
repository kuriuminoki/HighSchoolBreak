#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include <string>


enum CHARACTER_NUM {
	AKATSUKI,
	TOUNO,
	TAKINO,
	KAIBARA,
	KANEI
};


enum GROUP_KIND {
	STUDENT,
	TEACHER,
	MONSTER
};


/*
* プロフィール
*/
class CharacterProfile {
private:
	std::string m_lastName;
	std::string m_firstName;

	CHARACTER_NUM m_characterNum; // キャラの番号

public:
	CharacterProfile(std::string lastName, std::string firstName);

	// ゲッタ
	inline std::string getLastName() const { return m_lastName; }
	inline std::string getFirstName() const { return m_firstName; }
	std::string getFullName() const;
	inline CHARACTER_NUM getCharacterIconGraphNum() const { return m_characterNum; }
};


/*
* ステータス
*/
class CharacterStatus {
private:
	int m_maxHp;
	int m_hp;
	int m_speed;

public:
	CharacterStatus();

	// ゲッタ
	inline int getMaxHp() const { return m_maxHp; }
	inline int getHp() const { return m_hp; }
	inline int getSpeed() const { return m_speed; }
};


/*
* キャラの基底クラス
*/
class Character {
private:
	CharacterProfile* m_characterProfile;
	CharacterStatus* m_characterStatus;
	int m_dispX;
	int m_dispY;
	int m_x;
	int m_y;
	GROUP_KIND m_groupKind;

public:
	Character(CharacterProfile* characterProfile, CharacterStatus* characterStatus, int x, int y, GROUP_KIND groupKind);
	~Character();

	// ゲッタ
	inline const CharacterProfile* getCharacterProfile() const { return m_characterProfile; }
	inline const CharacterStatus* getCharacterStatus() const { return m_characterStatus; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getDispX() const { return m_dispX; }
	inline int getDispY() const { return m_dispY; }
	inline GROUP_KIND getGroupKind() const { return m_groupKind; }
};


#endif