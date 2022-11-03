#pragma once

#include <SFML/Graphics.hpp>
#include <random>

struct GameIMPL;
class Block;

class Game
{
	GameIMPL* m_pImpl;
	static Game* m_game;

	Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

public:
	sf::RenderWindow m_window;
	std::default_random_engine m_gen;

	static Game* GetInstance();
	sf::Font* GetFont() const;

	void Draw();
	void SpawnNewBlock();
	bool MoveLeft(const bool& cantMove = false);
	bool MoveUp(const bool& cantMove = false);
	bool MoveRight(const bool& cantMove = false);
	bool MoveDown(const bool& cantMove = false);
	bool CheckEnd();
	void Start(const bool& restart);
	void GameEnd();
};

