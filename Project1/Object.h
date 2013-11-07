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
GLuint vbo;
GLuint ibo;
unsigned int NumIndices;
Pipeline objectTransforms;
std::string textureFile;
Texture* pObjTexture;


public:
Object();
Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string texture);
virtual ~Object();
void Render();
void Translate(Vector3f trans);
void Scale(Vector3f scale);
void Rotate(Vector3f rot);
void getTransforms(std::vector<Vector3f> &reference);
};