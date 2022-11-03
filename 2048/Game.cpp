#include "Game.h"
#include "Block.h"
#include <fstream>

Game* Game::m_game = nullptr;

struct GameIMPL
{
	sf::Font m_font;
	int m_score = 0;
	int m_bestScore = 0;
	Block* m_board[4][4];
	GameIMPL();
};

GameIMPL::GameIMPL()
{
	m_font.loadFromFile("resourses/arial.ttf");

	std::ifstream fin("resourses/score.dat");
	if (fin.is_open())
		fin >> m_bestScore;
}

Game::Game()
{
	m_pImpl = new GameIMPL;
	std::random_device rd;
	m_gen.seed(rd());

	m_window.create(sf::VideoMode(600, 600), "2048");
}

Game* Game::GetInstance()
{
	if (m_game == nullptr)
		m_game = new Game();
	return m_game;
}

void Game::Start(const bool& restart)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{			
			if (restart)
			{
				if (m_pImpl->m_board[i][j] != nullptr)
					delete m_pImpl->m_board[i][j];
			}

			m_pImpl->m_board[i][j] = nullptr;
		}
	}

	SpawnNewBlock();
	SpawnNewBlock();

	if (restart)
	{
		m_pImpl->m_score = 0;
	}
}

void Game::SpawnNewBlock()
{
	int x, y;

	while (true)
	{
		x = m_gen() % 4;
		y = m_gen() % 4;

		if (m_pImpl->m_board[x][y] == nullptr)
			break;
	}

	m_pImpl->m_board[x][y] = new Block(x, y);
}

void Game::Draw()
{
	sf::Text* score = new sf::Text;
	score->setFont(m_pImpl->m_font);
	score->setCharacterSize(32);
	score->setFillColor(sf::Color::Black);
	score->setString("Score: " + std::to_string(m_pImpl->m_score));
	score->setPosition(18, 10);

	sf::Text* bestScore = new sf::Text;
	bestScore->setFont(m_pImpl->m_font);
	bestScore->setCharacterSize(32);
	bestScore->setFillColor(sf::Color::Black);
	bestScore->setString("Best score: " + std::to_string(m_pImpl->m_bestScore));
	bestScore->setPosition(85, 50);

	sf::RectangleShape* frame = new sf::RectangleShape;
	frame->setOutlineThickness(4);
	frame->setOutlineColor(sf::Color::Black);
	frame->setFillColor(sf::Color(230, 230, 230));
	frame->setSize(sf::Vector2f(440, 440));
	frame->setPosition(80, 110);

	sf::RectangleShape* innerFrame = new sf::RectangleShape;
	innerFrame->setOutlineThickness(1);
	innerFrame->setOutlineColor(sf::Color::Black);
	innerFrame->setFillColor(sf::Color(230, 230, 230));
	innerFrame->setSize(sf::Vector2f(110, 110));

	sf::Context context;

	//while (m_window.isOpen())
	//{
		m_window.clear(sf::Color(230, 230, 230));

		m_window.draw(*score);
		m_window.draw(*bestScore);
		m_window.draw(*frame);

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				innerFrame->setPosition(80 + 110 * i, 110 + 110 * j);
				m_window.draw(*innerFrame);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (m_pImpl->m_board[i][j] != nullptr)
					m_window.draw(*m_pImpl->m_board[i][j]);
			}
		}

		m_window.display();
	//}
}

bool Game::MoveLeft(const bool& cantMove)
{
	bool moved = false;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_pImpl->m_board[j][i] != nullptr)
			{
				int number = m_pImpl->m_board[j][i]->GetNumber();
				for (int k = i + 1; k < 4; k++)
				{
					if (m_pImpl->m_board[j][k] != nullptr)
					{
						if (number == m_pImpl->m_board[j][k]->GetNumber())
						{
							if (cantMove == true)
								return false;

							m_pImpl->m_board[j][i]->Merge();
							delete m_pImpl->m_board[j][k];
							m_pImpl->m_board[j][k] = nullptr;
							moved = true;
							m_pImpl->m_score += number * 2;
						}
						break;
					}
				}
			}
			else
			{
				for (int k = i + 1; k < 4; k++)
				{
					if (m_pImpl->m_board[j][k] != nullptr)
					{
						m_pImpl->m_board[j][k]->Place(j, i);
						m_pImpl->m_board[j][i] = m_pImpl->m_board[j][k];
						m_pImpl->m_board[j][k] = nullptr;
						moved = true;
						--i;
						break;
					}
				}
			}
		}
	}

	if (cantMove == true)
		return true;
	else
		return moved;
}

