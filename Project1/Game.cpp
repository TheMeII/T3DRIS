#include "Game.h"


Game::Game()
{
	t = 0.0;
	dt = 1000.0 / 60.0; //DELTA-time ms/frames(every 1/60 second)
}

Game::~Game()
{

}


int Game::run()
{
	int i = 0;
	while(true)
	{
		i++;
		std::cout << "Hello " << i <<  std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	return 0;
}

void Game::startThread()
{
	gameThread = new std::thread(&Game::run, this);
}


