#ifndef ATTACK_INFO_H_INCLUDED
#define ATTACK_INFO_H_INCLUDED


#include <map>
#include <string>
#include <utility>
#include <vector>


/*
* 1マス分の攻撃情報
*/
class AttackElement {
private:
	int m_damage;
	int m_dx;
	int m_dy;
public:
	AttackElement(int damage, int dx, int dy);

	// ゲッタ
	inline int getDamage() const { return m_damage; }
	inline int getDx() const { return m_dx; }
	inline int getDy() const { return m_dy; }
};


/*
* 攻撃情報
*/
class AttackInfo {
private:
	std::vector<AttackElement*> m_elements;

public:
	AttackInfo();
	~AttackInfo();

	// ゲッタ
	inline const std::vector<AttackElement*>& getAttackElement() const { return m_elements; }

	void pushAttackElement(AttackElement* attackElement) { m_elements.push_back(attackElement); }
};


/*
* csvファイルから攻撃情報を読み込む
*/
class AttackInfoCreator {
private:
	std::map<std::string, std::vector<std::vector<std::string> > > m_data; // スキル名をkeyに攻撃のマス情報(二次元配列)を持つ

public:
	AttackInfoCreator();

	AttackInfo* createAttackInfo(std::string attackName, int baseDamage);
};


#endif