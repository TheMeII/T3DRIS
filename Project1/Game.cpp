#include "Game.h"


//GAME CLASS IS THE CLASS, WHERE WE HAVE ALL THE CODE CONTAINING OBJECTS THAT BELONG TO GAME, TARNSFORMATIONS(OR PHYSICS) FOR THOSE OBJECT, GAME LOGIG AND SUCH
//ALL RENDERED OBJECTS WILL BE RECIVED FROM THIS CLASS MEMBER VARIABLE, vector<Object> gameObjects

Game::Game(RenderObject *reference)
//	:gameObjects(reference)
{
	gameObjects = reference;
	setup = 0;
	t = 0.0;
	dt = 1000.0 / 60.0; //DELTA-time ms/frames(every 1/60 second)
	numberOfObjects = 0;
	activeBlock = false;
}

Game::~Game()
{

}


int Game::run()
{
	int i = 0;
	while(true)
	{
		if(activeBlock != true)
		{
			createBlock();
			activeBlock = true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds((int)dt));
	}
	return 0;
}

void Game::startThread()
{
	setupGame();
	gameThread = new std::thread(&Game::run, this);
}


void Game::createBlock()
{
	currentShape.selectShape(4,&currentShape);
	gameObjects->pushObject(currentShape.returnShape());
	numberOfObjects++;
}

void Game::setupGame()
{
	createSkybox();
	createFloor();
	
	//SKYBOX TRANSFORMATIONS
	gameObjects->renderObjects[0].Translate(Vector3f(0.0f, 0.0f, 0.0f));
	gameObjects->renderObjects[0].Scale(Vector3f(100.0f, 100.0f, 100.0f));

	//FLOOR TRANSFORMATIONS
	gameObjects->renderObjects[1].Translate(Vector3f(0.0f, 0.0f, 0.0f));
	gameObjects->renderObjects[1].Scale(Vector3f(16.0f, 1.0f, 16.0f));;

	setup = 1;
}


int Game::isSetup()
{
	int i = 0;
	return setup;
}

void Game::createSkybox()
{
	//PRODUCES TWO TRIGLES THAT FORM RECTANGLE
	//make Vector3f array
	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;

	//front
	vertices.push_back(Vertex(Vector3f(-1.0f, -1.0f, 1.0f), Vector2f(0.0f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector2f(0.25f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(0.25f, 0.3333f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector2f(0.0f, 0.3333f)));
	//top
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector2f(0.0f, 0.3333f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(0.25f, 0.3333f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, -1.0f), Vector2f(0.25f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, -1.0f), Vector2f(0.0f, 0.0f)));
	//back
	vertices.push_back(Vertex(Vector3f(1.0f, -1.0f, -1.0f), Vector2f(0.50f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, -1.0f, -1.0f), Vector2f(0.75f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, -1.0f), Vector2f(0.75f, 0.3333f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, -1.0f), Vector2f(0.50f, 0.3333f)));
	//bottom
	vertices.push_back(Vertex(Vector3f(-1.0f, -1.0f, -1.0f), Vector2f(0.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, -1.0f, -1.0f), Vector2f(0.25f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector2f(0.25f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, -1.0f, 1.0f), Vector2f(0.0f, 0.6666f)));
	//left
	vertices.push_back(Vertex(Vector3f(-1.0f, -1.0f, -1.0f), Vector2f(0.75f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, -1.0f, 1.0f), Vector2f(1.0f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector2f(1.0f, 0.3333f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, -1.0f), Vector2f(0.75f, 0.3333f)));
	//right
	vertices.push_back(Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector2f(0.25f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(1.0f, -1.0f, -1.0f), Vector2f(0.50f, 0.6666f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, -1.0f), Vector2f(0.50f, 0.3333f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(0.25f, 0.3333f)));

	unsigned int Ind[] = {
		0, 1, 2,//front-face
		2, 3, 0,
		4, 5, 6,//top-face
		6, 7, 4,
		8, 9, 10,//back
		10, 11, 8,
		12, 13, 14,//bottom
		14, 15, 12,
		16, 17, 18, //left
		18, 19, 16,
		20, 21, 22, //right
		22, 23, 20 };


	indices.assign(Ind, Ind + sizeof(Ind) / sizeof(unsigned int));
	Object oo = Object(vertices, indices, "space.png");
	
	gameObjects->pushObject(oo);
	
}

void Game::createFloor()
{
	
	//PRODUCES TWO TRIGLES THAT FORM RECTANGLE
 //make Vector3f array
	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(Vector3f(-1.0f, -1.0f, 1.0f), Vector2f(0.0f, 1.0f))); //floor
	vertices.push_back(Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector2f(1.0f, 1.0f))); //floor
	vertices.push_back(Vertex(Vector3f(1.0f, -1.0f, -1.0f), Vector2f(1.0f, 0.0f))); //floor
	vertices.push_back(Vertex(Vector3f(-1.0f, -1.0f, -1.0f), Vector2f(0.0f, 0.0f))); //floor


	unsigned int Ind[] = {
		0, 1, 2,//face
		2, 3, 0,
		};

	indices.assign(Ind, Ind + sizeof(Ind) / sizeof(unsigned int));
	Object oo = Object(vertices, indices, "floor_grid.png");
	
	gameObjects->pushObject(oo);
	
}

void Game::executeStaticTrasformations()
{

}



void Game::setSetup(bool set)
{
	if(set)
	{
		setup = 1;
	}

	if(!set)
	{
		setup = 0;
	}

}











