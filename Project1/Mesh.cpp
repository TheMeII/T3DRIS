#include "Mesh.h"

Mesh::Mesh()
{
	
}

Mesh::~Mesh()
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	size = 0;
}

void Mesh::addVertices(std::vector<glm::vec3> vectorData, std::vector<unsigned int> indexData)
{
	size = indexData.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vectorData), &vectorData[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indexData), &indexData[0], GL_STATIC_DRAW);
}