#pragma once
#include <SFML/Graphics.hpp>

const int SIDE_SIZE = 100;

class Block : public sf::Drawable
{
	static bool m_texturesLoad;

	int m_number;
	sf::RectangleShape m_square;
	sf::Text m_numberText;

	void LoadTextures();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	void Place(const int& x, const int& y);
	void Merge();
	int GetNumber() const;
	Block(const int& x, const int& y);
};

