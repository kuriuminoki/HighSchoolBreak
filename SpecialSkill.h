#ifndef SPECIAL_SKILL_H_INCLUDED
#define SPECIAL_SKILL_H_INCLUDED


#include "Skill.h"
#include <string>


class AttackInfoCreator;
class CsvReader;


Skill* createSpecial(std::string specialName, CsvReader* csvReader, AttackInfoCreator* attackInfoCreator);


class AttackSpecialSkill : public AdditionalAttackSkill
{
private:
	AttackInfo* m_attackInfo;

public:
	AttackSpecialSkill(AttackInfo* attackInfo);

	std::string getSkillBonusDesc(int turn) const { return ""; }

private:
	// ターン経過で増えるボーナスの計算
	int calcTurnBonus(int turn) const { return 0; }
};


#endif