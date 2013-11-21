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



