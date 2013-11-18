#include "KinectControl.h"



int KinectControl::run()
{
	running = true;
	while (running)
	{
		skeleton->Run();
	}
	return 0;
}

void KinectControl::StartThread()
{
	kThread = new std::thread(&KinectControl::run, this);
}

int KinectControl::stop()
{
	running=false;
	return 0;
}

int KinectControl::next()
{
	
	return 0;
}

int KinectControl::push(int i)
{
	
	return 0;
}

KinectControl::KinectControl(int *move)
{
	//Koitetaan luoda Skeleton basics esimerkin avulla skeleton 
	//joka kertoo asentonsa tälle luokalle joka purkaa liikkeet gestureiksi

	skeleton = new CSkeletonBasics(move);
	

}




KinectControl::~KinectControl(void)
{
}
