#include "Character.h"
#include "CharacterBuff.h"
#include "Define.h"
#include "Graphs.h"
#include "DxLib.h"


#include <set>
#include <string>
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


// 保持しているバフ・デバフのアイコンを並べて描画
int drawBuffs(int x, int y, double ex, vector<CharacterBuff*> buffs, const CharacterGraphs* characterGraphs, int dispLimit) {
	set<BUFF_KIND> alreadyDisp;
	int buffX = 0;
	int buffWide = 0;
	GetGraphSize(characterGraphs->getStatusGraphs(ATTACK_UP), &buffWide, NULL);
	buffWide = (int)(buffWide * ex);
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


// 文字列を領域の真ん中に描画したい時の座標を取得
void getDispCenterStrPos(int* x, int* y, int x1, int y1, int x2, int y2, int font, string dispStr) {
	int fontWide = GetDrawStringWidthToHandle(dispStr.c_str(), (int)dispStr.size(), font);
	*x = x1 + (x2 - x1 - fontWide) / 2;

	int fontHeight = 0;
	GetFontStateToHandle(NULL, &fontHeight, NULL, font);
	*y = y1 + (y2 - y1 - fontHeight) / 2;
}


// 角度と長さから座標を求める
void calcXYfromTheta(int theta, int length, int* x, int* y) {
	double rad = theta * 3.14 / 180;
	int dx = (int)(length * cos(rad));
	int dy = (int)(length * sin(rad));
	*x = *x + dx;
	*y = *y + dy;
}


const int getRateLength(int maxLength, int value, int maxValue) {
	return (maxLength * value) / maxValue;
}


void drawChartLine(int x, int y, int length, int thickness, int color, bool dispName, int font) {
	int hpX = x, hpY = y;
	calcXYfromTheta(30, length, &hpX, &hpY);
	int powerX = x, powerY = y;
	calcXYfromTheta(90, length, &powerX, &powerY);
	int defenseX = x, defenseY = y;
	calcXYfromTheta(150, length, &defenseX, &defenseY);
	int speedX = x, speedY = y;
	calcXYfromTheta(210, length, &speedX, &speedY);
	int intelligenceX = x, intelligenceY = y;
	calcXYfromTheta(270, length, &intelligenceX, &intelligenceY);
	int teamworkX = x, teamworkY = y;
	calcXYfromTheta(330, length, &teamworkX, &teamworkY);
	DrawLine(hpX, hpY, powerX, powerY, color, thickness);
	DrawLine(powerX, powerY, defenseX, defenseY, color, thickness);
	DrawLine(defenseX, defenseY, speedX, speedY, color, thickness);
	DrawLine(speedX, speedY, intelligenceX, intelligenceY, color, thickness);
	DrawLine(intelligenceX, intelligenceY, teamworkX, teamworkY, color, thickness);
	DrawLine(teamworkX, teamworkY, hpX, hpY, color, thickness);

	if (dispName) {
		int fontSize = 0;
		GetFontStateToHandle(NULL, &fontSize, NULL, font);
		DrawStringToHandle(hpX, hpY, "体力", color, font);
		DrawStringToHandle(powerX - (int)(fontSize * 1.5), powerY, "攻撃力", color, font);
		DrawStringToHandle(defenseX - fontSize * 3, defenseY, "防御力", color, font);
		DrawStringToHandle(speedX - fontSize * 2, speedY - fontSize, "走力", color, font);
		DrawStringToHandle(intelligenceX - fontSize, intelligenceY - fontSize, "知力", color, font);
		DrawStringToHandle(teamworkX, teamworkY - fontSize, "連携力", color, font);
	}
}


void drawChart(int x, int y, double ex, int font, int color, int maxColor, const CharacterStatus* status) {
	const int LENGTH = (int)(150 * ex);
	drawChartLine(x, y, LENGTH / 2, 2, maxColor, false, font);

	int hpX = x, hpY = y;
	calcXYfromTheta(30, getRateLength(LENGTH, status->getMaxHp(), CharacterStatus::MAX_HP), &hpX, &hpY);
	int powerX = x, powerY = y;
	calcXYfromTheta(90, getRateLength(LENGTH, status->getPower(), CharacterStatus::MAX_POWER), &powerX, &powerY);
	int defenseX = x, defenseY = y;
	calcXYfromTheta(150, getRateLength(LENGTH, status->getDefense(), CharacterStatus::MAX_DEFENSE), &defenseX, &defenseY);
	int speedX = x, speedY = y;
	calcXYfromTheta(210, getRateLength(LENGTH, status->getSpeed(), CharacterStatus::MAX_SPEED), &speedX, &speedY);
	int intelligenceX = x, intelligenceY = y;
	calcXYfromTheta(270, getRateLength(LENGTH, status->getIntelligence(), CharacterStatus::MAX_INTELLIGENCE), &intelligenceX, &intelligenceY);
	int teamworkX = x, teamworkY = y;
	calcXYfromTheta(330, getRateLength(LENGTH, status->getTeamWork(), CharacterStatus::MAX_TEAMWORK), &teamworkX, &teamworkY);

	DrawTriangle(x, y, hpX, hpY, powerX, powerY, color, TRUE);
	DrawTriangle(x, y, powerX, powerY, defenseX, defenseY, color, TRUE);
	DrawTriangle(x, y, defenseX, defenseY, speedX, speedY, color, TRUE);
	DrawTriangle(x, y, speedX, speedY, intelligenceX, intelligenceY, color, TRUE);
	DrawTriangle(x, y, intelligenceX, intelligenceY, teamworkX, teamworkY, color, TRUE);
	DrawTriangle(x, y, teamworkX, teamworkY, hpX, hpY, color, TRUE);

	drawChartLine(x, y, LENGTH, 4, maxColor, true, font);

	DrawCircle(x, y, 3, RED, TRUE);
}
