#include "SpecialSkill.h"
#include "AttackInfo.h"
#include "Character.h"
#include "CsvReader.h"
#include "Skill.h"

#include "DxLib.h"


using namespace std;


/*
* csvƒtƒ@ƒCƒ‹‚©‚ç“Ç‚İ‚ñ‚¾î•ñ‚ğó‚¯æ‚Á‚Ä•KE‹Z‚ğì¬
*/
Skill* createSpecial(string specialName, CsvReader* csvReader, AttackInfoCreator* attackInfoCreator) {
	Skill* res = nullptr;
	map<string, string> skillData = csvReader->findOne("name", specialName.c_str());
	if (skillData.empty()) {
		ErrorLogAdd((specialName + "‚ªspecialInfo.csv‚É‚ ‚è‚Ü‚¹‚ñB").c_str());
		return createSkill(specialName, csvReader, attackInfoCreator);
	}

	string className = skillData["class"];
	if (className == "AttackSpecialSkill") {
		string baseDamage = skillData["param1"];
		res = new AttackSpecialSkill(attackInfoCreator->createAttackInfo(specialName, baseDamage.empty() ? 0 : stoi(baseDamage)));
	}
	else {
		res = new AttackSpecialSkill(attackInfoCreator->createAttackInfo("‘oŒ•¯a", 0));
		specialName = "class not found: " + className;
		ErrorLogAdd(specialName.c_str());
	}

	res->setSkillName(specialName);
	return res;
}


AttackSpecialSkill::AttackSpecialSkill(AttackInfo* attackInfo) :
	AdditionalAttackSkill(0, attackInfo)
{
	m_skillName = "•KE‹Z–¼–¢İ’è";
}


