#ifndef DRAW_UTILS_H_INCLUDED
#define DRAW_UTILS_H_INCLUDED


#include <vector>


class CharacterBuff;
class CharacterGraphs;


// HPバーの描画
void drawHpBar(int x1, int y1, int x2, int y2, int hp, int nowHp, int maxHp);

// スキルポイントのバーを表示
void drawSkillPointBar(int x1, int y1, int x2, int y2, int skillPoint, int maxSkillPoint, int needSkillPoint);

// バフ・デバフのアイコンを描画
int drawBuffs(int x, int y, double ex, std::vector<CharacterBuff*> buffs, const CharacterGraphs* characterGraphs, int dispLimit);

#endif