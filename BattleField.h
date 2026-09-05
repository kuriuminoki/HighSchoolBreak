#ifndef BATTLE_FIELD_H_INCLUDED
#define BATTLE_FIELD_H_INCLUDED


#include <vector>
#include <utility>


enum COMMAND_TO_BF;


class Cell;
class CellInfoButton;
class Character;
class CharacterInfoButton;
class Dice;
class CharacterController;
class TextButton;
class Skill;
class SkillInfoButton;
class SoundPlayer;


/*
* BattleField用の音
*/
class BattleFieldSoundHandle {
private:
	int m_decideSound;
	int m_overlapSound;
	int m_skillPlaySound;
	int m_skillDescSound;
	int m_specialDescSound;
	int m_hangingSound;
	int m_setSkillSound;
	int m_moveSound;
public:
	BattleFieldSoundHandle();
	~BattleFieldSoundHandle();

	inline int getDecideSound() const { return m_decideSound; }
	inline int getOverlapSound() const { return m_overlapSound; }
	inline int getSkillPlaySound() const { return m_skillPlaySound; }
	inline int getSkillDescSound() const { return m_skillDescSound; }
	inline int getSpecialDescSound() const { return m_specialDescSound; }
	inline int getHangingSound() const { return m_hangingSound; }
	inline int getSetSkillSound() const { return m_setSkillSound; }
	inline int getMoveSound() const { return m_moveSound; }
};


/*
* スキル発動処理
*/
class SkillPlayer {
private:
	const int PRE_PLAY_TIME = 60; // スキル発動前の時間
	const int SKILL_GUIDE_TIME = 180; // スキルの詳細を表示する時間

	bool m_isSpecial; // 必殺技ならtrue
	CharacterController* m_userController_p;
	SoundPlayer* m_soundPlayer_p;
	const BattleFieldSoundHandle* m_soundHandle_p;

	const Skill* m_skill_p;
	const Character* m_skillOwner_p;
	int m_cnt;
	int m_x, m_y;
	int m_turn;

public:
	SkillPlayer(bool isSpecial, CharacterController* userController, SoundPlayer* soundPlayer_p, const BattleFieldSoundHandle* soundHandle_p);

	COMMAND_TO_BF play(std::vector<std::vector<Cell*> >& cells);

	inline const Skill* getSkill() const { return m_skill_p; }
	inline const Character* getSkillOwner() const { return m_skillOwner_p; }
	inline int getTurn() const { return m_turn; }
	inline bool isSpecial() const { return m_isSpecial; }
	int getCnt() const { return isPrePlaySpan() ? m_cnt : m_cnt - PRE_PLAY_TIME; }
	inline bool isPrePlaySpan() const { return m_cnt < PRE_PLAY_TIME; }
};


/*
* すごろくゲーム
*/
class BattleField {
private:
	const int ATTACK_FREEZE_TIME = 60; // 攻撃時の時間停止時間

	SoundPlayer* m_soundPlayer_p;

	int m_columnSize; // 横サイズ
	int m_rowSize; // 縦サイズ
	std::vector<std::vector<Cell*> > m_cells; // マス cells[Y][X]
	std::vector<Character*> m_characters;
	std::vector<CharacterInfoButton*> m_characterInfoButton;
	CellInfoButton* m_cellInfoButton;
	SkillInfoButton* m_skillInfoButton;
	Dice* m_dice;
	TextButton* m_endActionButton; // 行動終了ボタン
	BattleFieldSoundHandle* m_soundHandle;

	int m_handX, m_handY;
	int m_activeCharacterIndex; // 今行動中のキャラ
	bool m_alreadyAttack; // 攻撃したか
	CharacterController* m_characterController; // キャラ移動用
	std::pair<Skill*, const Character*> m_hangingSkill_pair; // 今設置しようとしているスキルとその所持キャラ
	SkillPlayer* m_skillPlayer;
	int m_freezeTime;

public:
	BattleField(SoundPlayer* soundPlayer_p);
	~BattleField();

	// ゲッタ
	inline const std::vector<std::vector<Cell*> > getCells() const { return m_cells; }
	inline const std::vector<Character*> getCharacters() const { return m_characters; }
	inline const std::vector<CharacterInfoButton*> getCharacterInfoButton() const { return m_characterInfoButton; }
	inline const CellInfoButton* getCellInfoButton() const { return m_cellInfoButton; }
	inline const SkillInfoButton* getSkillInfoButton() const { return m_skillInfoButton; }
	inline const Dice* getDice() const { return m_dice; }
	inline const TextButton* getEndActionButton() const { return m_endActionButton; }
	inline const Skill* getHangingSkill() const { return m_hangingSkill_pair.first; }
	inline const SkillPlayer* getSkillPlayer() const { return m_skillPlayer; }

	// 特殊な処理をするゲッタ
	inline const Character* getActiveCharacter() const { return m_characters[m_activeCharacterIndex]; }

	bool play();

private:
	void playCharacterMove();
	void updateBattleField();

	void initCells();
	void nextTurn();
	void initController(); // 操作キャラの種類に合わせてControllerを作成
	void setDamageCell(int y, int x, const Character* character_p); // 座標y, xから攻撃したときの範囲を設定
	void damageCharacterEachCell(); // 各セルについて設定されたダメージをキャラに適用
	bool ableSpecialSkill(const Character* character); // 必殺技を発動できるか判定
};


#endif