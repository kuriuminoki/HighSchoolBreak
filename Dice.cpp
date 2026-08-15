#include "Dice.h"
#include "Button.h"
#include "DxLib.h"

#include <sstream>


using namespace std;


Dice::Dice(int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor):
	Button(x1, y1, x2, y2, edgeLength, innerColor, edgeColor)
{
	m_cnt = 0;
	m_value = 1;
}


void Dice::init(int maxValue, int minValue, int goalCount) {
	m_maxValue = maxValue;
	m_minValue = minValue;
	m_cnt = goalCount;
}


bool Dice::play() {
	if (m_cnt <= 0) {
		return true;
	}
	m_cnt--;
	m_value = GetRand(m_maxValue - m_minValue) + 1 + m_minValue;
	return false;
}


void Dice::draw(int handX, int handY, int font, int color) const {
	Button::draw(handX, handY, true);

	ostringstream oss;
	oss << m_value;
	int fontSize = 0;
	GetFontStateToHandle(NULL, &fontSize, NULL, font);
	DrawStringToHandle((m_x1 + m_x2) / 2 - fontSize, (m_y1 + m_y2) / 2 - fontSize / 2, oss.str().c_str(), color, font);
}
