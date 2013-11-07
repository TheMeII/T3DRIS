#pragma once

#include <GL\freeglut.h>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

class Mesh{
private:
	GLuint VBO; //VERTEX BUFFER OBJECT pointer
	GLuint IBO;
	int size;
public:
	Mesh();
	~Mesh();
	void addVertices(std::vector<glm::vec3> vectorData, std::vector<unsigned int> indexData);

};