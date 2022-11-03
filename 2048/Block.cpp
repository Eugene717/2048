#include "Block.h"
#include "Game.h"

bool Block::m_texturesLoad = false;
static sf::Texture m_textures[11];

void Block::LoadTextures()
{
	m_textures[0].loadFromFile("resourses/2.png");
	m_textures[1].loadFromFile("resourses/4.png");
	m_textures[2].loadFromFile("resourses/8.png");
	m_textures[3].loadFromFile("resourses/16.png");
	m_textures[4].loadFromFile("resourses/32.png");
	m_textures[5].loadFromFile("resourses/64.png");
	m_textures[6].loadFromFile("resourses/128.png");
	m_textures[7].loadFromFile("resourses/256.png");
	m_textures[8].loadFromFile("resourses/512.png");
	m_textures[9].loadFromFile("resourses/1024.png");
	m_textures[10].loadFromFile("resourses/2048.png");

	m_texturesLoad = true;
}

Block::Block(const int& x, const int& y)
{	
	if (m_texturesLoad == false)
		LoadTextures();

	Game* game = Game::GetInstance();

	if (game->m_gen() % 10 > 2)
	{
		m_number = 2;
		m_square.setTexture(&m_textures[0]);
	}
	else
	{
		m_number = 4;
		m_square.setTexture(&m_textures[1]);
	}

	m_square.setOutlineThickness(1);
	m_square.setOutlineColor(sf::Color::Black);
	m_square.setSize(sf::Vector2f(98, 98));
	m_square.setOrigin(50, 50);
	m_square.setPosition(26 + 110 * (y + 1), 55 + 110 * (x + 1));

	m_numberText.setFont(*game->GetFont());
	m_numberText.setFillColor(sf::Color::Black);
	m_numberText.setCharacterSize(50);
	m_numberText.setString("" + std::to_string(m_number));
	m_numberText.setOrigin(m_numberText.getGlobalBounds().width / 2, m_numberText.getGlobalBounds().height / 2);
	m_numberText.setPosition(m_square.getGlobalBounds().left + 50, m_square.getGlobalBounds().top + 40);

	//game->Draw();
	//for (float i = 0.0; i <= 1.0; i+=0.1)
	//{
	//	m_square.setScale(i, i);
	//	m_numberText.setScale(i, i);

	//	//game->Draw();
	//	game->m_window.draw(m_square);
	//	game->m_window.draw(m_numberText);
	//	game->m_window.display();
	//	sf::sleep(sf::milliseconds(20));
	//}
}

void Block::Place(const int& x, const int& y)
{
	//Game* game = Game::GetInstance();

	m_square.setPosition(26 + 110 * (y + 1), 55 + 110 * (x + 1));
	m_numberText.setOrigin(m_numberText.getGlobalBounds().width / 2, m_numberText.getGlobalBounds().height / 2);
	m_numberText.setPosition(m_square.getGlobalBounds().left + 50, m_square.getGlobalBounds().top + 40);
}

void Block::Merge()
{
	m_number *= 2;
	m_numberText.setString(std::to_string(m_number));
	m_numberText.setOrigin(m_numberText.getGlobalBounds().width / 2, m_numberText.getGlobalBounds().height / 2);
	m_numberText.setPosition(m_square.getGlobalBounds().left + 50, m_square.getGlobalBounds().top + 40);
	if (m_number >= 8)
		m_numberText.setFillColor(sf::Color::White);

	for (int i = 1, j = 4; i < 11; i++, j *= 2)
	{
		if (m_number == j)
			m_square.setTexture(&m_textures[i]);
	}
}

void Block::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Game* game = Game::GetInstance();

	game->m_window.draw(m_square);
	game->m_window.draw(m_numberText);
}

int Block::GetNumber() const
{
	return m_number;
}