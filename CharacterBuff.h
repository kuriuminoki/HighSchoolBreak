#ifndef CHARACTER_BUFF_H_INCLUDED
#define CHARACTER_BUFF_H_INCLUDED


enum BUFF_KIND;


/*
* キャラクターのバフ・デバフ
*/
class CharacterBuff {
protected:
	int m_endTurn;
	int m_nowTurn;
	BUFF_KIND m_buffKind;
public:
	CharacterBuff(int endTurn);

	// ゲッタ
	inline BUFF_KIND getBuffKind() const { return m_buffKind; }

	bool isEnd() const { return m_nowTurn >= m_endTurn; }
	void nextTurn() { m_nowTurn++; }

	virtual int getAttackBuf() const { return 0; }
	virtual int getSpeedBuf() const { return 0; }
	virtual int getDefenseBuf() const { return 0; }
};


/*
* 攻撃力のバフ・デバフ
*/
class AttackBuff : public CharacterBuff
{
private:
	int m_attackValue;
public:
	AttackBuff(int endTurn, int attackValue);

	int getAttackBuf() const { return m_attackValue; }
};


/*
* スピードのバフ・デバフ
*/
class SpeedBuff : public CharacterBuff
{
private:
	int m_speedValue;
public:
	SpeedBuff(int endTurn, int speedValue);

	int getSpeedBuf() const { return m_speedValue; }
};


/*
* 防御力のバフ・デバフ
*/
class DefenseBuff : public CharacterBuff
{
private:
	int m_defenseValue;
public:
	DefenseBuff(int endTurn, int defenseValue);

	int getDefenseBuf() const { return m_defenseValue; }
};


#endif