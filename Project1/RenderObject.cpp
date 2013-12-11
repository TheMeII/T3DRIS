#include "RenderObject.h"

RenderObject::RenderObject()
{
	
}


RenderObject::~RenderObject()
{

}



void RenderObject::pushObject(Object obj)
{
	renderObjects.push_back(obj);
}


Object RenderObject::getObject(int index)
{
	return renderObjects[index];
}


int RenderObject::getSize()
{
	return (int)renderObjects.size();
}

<<<<<<< HEAD
void RenderObject::doTransforms()
{
	for(int i = 0; i < renderObjects.size()-1; i++)
	{
		renderObjects[i].Translate(Vector3f(objectTransforms[i].posX, objectTransforms[i].posY, objectTransforms[i].posZ));
		renderObjects[i].Scale(Vector3f(objectTransforms[i].rotX, objectTransforms[i].rotY, objectTransforms[i].rotZ));
		renderObjects[i].Rotate(Vector3f(objectTransforms[i].sclX, objectTransforms[i].sclY, objectTransforms[i].sclZ));
	}

}

void RenderObject::sendTransforms(float pX, float pY, float pZ, float rX, float rY, float rZ, float sX, float sY, float sZ)
{
	transforms trans;
	trans.posX = pX;
	trans.posY = pY;
	trans.posZ = pZ;
	trans.rotX = rX;
	trans.rotY = rY;
	trans.rotZ = rZ;
	trans.sclX = sX;
	trans.sclY = sY;
	trans.sclZ = sZ;
	objectTransforms.push_back(trans);
}


=======
>>>>>>> 0009d5ff6d2a855db6f9e3ad25d4a9ab3c285cbd


