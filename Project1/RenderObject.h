#pragma once
#include "InitResources.h"
#include <vector>
#include "Object.h"
#include <string>

class RenderObject
{
private:
	
	struct transforms{
		float posX;
		float posY;
		float posZ;
		float rotX;
		float rotY;
		float rotZ;
		float sclX;
		float sclY;
		float sclZ;
	};

public:
	int index;
	std::vector<Object> renderObjects;
	RenderObject();
	~RenderObject();
	void pushObject(Object obj);
	Object getObject(int index);
	int getSize();
	void doTransforms();
	void sendTransforms(float pX, float pY, float pZ, float rX, float rY, float rZ, float sX, float sY, float sZ);
	std::vector<transforms> objectTransforms;
};
