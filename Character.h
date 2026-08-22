#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include <string>
#include <utility>
#include <vector>


enum CHARACTER_NUM;


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
	int m_id; // 重複なしのID
	std::string m_lastName;
	std::string m_firstName;

	CHARACTER_NUM m_characterNum; // キャラの種類番号

public:
	CharacterProfile(int id, std::string lastName, std::string firstName);

	// ゲッタ
	inline int getId() const { return m_id; }
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
	int m_dispHp;
	int m_hp;
	int m_speed;

public:
	CharacterStatus();

	// ゲッタ
	inline int getMaxHp() const { return m_maxHp; }
	inline int getDispHp() const { return m_dispHp; }
	inline int getHp() const { return m_hp; }
	inline int getSpeed() const { return m_speed; }

	// セッタ
	inline void setDispHp(int dispHp) { m_dispHp = dispHp; }
	inline void setHp(int hp) { m_hp = hp; }

};


class AttackInfo {
private:
	std::vector<std::pair<int, std::pair<int, int> > > m_targets; // (攻撃力, (dy, dx))

public:
	AttackInfo();

	// ゲッタ
	inline const std::vector<std::pair<int, std::pair<int, int> > >& getTargets() const { return m_targets; }
};


/*
* キャラの基底クラス
*/
class Character {
private:
	CharacterProfile* m_characterProfile;
	CharacterStatus* m_characterStatus;
	AttackInfo* m_attackInfo;
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
	inline const AttackInfo* getAttackInfo() const { return m_attackInfo; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getDispX() const { return m_dispX; }
	inline int getDispY() const { return m_dispY; }
	inline GROUP_KIND getGroupKind() const { return m_groupKind; }

	// セッタ
	inline void setDispX(int x) { m_dispX = x; }
	inline void setDispY(int y) { m_dispY = y; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }

	// HPをdamageValue分減らす。ただし0未満にはならない。最大HPを超えない。
	void damage(int damageValue);
	// 表示HPを更新
	void updateDispHp();
};


#endif