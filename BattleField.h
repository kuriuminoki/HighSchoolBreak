#ifndef BATTLE_FIELD_H_INCLUDED
#define BATTLE_FIELD_H_INCLUDED


#include <vector>
#include <utility>


class Cell;
class Character;
class CharacterInfoButton;
class Dice;
class TextButton;


enum DIRECTION {
	LEFT,
	RIGHT,
	UP,
	DOWN
};


// キャラの移動 (warp=trueなら瞬間移動)
bool move(Character* character_p, int gy, int gx, std::vector<std::vector<Cell*> >& cells, bool warp);


class CharacterController {
private:
	Dice* m_dice_p; // サイコロ
	Character* m_character_p; // 操作キャラ

	std::vector<std::vector<DIRECTION> > m_routeMemo; // 各マスへ移動する際、どの方向に移動した結果到着するか
	std::vector<std::pair<int, int> > m_track; // キャラの移動予定マス 先頭がゴール地点、末尾がスタート地点
	bool m_ableFinish;

public:
	CharacterController(Dice* dice_p);

	inline void setCharacter(Character* character_p) { m_character_p = character_p; }

	void initControl();

	bool play(int handX, int handY, std::vector<std::vector<Cell*> >& cells);

private:
	void searchAllTrack(int maxDistance, std::vector<std::vector<Cell*> >& cells);
	void searchGoalRoute(int gy, int gx, std::vector<std::vector<Cell*> >& cells);
};


class BattleField {
private:
	int m_columnSize; // 横サイズ
	int m_rowSize; // 縦サイズ
	std::vector<std::vector<Cell*> > m_cells; // マス cells[Y][X]
	std::vector<Character*> m_characters;
	std::vector<CharacterInfoButton*> m_characterInfoButton;
	Dice* m_dice;
	TextButton* m_endActionButton; // 行動終了ボタン

	int m_handX, m_handY;
	int m_activeCharacterIndex; // 今行動中のキャラ
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

	// 特殊な処理をするゲッタ
	inline const Character* getActiveCharacter() const { return m_characters[m_activeCharacterIndex]; }

	bool play();
};


#endif