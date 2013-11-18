#include "KinectControl.h"


#include <string>


int KinectControl::run()
{
	running = true;
	while (running)
	{
		


	}
	return 0;
}

int KinectControl::stop()
{
	
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

KinectControl::KinectControl()
{
	//Koitetaan luoda Skeleton basics esimerkin avulla skeleton 
	//joka kertoo asentonsa tälle luokalle joka purkaa liikkeet gestureiksi

	skeleton = new CSkeletonBasics();
	

}




KinectControl::~KinectControl(void)
{
}
