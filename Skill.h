#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED


#include <string>
#include <vector>


class AttackInfo;
class Cell;
class CharacterController;


enum SKILL_CATEGORY;
enum GROUP_KIND;


// BattleFieldクラスへの要求
enum COMMAND_TO_BF {
	NONE_REQUEST,
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
	virtual COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController) const { return COMMAND_TO_BF::NONE_REQUEST; }

	// 攻撃範囲を設定する。
	virtual void setDamageCell(int y, int x, std::vector<std::vector<Cell*> >& cells) const {}
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
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController) const;

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
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController) const;

	// 攻撃範囲を設定する。
	void setDamageCell(int y, int x, std::vector<std::vector<Cell*> >& cells) const;

private:
	void putAttackInfoToCells(int y, int x, std::vector<std::vector<Cell*> >& cells, GROUP_KIND groupKind, bool attack) const;
};


/*
* 踏んだ敵にダメージを与える(罠)スキル
*/
class DefenceSkill : public Skill
{
private:
	int m_damage;

public:
	DefenceSkill(int needSkillPoint, int damage);

	// スキルの説明文
	std::string getSkillDesc() const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController) const;
};


#endif