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
	int getNumberOfObjects();
	void incNumberOfObjects();
	bool getCreateNew();
	void setCreateNew(bool set);
	//public for now to be accesed from engine to render
	RenderObject *gameObjects; //pointer to EngineFiles objects
	//TODO: OWN VECTOR FOR STATIC CONTENT/FLOOR/SKYBOX?
	float posX, posZ;
	float rotX, rotY;
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
	bool createNew;
	std::vector<Shapes> clippedShapes;
	bool blockReady;
	unsigned int movementTimer;
public:
	void setPosX(float pX);
	void setPosZ(float pZ);
	void setRotX(float rX);
	void setRotY(float rY);
};