#include "Cell.h"
#include "Define.h"


Cell::Cell(CELL_KIND cellKind, int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor) :
	Button(x1, y1, x2, y2, edgeLength, innerColor, edgeColor)
{
	m_cellKind = cellKind;

	m_character_p = nullptr;
	m_markingColor = -1;
}


void Cell::draw(int handX, int handY, bool fill) const {
	if (overlap(handX, handY)) {
		DrawBox(m_x1 - m_edgeLength, m_y1 - m_edgeLength, m_x2 + m_edgeLength, m_y2 + m_edgeLength, m_edgeColor, TRUE);
	}
	int color = m_markingColor != -1 ? m_markingColor : m_innerColor;
	DrawBox(m_x1, m_y1, m_x2, m_y2, color, TRUE);
}


bool Cell::ableMoving() {
	return m_character_p == nullptr;
}
