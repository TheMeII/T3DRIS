#pragma once
#include "InitResources.h"
#include <vector>
#include "Object.h"
#include <string>

class RenderObject
{
private:
	
<<<<<<< HEAD
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
=======
>>>>>>> 0009d5ff6d2a855db6f9e3ad25d4a9ab3c285cbd

public:
	int index;
	std::vector<Object> renderObjects;
	RenderObject();
	~RenderObject();
	void pushObject(Object obj);
	Object getObject(int index);
	int getSize();
<<<<<<< HEAD
	void doTransforms();
	void sendTransforms(float pX, float pY, float pZ, float rX, float rY, float rZ, float sX, float sY, float sZ);
	std::vector<transforms> objectTransforms;
=======
>>>>>>> 0009d5ff6d2a855db6f9e3ad25d4a9ab3c285cbd
};
