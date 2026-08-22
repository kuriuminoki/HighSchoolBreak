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


// エフェクト画像の種類
enum EFFECT_KIND {
	ATARI
};


/*
* エフェクト画像
*/
class EffectGraphs {
private:
	std::vector<int> m_atariGraphs;
public:
	EffectGraphs();
	~EffectGraphs();

	// ゲッタ
	int getEffectGraphs(EFFECT_KIND effectKind, int index);

	// エフェクト画像の枚数を取得
	static int getEffectSize(EFFECT_KIND effectKind) {
		switch (effectKind) {
		case ATARI:
			return 4;
		}
		return 0;
	}
};


#endif