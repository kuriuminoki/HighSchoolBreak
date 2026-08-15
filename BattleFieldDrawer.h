#ifndef BATTLE_FIELD_DRAWER_H_INCLUDED
#define BATTLE_FIELD_DRAWER_H_INCLUDED


#include <vector>


class BattleField;
enum CHARACTER_NUM;


/*
* ƒLƒƒƒ‰‚Ì‰æ‘œ
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
* ‚·‚²‚ë‚­ƒQ[ƒ€‚Ì•`‰æ
*/
class BattleFieldDrawer {
private:
	BattleField* m_battleField_p;

	double m_exX, m_exY;
	int m_handX, m_handY;
	CharacterGraphs* m_characterGraphs;
	int m_font;

public:
	BattleFieldDrawer(BattleField* battleField_p);
	~BattleFieldDrawer();

	void draw();
};


#endif