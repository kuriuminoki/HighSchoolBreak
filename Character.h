#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include <string>
#include <utility>
#include <vector>


enum CHARACTER_NUM;


enum GROUP_KIND {
	NOT_ANY_GROUP,
	STUDENT,
	TEACHER,
	MONSTER
};


class Skill;
class CharacterBuff;


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
	int m_skillPoint;
	int m_maxSkillPoint;
	int m_specialPoint;
	int m_maxSpecialPoint;

public:
	CharacterStatus();

	// ゲッタ
	inline int getMaxHp() const { return m_maxHp; }
	inline int getDispHp() const { return m_dispHp; }
	inline int getHp() const { return m_hp; }
	inline int getSpeed() const { return m_speed; }
	inline int getSkillPoint() const { return m_skillPoint; }
	inline int getMaxSkillPoint() const { return m_maxSkillPoint; }
	inline int getSpecialPoint() const { return m_specialPoint; }
	inline int getMaxSpecialPoint() const { return m_maxSpecialPoint; }

	// セッタ
	inline void setDispHp(int dispHp) { m_dispHp = dispHp; }
	inline void setHp(int hp) { m_hp = hp; }
	inline void setSkillPoint(int skillPoint) { m_skillPoint = skillPoint; }
	inline void setSpecialPoint(int specialPoint) { m_specialPoint = specialPoint; }

};


class AttackInfo {
private:
	std::vector<std::pair<int, std::pair<int, int> > > m_targets; // (攻撃力, (dy, dx))

public:
	AttackInfo(int n);

	// ゲッタ
	inline const std::vector<std::pair<int, std::pair<int, int> > >& getTargets() const { return m_targets; }
};


/*
* キャラの基底クラス
*/
class Character {
private:
	static const int DISP_HP_COUNT = 120; // HPバーの表示時間

	// キャラ情報
	CharacterProfile* m_characterProfile;
	CharacterStatus* m_characterStatus;
	AttackInfo* m_attackInfo;
	std::vector<Skill*> m_skill;
	Skill* m_specialSkill;

	// 戦場での状態
	int m_dispX;
	int m_dispY;
	int m_x;
	int m_y;
	GROUP_KIND m_groupKind;
	int m_dispHpCnt; // HPが増減したときに一定時間HPバーを表示する際の時間計測
	int m_needSkillPoint; // 今手に持っているスキルを設置すると消費されるスキルポイント量
	std::vector<CharacterBuff*> m_buffs;

public:
	Character(CharacterProfile* characterProfile, CharacterStatus* characterStatus, int x, int y, GROUP_KIND groupKind);
	~Character();

	// ゲッタ
	inline const CharacterProfile* getCharacterProfile() const { return m_characterProfile; }
	inline const CharacterStatus* getCharacterStatus() const { return m_characterStatus; }
	inline const AttackInfo* getAttackInfo() const { return m_attackInfo; }
	inline std::vector<Skill*> getSkill() const { return m_skill; }
	inline Skill* getSpecialSkill() const { return m_specialSkill; }
	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getDispX() const { return m_dispX; }
	inline int getDispY() const { return m_dispY; }
	inline GROUP_KIND getGroupKind() const { return m_groupKind; }
	inline bool dispHpBar() const { return m_dispHpCnt > 0; }
	inline int getNeedSkillPoint() const { return m_needSkillPoint; }
	inline std::vector<CharacterBuff*> getBuffs() const { return m_buffs; }

	// セッタ
	inline void setDispX(int x) { m_dispX = x; }
	inline void setDispY(int y) { m_dispY = y; }
	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setNeedSkillPoint(int needSkillPoint) { m_needSkillPoint = needSkillPoint; }

	// HPをdamageValue分減らす。ただし0未満にはならない。最大HPを超えない。
	void damage(int damageValue);

	// 表示HPを更新
	void updateDispHp();

	// スキルポイントの変動
	void addSkillPoint(int addValue);

	// 必殺ポイントの変動
	void addSpecialPoint(int addValue);

	// バフ・デバフを付与
	void addBuff(CharacterBuff* buff);

	// 次のターンにするときの処理
	void nextTurn();

	// バフ・デバフを考慮した攻撃力の補正値を返す
	int calcAttackBuffValue() const;

	// バフ・デバフを考慮したスピードの補正値を返す
	int calcSpeedBuffValue() const;

	// バフ・デバフを考慮した攻撃力の補正値を返す
	int calcDefenseBuffValue() const;
};


#endif