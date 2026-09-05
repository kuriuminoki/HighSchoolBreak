#ifndef CHARACTER_CONTROLLER_H_INCLUDED
#define CHARACTER_CONTROLLER_H_INCLUDED


#include <vector>

class Cell;
class Character;
class Dice;
class SoundPlayer;
class BattleFieldSoundHandle;


enum DIRECTION {
	LEFT,
	RIGHT,
	UP,
	DOWN
};


// キャラの移動 (warp=trueなら瞬間移動)
bool move(Character* character_p, int gy, int gx, std::vector<std::vector<Cell*> >& cells, bool warp, bool ableAddSkillPoint, SoundPlayer* soundPlayer_p, const BattleFieldSoundHandle* soundHandle_p);


/*
* キャラのコントローラ
*/
class CharacterController {
protected:
	Dice* m_dice_p; // サイコロ
	SoundPlayer* m_soundPlayer_p;
	const BattleFieldSoundHandle* m_soundHandle_p;

	Character* m_character_p; // 操作キャラ
	std::vector<std::vector<DIRECTION> > m_routeMemo; // 各マスへ移動する際、どの方向に移動した結果到着するか
	std::vector<std::pair<int, int> > m_track; // キャラの移動予定マス 先頭がゴール地点、末尾がスタート地点

	bool m_ableAddSkillPoint; // スキルで追加移動したときはスキルポイントの追加なし

public:
	CharacterController(Dice* dice_p, SoundPlayer* soundPlayer_p, const BattleFieldSoundHandle* soundHandle_p);

	inline const Character* getCharacter() const { return m_character_p; }

	inline void setCharacter(Character* character_p) { m_character_p = character_p; }

	virtual void initControl();

	virtual bool play(int handX, int handY, std::vector<std::vector<Cell*> >& cells) = 0;

	virtual void moveSpecificDistance(int distance, std::vector<std::vector<Cell*> >& cells); // スキルでＸマス以内の移動をするとき用

	virtual bool isWatingGoalSelect() { return true; }

protected:
	void searchAllTrack(int maxDistance, std::vector<std::vector<Cell*> >& cells);
	void searchGoalRoute(int gy, int gx, std::vector<std::vector<Cell*> >& cells);
};


/*
* 生徒操作
*/
class StudentController : public CharacterController
{
private:
	bool m_ableFinish;

public:
	StudentController(Dice* dice_p, SoundPlayer* soundPlayer_p, const BattleFieldSoundHandle* soundHandle_p);

	void initControl();

	bool play(int handX, int handY, std::vector<std::vector<Cell*> >& cells);

	void moveSpecificDistance(int distance, std::vector<std::vector<Cell*> >& cells);

	bool isWatingGoalSelect();
};


/*
* 敵操作
*/
class EnemyController : public CharacterController
{
private:
	enum ENEMY_STATE {
		INIT_DICE,
		PLAY_DICE,
		MOVING
	};
	ENEMY_STATE m_state;

public:
	EnemyController(Dice* dice_p, SoundPlayer* soundPlayer_p, const BattleFieldSoundHandle* soundHandle_p);

	void initControl();

	bool play(int handX, int handY, std::vector<std::vector<Cell*> >& cells);

	void moveSpecificDistance(int distance, std::vector<std::vector<Cell*> >& cells);

	bool isWatingGoalSelect() { return true; }
};


#endif