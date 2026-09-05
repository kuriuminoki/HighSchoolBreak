#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED


#include <string>
#include <vector>


class AttackInfo;
class Cell;
class Character;
class CharacterController;


enum SKILL_CATEGORY;
enum GROUP_KIND;


// BattleFieldクラスへの要求
enum COMMAND_TO_BF {
	NONE_REQUEST,			// 特になし
	RETRY_MOVE,				// キャラの移動パートをもう一度やってください
	SKILL_PLAY_CONTINUE		// 次のフレームでもSkillPlayerを呼んでください
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

	// スキルのターン経過ボーナス説明文
	virtual std::string getSkillBonusDesc(int turn) const = 0;

	// 発火させる。y, xはこのスキルの発動場所。
	virtual COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController, const Character* skillOwner) const { return COMMAND_TO_BF::NONE_REQUEST; }

	// 攻撃範囲を設定する。
	virtual void setDamageCell(int y, int x, std::vector<std::vector<Cell*> >& cells) const {}

	// 所属によるペナルティがあるか
	bool isGroupPenalty(int y, int x, std::vector<std::vector<Cell*> >& cells, const Character* skillOwner) const;

private:
	// ターン経過で増えるボーナスの計算
	virtual int calcTurnBonus(int turn) const { return 0; }
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

	// スキルのターン経過ボーナス説明文
	std::string getSkillBonusDesc(int turn) const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController, const Character* skillOwner) const;

private:
	// ターン経過で増えるボーナスの計算
	int calcTurnBonus(int turn) const;
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

	// スキルのターン経過ボーナス説明文
	std::string getSkillBonusDesc(int turn) const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController, const Character* skillOwner) const;

	// 攻撃範囲を設定する。
	void setDamageCell(int y, int x, std::vector<std::vector<Cell*> >& cells) const;

private:
	void putAttackInfoToCells(int y, int x, std::vector<std::vector<Cell*> >& cells, GROUP_KIND groupKind, bool attack, const Character* skillOwner) const;

	// ターン経過で増えるボーナスの計算
	int calcTurnBonus(int turn) const;
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

	// スキルのターン経過ボーナス説明文
	std::string getSkillBonusDesc(int turn) const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController, const Character* skillOwner) const;

private:
	// ターン経過で増えるボーナスの計算
	int calcTurnBonus(int turn) const;
};


/*
* 攻撃力のバフ・デバフを付与するスキル
*/
class AttackBuffSkill : public Skill
{
private:
	int m_buffTurnSum;
	int m_attackValue;

public:
	AttackBuffSkill(int needSkillPoint, int buffTurnSum, int attackValue);

	// スキルの説明文
	std::string getSkillDesc() const;

	// スキルのターン経過ボーナス説明文
	std::string getSkillBonusDesc(int turn) const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController, const Character* skillOwner) const;

private:
	// ターン経過で増えるボーナスの計算
	int calcTurnBonus(int turn) const;
};


/*
* スピードのバフ・デバフを付与するスキル
*/
class SpeedBuffSkill : public Skill
{
private:
	int m_buffTurnSum;
	int m_speedValue;

public:
	SpeedBuffSkill(int needSkillPoint, int buffTurnSum, int speedValue);

	// スキルの説明文
	std::string getSkillDesc() const;

	// スキルのターン経過ボーナス説明文
	std::string getSkillBonusDesc(int turn) const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController, const Character* skillOwner) const;

private:
	// ターン経過で増えるボーナスの計算
	int calcTurnBonus(int turn) const;
};


/*
* 防御力のバフ・デバフを付与するスキル
*/
class DefenseBuffSkill : public Skill
{
private:
	int m_buffTurnSum;
	int m_defenseValue;

public:
	DefenseBuffSkill(int needSkillPoint, int buffTurnSum, int defenseValue);

	// スキルの説明文
	std::string getSkillDesc() const;

	// スキルのターン経過ボーナス説明文
	std::string getSkillBonusDesc(int turn) const;

	// 発火させる。y, xはこのスキルの発動場所。
	COMMAND_TO_BF fire(int y, int x, std::vector<std::vector<Cell*> >& cells, CharacterController* characterController, const Character* skillOwner) const;

private:
	// ターン経過で増えるボーナスの計算
	int calcTurnBonus(int turn) const;
};


#endif