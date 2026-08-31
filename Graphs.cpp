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

	commonPath = "picture/skillIcon/";
	for (int i = 0; i < 5; i++) {
		m_skillIconGraphs.push_back(-1);
	}
	m_skillIconGraphs[0] = LoadGraph((commonPath + "attack.png").c_str());
	m_skillIconGraphs[1] = LoadGraph((commonPath + "defence.png").c_str());
	m_skillIconGraphs[2] = LoadGraph((commonPath + "cure.png").c_str());
	m_skillIconGraphs[3] = LoadGraph((commonPath + "move.png").c_str());
	m_skillIconGraphs[4] = LoadGraph((commonPath + "other.png").c_str());

	commonPath = "picture/status/";
	for (unsigned int i = 0; i < 4; i++) {
		m_statusGraphs.push_back(-1);
	}
	m_statusGraphs[0] = LoadGraph((commonPath + "攻撃力上昇.png").c_str());
	m_statusGraphs[1] = LoadGraph((commonPath + "攻撃力下降.png").c_str());
	m_statusGraphs[2] = LoadGraph((commonPath + "スピード上昇.png").c_str());
	m_statusGraphs[3] = LoadGraph((commonPath + "スピード下降.png").c_str());
}


CharacterGraphs::~CharacterGraphs() {
	for (unsigned int i = 0; i < m_characterIconGraphs.size(); i++) {
		DeleteGraph(m_characterIconGraphs[i]);
	}
	for (unsigned int i = 0; i < m_skillIconGraphs.size(); i++) {
		DeleteGraph(m_skillIconGraphs[i]);
	}
	for (unsigned int i = 0; i < m_statusGraphs.size(); i++) {
		DeleteGraph(m_statusGraphs[i]);
	}
}


int CharacterGraphs::getCharacterIconGraphs(CHARACTER_NUM characterNum) const {
	return m_characterIconGraphs[characterNum];
}


int CharacterGraphs::getSkillIconGraphs(SKILL_CATEGORY skillCategory) const {
	return m_skillIconGraphs[skillCategory];
}


int CharacterGraphs::getStatusGraphs(BUFF_KIND buffKind) const {
	return m_statusGraphs[buffKind];
}


/*
* エフェクト画像
*/
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
