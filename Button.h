#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED


/*
* ボタンの基底クラス
*/
class Button {
protected:
	int m_x1, m_y1, m_x2, m_y2;
	int m_edgeLength;
	int m_innerColor, m_edgeColor;

	double m_exX, m_exY;
	bool m_ableClick;
	int m_offColor;

public:
	Button(int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor);

	// ゲッタ
	inline int getX1() const { return m_x1; }
	inline int getY1() const { return m_y1; }
	inline int getX2() const { return m_x2; }
	inline int getY2() const { return m_y2; }
	inline bool getAbleClick() const { return m_ableClick; }

	// ボタンのON/OFF
	inline void on() { m_ableClick = true; }
	inline void off(int color) { m_ableClick = false; m_offColor = color; }

	// カーソルが重なっているか
	inline bool overlap(int handX, int handY) const { return  m_ableClick && handX > m_x1 && handX < m_x2 && handY > m_y1 && handY < m_y2; }

	// 描画
	virtual void draw(int handX, int handY, bool fill) const;
};


#include <string>

class Cell;
class Character;
class CharacterGraphs;


/*
* テキスト付きのボタン
*/
class TextButton : public Button
{
private:
	std::string m_dispText;

public:
	TextButton(std::string dispText, int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor);

	void draw(int handX, int handY, bool fill, int font, int textColor) const;
};


/*
* キャラ情報
*/
class CharacterInfoButton : public Button
{
private:
	const Character* m_character_p;

public:
	CharacterInfoButton(int x1, int y1, int x2, int y2, const Character* character_p);

	// セッタ
	inline void setCharacter(const Character* character_p) { m_character_p = character_p; }

	void draw(int handX, int handY, const CharacterGraphs* characterGraphs, int font) const;
};


/*
* マス情報
*/
class CellInfoButton : public Button
{
private:
	const Cell* m_cell_p;

public:
	CellInfoButton(int x1, int y1, int x2, int y2, const Cell* cell_p);

	// セッタ
	inline void setCell(const Cell* cell_p) { m_cell_p = cell_p; }

	void draw(int handX, int handY, const CharacterGraphs* characterGraphs, int font) const;
};


#endif