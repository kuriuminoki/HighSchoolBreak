#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED


#include <string>
#include <vector>


class AttackInfo;
class Cell;


enum SKILL_CATEGORY;


// BattleFieldクラスへの要求
enum COMMAND_TO_BF {
	NONE,
	RETRY_MOVE
};


/*
* スキルの基底クラス
*/
class Skill {
protected:
	std::string m_skillName;
	SKILL_CATEGORY m_skillCategory;
	int m_needSkillPoint;

public:
	Skill();
	virtual ~Skill() {}

	// ゲッタ
	inline std::string getSkillName() const { return m_skillName; }
	inline SKILL_CATEGORY getSkillCategory() const { return m_skillCategory; }
	inline int getNeedSkillPoint() const { return m_needSkillPoint; }

	// スキルの説明文
	virtual std::string getSkillDesc() const = 0;

	// 発火させる。y, xはこのスキルの発動場所。
	virtual COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells) { return COMMAND_TO_BF::NONE; }
};


/*
* 追加で移動するスキル
*/
class MoveWithoutDiceSkill : public Skill
{
private:
	int m_distance;

public:
	MoveWithoutDiceSkill(int needSkillPoint, int distance);
	~MoveWithoutDiceSkill() {}

	// スキルの説明文
	std::string getSkillDesc() const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells);
};


/*
* 追加で攻撃/回復するスキル
*/
class AdditionalAttackSkill : public Skill
{
private:
	AttackInfo* m_attackInfo;

public:
	AdditionalAttackSkill(int needSkillPoint, AttackInfo* attackInfo);
	~AdditionalAttackSkill();

	// スキルの説明文
	std::string getSkillDesc() const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells);
};


#endif