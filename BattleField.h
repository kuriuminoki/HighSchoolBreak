#ifndef BATTLE_FIELD_H_INCLUDED
#define BATTLE_FIELD_H_INCLUDED


#include <vector>
#include <utility>


class Cell;
class Character;
class CharacterInfoButton;
class Dice;
class CharacterController;
class TextButton;


class BattleField {
private:
	int m_columnSize; // 横サイズ
	int m_rowSize; // 縦サイズ
	std::vector<std::vector<Cell*> > m_cells; // マス cells[Y][X]
	std::vector<Character*> m_characters;
	std::vector<CharacterInfoButton*> m_characterInfoButton;
	Dice* m_dice;
	TextButton* m_endActionButton; // �s���I���{�^��

	int m_handX, m_handY;
	int m_activeCharacterIndex; // ���s�����̃L����
	CharacterController* m_characterController;

public:
	BattleField();
	~BattleField();

	// ゲッタ
	inline const std::vector<std::vector<Cell*> > getCells() const { return m_cells; }
	inline const std::vector<Character*> getCharacters() const { return m_characters; }
	inline const std::vector<CharacterInfoButton*> getCharacterInfoButton() const { return m_characterInfoButton; }
	inline const Dice* getDice() const { return m_dice; }
	inline const TextButton* getEndActionButton() const { return m_endActionButton; }

	// ����ȏ���������Q�b�^
	inline const Character* getActiveCharacter() const { return m_characters[m_activeCharacterIndex]; }

	bool play();

private:
	void initController();
};


#endif