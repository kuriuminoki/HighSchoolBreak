#include "Graphs.h"
#include "DxLib.h"


#include <string>
#include <sstream>


using namespace std;


/*
* キャラの画像
*/
CharacterGraphs::CharacterGraphs() {
	string commonPath = "picture/icon/";
	for (int i = 0; i < 5; i++) {
		m_characterIconGraphs.push_back(-1);
	}
	m_characterIconGraphs[0] = LoadGraph((commonPath + "アカツキ.png").c_str());
	m_characterIconGraphs[1] = LoadGraph((commonPath + "トウノ.png").c_str());
	m_characterIconGraphs[2] = LoadGraph((commonPath + "タキノ.png").c_str());
	m_characterIconGraphs[3] = LoadGraph((commonPath + "カイバラ.png").c_str());
	m_characterIconGraphs[4] = LoadGraph((commonPath + "カンエイ.png").c_str());
}


CharacterGraphs::~CharacterGraphs() {

}


int CharacterGraphs::getCharacterIconGraphs(CHARACTER_NUM characterNum) const {
	return m_characterIconGraphs[characterNum];
}


EffectGraphs::EffectGraphs() {
	for (int i = 0; i < getEffectSize(ATARI); i++) {
		ostringstream oss;
		oss << "picture/effect/orange" << i + 1 << ".png";
		m_atariGraphs.push_back(LoadGraph(oss.str().c_str()));
	}
}


EffectGraphs::~EffectGraphs() {
	for (unsigned i = 0; i < m_atariGraphs.size(); i++) {
		DeleteGraph(m_atariGraphs[i]);
	}
}


// ゲッタ
int EffectGraphs::getEffectGraphs(EFFECT_KIND effectKind, int index) {
	if (getEffectSize(effectKind) < index) { return -1; }
	switch (effectKind) {
	case ATARI:
		return m_atariGraphs[index];
	}
	return -1;
}
