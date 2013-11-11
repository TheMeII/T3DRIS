#include "KinectControl.h"
#include <thread>

#include <string>


KinectControl::KinectControl(std::string *text)
{
	//Koitetaan luoda Skeleton basics esimerkin avulla skeleton 
	//joka kertoo asentonsa t�lle luokalle joka purkaa liikkeet gestureiksi

	CSkeletonBasics *skeleton = new CSkeletonBasics(*text);
	std::thread (skeleton->Run(), this);
	

}


KinectControl::~KinectControl(void)
{
}
