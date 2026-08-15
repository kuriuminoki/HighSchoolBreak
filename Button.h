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

public:
	Button(int x1, int y1, int x2, int y2, int edgeLength, int innerColor, int edgeColor);

	// ゲッタ
	inline int getX1() const { return m_x1; }
	inline int getY1() const { return m_y1; }
	inline int getX2() const { return m_x2; }
	inline int getY2() const { return m_y2; }

	// カーソルが重なっているか
	inline bool overlap(int handX, int handY) const { return  handX > m_x1 && handX < m_x2 && handY > m_y1 && handY < m_y2; }

	// 描画
	virtual void draw(int handX, int handY, bool fill) const;
};


class Character;
class CharacterGraphs;


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

	void draw(int handX, int handY, bool fill, const CharacterGraphs* characterGraphs) const;
};


#endif