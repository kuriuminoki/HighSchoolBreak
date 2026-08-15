#include "Cell.h"
#include "Define.h"


Cell::Cell(CELL_KIND cellKind, int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor) :
	Button(x1, y1, x2, y2, edgeLength, innerColor, edgeColor)
{
	m_cellKind = cellKind;

	m_character_p = nullptr;
}


