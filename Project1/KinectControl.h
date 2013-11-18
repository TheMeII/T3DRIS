#pragma once
#include <windows.h>
#include "SkeletonBasics.h"
#include <iostream>
#include <thread>

class KinectControl
{
public:
	KinectControl();
	~KinectControl(void);
	int run();
	int stop();
	int next();
private:
	int push(int i);
	bool running;
	CSkeletonBasics *skeleton;
};

