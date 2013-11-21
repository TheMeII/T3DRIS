#pragma once

#include <Windows.h>
#include <thread>
#include <vector>
#include <iostream>
#include "Shapes.h"
#include "Object.h"
#include "RenderObject.h"


class Game{
public:
	Game(RenderObject *reference);
	~Game();
	int run(); //game loop
	void startThread();
	Object getObject(int index);
	int isSetup();
	void setSetup(bool set);
	//public for now to be accesed from engine to render
	RenderObject *gameObjects; //pointer to EngineFiles objects
	//TODO: OWN VECTOR FOR STATIC CONTENT/FLOOR/SKYBOX?
private:
	std::thread* gameThread;
	double t;
	double dt; //DELTA-time

	void createSkybox();
	void executeStaticTrasformations();
	void createFloor();
	void setupGame();
	//OBJECT CREATIONS
	void createBlock();
	int setup;
	/*//next two intialized at the beginning of each game, and not modified after that
	Object gameboard; //static board that is in bottom
	Object skybox; //skybox that is around gamespace
	*/
	Shapes currentShape;

	int numberOfObjects;

	//GAME LOGIC VARIABLES
	bool activeBlock; //USED TO DETERMINE IF NEW BLOCK IS NEEDED TO BE CREATED // IS THERE A ACTIVE BLOCK
	std::vector<Shapes> clippedShapes;
};