#pragma once

#include <vector>

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "math3d.h"
#include "Camera.h"
#include "texture.h"
#include "pipeline.h"
#include "InitResources.h"

class Object{
private:

struct objectEntry{ 
GLuint vbo;
GLuint ibo;
unsigned int NumIndices;
std::string textureFile;
Texture* pObjTexture;
};

Pipeline objectTransforms;
std::vector<objectEntry> entries;

public:
Object();
Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string texture);
void addEntry(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string texture);
virtual ~Object();
void Render();
void Translate(Vector3f trans);
void Scale(Vector3f scale);
void Rotate(Vector3f rot);
void getTransforms(std::vector<Vector3f> &reference);
};