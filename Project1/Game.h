#pragma once

#include <Windows.h>
#include <thread>
#include <vector>
#include <iostream>
#include "Shapes.h"
#include "Object.h"


class Game{
public:
	Game();
	~Game();
	int run(); //game loop
	void startThread();
private:
	std::thread* gameThread;
	double t;
	double dt; //DELTA-time

	//next two intialized at the beginning of each game, and not modified after that
	Object gameboard; //static board that is in bottom
	Object skybox; //skybox that is around gamespace

	std::vector<Shapes> clippedShapes;
	Shapes currentShape;

};