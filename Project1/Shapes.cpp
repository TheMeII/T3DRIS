#include "Shapes.h"
using namespace std; 


	
	Shapes::Shapes(){
	vector<unsigned int> indices;
	vector<Vertex> vertices;
	//front
	vertices.push_back(Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(0.0f, 1.0f, 1.0f), Vector2f(0.0f, 1.0f)));
	//top
	vertices.push_back(Vertex(Vector3f(0.0f, 1.0f, 1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 0.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.0f, 1.0f)));
	//back
	vertices.push_back(Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 0.0f), Vector2f(0.0f, 1.0f)));
	//bottom
	vertices.push_back(Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector2f(0.0f, 1.0f)));
	//left
	vertices.push_back(Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(0.0f, 1.0f, 1.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector2f(0.0f, 1.0f)));
	//right
	vertices.push_back(Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 0.0f, 0.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 0.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(0.0f, 1.0f)));

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
	shape = Object(vertices, indices, "cube_white.png");
	
		
}
	
	//vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector2f(0.0f, 0.0f)));

 void Shapes::AddToShape()
{

	vector<unsigned int> indices;
	vector<Vertex> vertices;

	//front
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 3.0f, 1.0f), Vector2f(1.0f, 1.0f)));//luodaan yhden laatikon ylemmäs
	vertices.push_back(Vertex(Vector3f(-1.0f, 3.0f, 1.0f), Vector2f(0.0f, 1.0f)));
	//top
	vertices.push_back(Vertex(Vector3f(-1.0f, 3.0f, 1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 3.0f, 1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 3.0f, -1.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 3.0f, -1.0f), Vector2f(0.0f, 1.0f)));
	//back
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, -1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, -1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 3.0f, -1.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 3.0f, -1.0f), Vector2f(0.0f, 1.0f)));
	//bottom
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, -1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, -1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector2f(0.0f, 1.0f)));
	//left
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, -1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 3.0f, 1.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(-1.0f, 3.0f, -1.0f), Vector2f(0.0f, 1.0f)));
	//right
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector2f(0.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 1.0f, -1.0f), Vector2f(1.0f, 0.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 3.0f, -1.0f), Vector2f(1.0f, 1.0f)));
	vertices.push_back(Vertex(Vector3f(1.0f, 3.0f, 1.0f), Vector2f(0.0f, 1.0f)));

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
	
	Shapes::shape.addEntry(vertices, indices, "cube_white.png");
	
}
 Object Shapes::returnShape(){
 return shape;
 
 }