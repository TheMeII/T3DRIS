#pragma once
#include "InitResources.h"
#include <vector>
#include "Object.h"
#include <string>

class RenderObject
{
private:
	

public:
	int index;
	std::vector<Object> renderObjects;
	RenderObject();
	~RenderObject();
	void pushObject(Object obj);
	Object getObject(int index);
	int getSize();
};
