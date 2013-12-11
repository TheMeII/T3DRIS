#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <queue>
#include "NuiApi.h"
#include "SkeletonBasics.h"
class KinectControl
{
public:
	KinectControl(int *move);
	~KinectControl();
	int run();
	void StartThread();
	int stop();
	int next();
private:
	std::thread* kThread;
	int push(int i);
	std::queue<int> moves;
	bool running;
	CSkeletonBasics *skeleton;
};

