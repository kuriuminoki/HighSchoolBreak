#include "CharacterBuff.h"
#include "Graphs.h"


CharacterBuff::CharacterBuff(int endTurn) {
	m_endTurn = endTurn;
	m_nowTurn = 0;
	m_buffKind = ATTACK_UP;
}


AttackBuff::AttackBuff(int endTurn, int attackValue):
	CharacterBuff(endTurn)
{
	m_attackValue = attackValue;
	if (m_attackValue > 0) {
		m_buffKind = ATTACK_UP;
	}
	else if (m_attackValue < 0) {
		m_buffKind = ATTACK_DOWN;
	}
}


SpeedBuff::SpeedBuff(int endTurn, int speedValue) :
	CharacterBuff(endTurn)
{
	m_speedValue = speedValue;
	if (m_speedValue > 0) {
		m_buffKind = SPEED_UP;
	}
	else if (m_speedValue < 0) {
		m_buffKind = SPEED_DOWN;
	}
}
