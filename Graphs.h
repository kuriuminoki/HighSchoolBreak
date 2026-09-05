#ifndef GRAPHS_H_INCLUDED
#define GRAPHS_H_INCLUDED


#include <vector>


/*
* キャラ画像の種類
*/
enum CHARACTER_NUM {
	AKATSUKI,
	TOUNO,
	TAKINO,
	KAIBARA,
	KANEI
};


// アイコン画像の使い分け用
enum SKILL_CATEGORY {
	ATTACK_SKILL,			// 攻撃
	DEFENCE_SKILL,		// 防御
	CURE_SKILL,			// 回復
	MOVE_SKILL,			// 移動
	OTHER_SKILL,			// その他
	ATTACK_UP_SKILL,		// 攻撃力アップ
	ATTACK_DOWN_SKILL,	// 攻撃力ダウン
	SPEED_UP_SKILL,		// スピードアップ
	SPEED_DOWN_SKILL,		// スピードダウン
	DEFENSE_UP_SKILL,		// 防御力アップ
	DEFENSE_DOWN_SKILL	// 防御力ダウン
};


// バフ・デバフの種類
enum BUFF_KIND {
	ATTACK_UP,		// 攻撃力アップ
	ATTACK_DOWN,	// 攻撃力ダウン
	SPEED_UP,		// スピードアップ
	SPEED_DOWN,		// スピードダウン
	DEFENSE_UP,		// 防御力アップ
	DEFENSE_DOWN	// 防御力ダウン
};


/*
* キャラの画像
*/
class CharacterGraphs {
private:
	std::vector<int> m_characterIconGraphs;
	std::vector<int> m_specialCharacterGraphs;
	std::vector<int> m_skillIconGraphs;
	std::vector<int> m_statusGraphs;
public:
	CharacterGraphs();
	~CharacterGraphs();

	int getCharacterIconGraphs(CHARACTER_NUM characterNum) const;
	int getSpecialCharacterGraphs(CHARACTER_NUM characterNum) const;
	int getSkillIconGraphs(SKILL_CATEGORY skillCategory) const;
	int getStatusGraphs(BUFF_KIND buffKind) const;
};


// エフェクト画像の種類
enum EFFECT_KIND {
	ATARI_EFFECT,
	SKILL_EFFECT
};


/*
* エフェクト画像
*/
class EffectGraphs {
private:
	std::vector<int> m_atariGraphs;
	std::vector<int> m_skillGraphs;
public:
	EffectGraphs();
	~EffectGraphs();

	// ゲッタ
	int getEffectGraphs(EFFECT_KIND effectKind, int index);

	// エフェクト画像の枚数を取得
	static int getEffectSize(EFFECT_KIND effectKind) {
		switch (effectKind) {
		case ATARI_EFFECT:
			return 4;
		case SKILL_EFFECT:
			return 6;
		}
		return 0;
	}
};


#endif