#include <SFML/Graphics.hpp>
#include "Game.h"
#include <thread>

int main()
{
	Game* game = Game::GetInstance();
	sf::Event event;

	//sf::sleep(sf::seconds(1));

	//sf::Thread suka(&Game::Draw, game);
	//std::thread rendering(&Game::Draw, game);

	game->Start(false);

	while (game->m_window.isOpen())
	{
		while (game->m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				game->m_window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					game->m_window.close();

				if (event.key.code == sf::Keyboard::Left)
					if (game->MoveLeft())
						game->SpawnNewBlock();
				if (event.key.code == sf::Keyboard::Up)
					if (game->MoveUp())
						game->SpawnNewBlock();
				if (event.key.code == sf::Keyboard::Down)
					if (game->MoveDown())
						game->SpawnNewBlock();
				if (event.key.code == sf::Keyboard::Right)
					if (game->MoveRight())
						game->SpawnNewBlock();
			}
		}

		game->Draw();
		if (game->CheckEnd())
		{
			game->GameEnd();
			game->Start(true);
		}
	}

	//rendering.detach();
	return 0;
}
