#include "CharacterBuff.h"
#include "Define.h"
#include "Graphs.h"
#include "DxLib.h"


#include <set>
#include <vector>


using namespace std;


// HPバーを表示
void drawHpBar(int x1, int y1, int x2, int y2, int hp, int nowHp, int maxHp) {
	DrawBox(x1, y1, x2, y2, BLACK, TRUE);
	int wide = x2 - x1;
	int hpWide = wide * hp / maxHp;
	int nowHpWide = wide * nowHp / maxHp;
	DrawBox(x1, y1, x1 + nowHpWide, y2, RED, TRUE);
	DrawBox(x1, y1, x1 + hpWide, y2, GREEN, TRUE);
}


// スキルポイントのバーを表示
void drawSkillPointBar(int x1, int y1, int x2, int y2, int skillPoint, int maxSkillPoint, int needSkillPoint) {
	DrawBox(x1, y1, x2, y2, BLACK, TRUE);
	int wide = x2 - x1;
	int skillWide = wide * skillPoint / maxSkillPoint;
	DrawBox(x1, y1, x1 + skillWide, y2, ORANGE, TRUE);
	int needSkillPointWide = wide * needSkillPoint / maxSkillPoint;
	if (skillPoint < needSkillPoint) {
		DrawBox(x1, y1, x1 + skillWide, y2, RED, TRUE);
	}
	else {
		DrawBox(x1 + skillWide - needSkillPointWide, y1, x1 + skillWide, y2, WHITE, TRUE);
	}
}


int drawBuffs(int x, int y, double ex, vector<CharacterBuff*> buffs, const CharacterGraphs* characterGraphs, int dispLimit) {
	set<BUFF_KIND> alreadyDisp;
	int buffX = 0;
	int buffWide = 0;
	GetGraphSize(characterGraphs->getStatusGraphs(ATTACK_UP), &buffWide, NULL);
	buffWide = (int)buffWide * ex;
	for (unsigned int i = 0; i < buffs.size(); i++) {
		if (dispLimit <= 0) {
			break;
		}
		BUFF_KIND buffKind = buffs[i]->getBuffKind();
		if (alreadyDisp.find(buffKind) == alreadyDisp.end()) {
			DrawRotaGraph(x + buffX + buffWide, y + buffWide, ex, 0.0, characterGraphs->getStatusGraphs(buffKind), TRUE);
			buffX += buffWide;
			dispLimit--;
			alreadyDisp.insert(buffKind);
		}
	}
	return buffWide;
}