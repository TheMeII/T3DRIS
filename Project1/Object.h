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
Vector3f translation, rotation, scale;
std::vector<objectEntry> entries;

//these are used to make copy of exsisting objects entries (that contains made transformations, rather than created setups)
//transformations are keeped uptated to vertices here, and when copy is made, these values are used to create new object.
struct objectCopy
{
std::vector<Vertex> copyVertices;
std::vector<unsigned int> copyIndices;
std::string copyTexture;
};
std::vector<objectCopy> copyData; //each object has copy data that is updated

public:
Object();
Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string texture);
void addEntry(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string texture);
void combineObjectEntries(Object combinedObject);
std::vector<Object::objectEntry> getEntries();
virtual ~Object();
void Render();
void Translate(Vector3f trans);
void Scale(Vector3f scale);
void Rotate(Vector3f rot);
void getTransforms(std::vector<Vector3f> &reference);
};