#ifndef BATTLE_FIELD_DRAWER_H_INCLUDED
#define BATTLE_FIELD_DRAWER_H_INCLUDED


class BattleField;
class CharacterGraphs;


/*
* Ç∑Ç≤ÇÎÇ≠ÉQÅ[ÉÄÇÃï`âÊ
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
	
	int m_cnt; // ì_ñ≈óp

public:
	BattleFieldDrawer(BattleField* battleField_p);
	~BattleFieldDrawer();

	void draw();
};


#endif