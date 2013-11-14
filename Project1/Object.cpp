#include "Object.h"

Object::Object()
{
	translation = Vector3f(0.0f, 0.0f, 0.0f);
	rotation = Vector3f(0.0f, 0.0f, 0.0f);
	scale = Vector3f(1.0f, 1.0f, 1.0f);
	objectEntry entry;
	entry.pObjTexture = NULL;
	entry.NumIndices = NULL; //this is needed for render
	entry.textureFile = "";
	entry.vbo = 0;
	entry.ibo = 0;
}

Object::Object(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string texture)
{
	translation = Vector3f(0.0f, 0.0f, 0.0f);
	rotation = Vector3f(0.0f, 0.0f, 0.0f);
	scale = Vector3f(1.0f, 1.0f, 1.0f);
	objectEntry entry;
	entry.pObjTexture = NULL;
	entry.NumIndices = indices.size(); //this is needed for render
	entry.textureFile = texture;
	entry.vbo = 0;
	entry.ibo = 0;

	objectCopy copy;
	copy.copyVertices = vertices;
	copy.copyIndices = indices;
	copy.copyTexture = texture;

	glGenBuffers(1, &entry.vbo); //generate buffer, 1param, number of generated buffers, 2param, adress to array of GLuints that will store handles that GPU allocates 
	glBindBuffer(GL_ARRAY_BUFFER, entry.vbo); //bind handle to target name, so we can execute comands to that target
											//1Param, tells what buffer contains (array of vertices), and the second is the generated buffer handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW); //After bind, fill buffer with data.
				//Params: Target buffer name, size of data, data to be stored, define GL_STATIC_DRAWW to imply, that we are not going to chage buffer contents

	glGenBuffers(1, &entry.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entry.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * entry.NumIndices, &indices[0], GL_STATIC_DRAW);

	if(entry.pObjTexture == NULL)
	{
		entry.pObjTexture = new Texture(GL_TEXTURE_2D, entry.textureFile);
	}
	if(!entry.pObjTexture->Load())
	{
			exit(0);
	}

	entries.push_back(entry);
	copyData.push_back(copy);
	
}

void Object::addEntry(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string texture)
{
	objectEntry entry;
	entry.pObjTexture = NULL;
	entry.NumIndices = indices.size(); //this is needed for render
	entry.textureFile = texture;

	objectCopy copy;
	copy.copyVertices = vertices;
	copy.copyIndices = indices;
	copy.copyTexture = texture;

	glGenBuffers(1, &entry.vbo); //generate buffer, 1param, number of generated buffers, 2param, adress to array of GLuints that will store handles that GPU allocates 
	glBindBuffer(GL_ARRAY_BUFFER, entry.vbo); //bind handle to target name, so we can execute comands to that target
											//1Param, tells what buffer contains (array of vertices), and the second is the generated buffer handle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW); //After bind, fill buffer with data.
				//Params: Target buffer name, size of data, data to be stored, define GL_STATIC_DRAWW to imply, that we are not going to chage buffer contents

	glGenBuffers(1, &entry.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entry.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * entry.NumIndices, &indices[0], GL_STATIC_DRAW);

	if(entry.pObjTexture == NULL)
	{
		entry.pObjTexture = new Texture(GL_TEXTURE_2D, entry.textureFile);
	}
	if(!entry.pObjTexture->Load())
	{
			exit(0);
	}

	entries.push_back(entry);
	copyData.push_back(copy);
}


void Object::Render()
{
	glEnableVertexAttribArray(0); //position data
	glEnableVertexAttribArray(1); //texture
	for(unsigned int i = 0; i < entries.size(); i++)
	{
		
		glBindBuffer(GL_ARRAY_BUFFER, entries[i].vbo); //bind vbo  buffer before draw
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); //Tells rendering pipeline how to interpret data inside buffer
					//Params: 1:Index of attribute (0 as in enable), 2:number of component in attribute (3, x,y,z), 3:data type of each component (float), 
					//4:will data be normalized before pipeline, 5: called stride, means numebr of bytes between two instances in buffer(0, cause only one vertex) 
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); //last is offset between start of vertex structure to texture attributes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[i].ibo); //Bind indices buffer before draw
		entries[i].pObjTexture->Bind(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, entries[i].NumIndices, GL_UNSIGNED_INT, 0); //draw content/state that is set before with buffers and such
			//Params: Type to render, number of indices in index buffer, type of each index, offset in bytes from start of index buffer to location of the first index to scan
	}
	glDisableVertexAttribArray(0); //disable vertex attribute, when not used in while. Good practice
	glDisableVertexAttribArray(1);
	
}

void Object::combineObjectEntries(Object combinedObject)
{
	for(int i = 0; i < combinedObject.entries.size(); i++)
	{
		entries.push_back(combinedObject.entries[i]);
	}

}

std::vector<Object::objectEntry> Object::getEntries()
{
	return entries;
}


void Object::Translate(Vector3f trans)
{
	translation.x = trans.x;
	translation.y = trans.y;
	translation.z = trans.z;
}

void Object::Scale(Vector3f scale)
{
	this->scale.x = scale.x;
	this->scale.y = scale.y;
	this->scale.z = scale.z;
}

void Object::Rotate(Vector3f rot)
{
	rotation.x = rot.x;
	rotation.y = rot.y;
	rotation.z = rot.z;
}

void Object::getTransforms(std::vector<Vector3f> &reference)
{
	reference.push_back(translation);
	reference.push_back(rotation);
	reference.push_back(scale);
}


Object::~Object()
{

}