bool Game::MoveUp(const bool& cantMove)
{
	bool moved = false;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_pImpl->m_board[i][j] != nullptr)
			{
				int number = m_pImpl->m_board[i][j]->GetNumber();
				for (int k = i + 1; k < 4; k++)
				{
					if (m_pImpl->m_board[k][j] != nullptr)
					{
						if (number == m_pImpl->m_board[k][j]->GetNumber())
						{
							if (cantMove == true)
								return false;

							m_pImpl->m_board[i][j]->Merge();
							delete m_pImpl->m_board[k][j];
							m_pImpl->m_board[k][j] = nullptr;
							moved = true;
							m_pImpl->m_score += number * 2;
						}
						break;
					}
				}
			}
			else
			{
				for (int k = i + 1; k < 4; k++)
				{
					if (m_pImpl->m_board[k][j] != nullptr)
					{
						m_pImpl->m_board[k][j]->Place(i, j);
						m_pImpl->m_board[i][j] = m_pImpl->m_board[k][j];
						m_pImpl->m_board[k][j] = nullptr;
						moved = true;
						--i;
						break;
					}
				}
			}
		}
	}

	if (cantMove == true)
		return true;
	else
		return moved;
}

bool Game::MoveRight(const bool& cantMove)
{
	bool moved = false;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 3; i >= 0; i--)
		{
			if (m_pImpl->m_board[j][i] != nullptr)
			{
				int number = m_pImpl->m_board[j][i]->GetNumber();
				for (int k = i - 1; k >= 0; k--)
				{
					if (m_pImpl->m_board[j][k] != nullptr)
					{
						if (number == m_pImpl->m_board[j][k]->GetNumber())
						{
							if (cantMove == true)
								return false;

							m_pImpl->m_board[j][i]->Merge();
							delete m_pImpl->m_board[j][k];
							m_pImpl->m_board[j][k] = nullptr;
							moved = true;
							m_pImpl->m_score += number * 2;
						}
						break;
					}
				}
			}
			else
			{
				for (int k = i - 1; k >= 0; k--)
				{
					if (m_pImpl->m_board[j][k] != nullptr)
					{
						m_pImpl->m_board[j][k]->Place(j, i);
						m_pImpl->m_board[j][i] = m_pImpl->m_board[j][k];
						m_pImpl->m_board[j][k] = nullptr;
						moved = true;
						++i;
						break;
					}
				}
			}
		}
	}

	if (cantMove == true)
		return true;
	else
		return moved;
}

bool Game::MoveDown(const bool& cantMove)
{
	bool moved = false;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 3; i >= 0; i--)
		{
			if (m_pImpl->m_board[i][j] != nullptr)
			{
				int number = m_pImpl->m_board[i][j]->GetNumber();
				for (int k = i - 1; k >= 0; k--)
				{
					if (m_pImpl->m_board[k][j] != nullptr)
					{
						if (number == m_pImpl->m_board[k][j]->GetNumber())
						{
							if (cantMove == true)
								return false;

							m_pImpl->m_board[i][j]->Merge();
							delete m_pImpl->m_board[k][j];
							m_pImpl->m_board[k][j] = nullptr;
							moved = true;
							m_pImpl->m_score += number * 2;
						}
						break;
					}
				}
			}
			else
			{
				for (int k = i - 1; k >= 0; k--)
				{
					if (m_pImpl->m_board[k][j] != nullptr)
					{
						m_pImpl->m_board[k][j]->Place(i, j);
						m_pImpl->m_board[i][j] = m_pImpl->m_board[k][j];
						m_pImpl->m_board[k][j] = nullptr;
						moved = true;
						++i;
						break;
					}
				}
			}
		}
	}

	if (cantMove == true)
		return true;
	else
		return moved;
}

bool Game::CheckEnd()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_pImpl->m_board[i][j] == nullptr)
				return false;
		}
	}

	if (!MoveLeft(true))
		return false;
	if (!MoveUp(true))
		return false;
	if (!MoveRight(true))
		return false;
	if (!MoveDown(true))
		return false;

	return true;
}

void Game::GameEnd()
{
	m_window.clear(sf::Color(230, 230, 230));

	sf::Text score;
	score.setFont(m_pImpl->m_font);
	score.setCharacterSize(32);
	score.setFillColor(sf::Color::Black);
	score.setString("Best score: " + std::to_string(m_pImpl->m_bestScore));
	score.setPosition(18, 10);

	m_window.draw(score);

	score.setString("Score: " + std::to_string(m_pImpl->m_score));
	score.setPosition(85, 50);

	m_window.draw(score);

	sf::RectangleShape frame;
	frame.setOutlineThickness(4);
	frame.setOutlineColor(sf::Color::Black);
	frame.setFillColor(sf::Color(230, 230, 230));
	frame.setSize(sf::Vector2f(440, 440));
	frame.setPosition(80, 110);

	m_window.draw(frame);

	frame.setOutlineThickness(1);
	frame.setSize(sf::Vector2f(110, 110));

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			frame.setPosition(80 + 110 * i, 110 + 110 * j);
			m_window.draw(frame);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_pImpl->m_board[i][j] != nullptr)
				m_window.draw(*m_pImpl->m_board[i][j]);
		}
	}

	sf::RectangleShape fade;
	fade.setSize(sf::Vector2f(600, 600));
	fade.setFillColor(sf::Color(0, 0, 0, 150));

	m_window.draw(fade);

	m_window.display();

	sf::sleep(sf::seconds(1));
}

sf::Font* Game::GetFont() const
{
	return &m_pImpl->m_font;
}