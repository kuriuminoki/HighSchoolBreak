#ifndef BATTLE_FIELD_DRAWER_H_INCLUDED
#define BATTLE_FIELD_DRAWER_H_INCLUDED


#include <vector>


class BattleField;
enum CHARACTER_NUM;


/*
* キャラの画像
*/
class CharacterGraphs {
private:
	std::vector<int> m_characterIconGraphs;
public:
	CharacterGraphs();
	~CharacterGraphs();

	int getCharacterIconGraphs(CHARACTER_NUM characterNum) const;
};


/*
* すごろくゲームの描画
*/
class BattleFieldDrawer {
private:
	BattleField* m_battleField_p;

	double m_exX, m_exY;
	int m_handX, m_handY;
	CharacterGraphs* m_characterGraphs;
	int m_font;
	int m_middleFont;
	int m_smallFont;
	
	int m_cnt; // 点滅用

public:
	BattleFieldDrawer(BattleField* battleField_p);
	~BattleFieldDrawer();

	void draw();
};


#endif