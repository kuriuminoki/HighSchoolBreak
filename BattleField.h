#ifndef BATTLE_FIELD_H_INCLUDED
#define BATTLE_FIELD_H_INCLUDED


#include <vector>


class Cell;
class Character;
class CharacterInfoButton;
class Dice;


class BattleField {
private:
	int m_columnSize; // 横サイズ
	int m_rowSize; // 縦サイズ
	std::vector<std::vector<Cell*> > m_cells; // マス cells[Y][X]
	std::vector<Character*> m_characters;
	std::vector<CharacterInfoButton*> m_characterInfoButton;

	int m_handX, m_handY;
	int m_turnNum; // 今何ターン目か
	int m_activeCharacterIndex; // 今行動中のキャラ
	Dice* m_dice; // サイコロ

public:
	BattleField();
	~BattleField();

	// ゲッタ
	inline const std::vector<std::vector<Cell*> > getCells() const { return m_cells; }
	inline const std::vector<Character*> getCharacters() const { return m_characters; }
	inline const std::vector<CharacterInfoButton*> getCharacterInfoButton() const { return m_characterInfoButton; }
	inline const Dice* getDice() const { return m_dice; }

	bool play();
};


#endif