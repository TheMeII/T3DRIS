//------------------------------------------------------------------------------
// <copyright file="SkeletonBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include <strsafe.h>
#include <iostream>
#include <string>
#include "SkeletonBasics.h"

static const float g_JointThickness = 3.0f;
static const float g_TrackedBoneThickness = 6.0f;
static const float g_InferredBoneThickness = 1.0f;


/// <summary>
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
//{
//    CSkeletonBasics application;
//    application.Run(hInstance, nCmdShow);
//}

/// <summary>
/// Constructor
/// </summary>
CSkeletonBasics::CSkeletonBasics(int *move) :
	m_hNextSkeletonEvent(INVALID_HANDLE_VALUE),
	m_pSkeletonStreamHandle(INVALID_HANDLE_VALUE),
	m_bSeatedMode(false),
	m_pNuiSensor(NULL)
{
	movement = move;
	connectFailed = false;
	skelNmbr=0;

}

/// <summary>
/// Destructor
/// </summary>
CSkeletonBasics::~CSkeletonBasics()
{
	if (m_pNuiSensor)
	{
		m_pNuiSensor->NuiShutdown();
	}

	if (m_hNextSkeletonEvent && (m_hNextSkeletonEvent != INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hNextSkeletonEvent);
	}

	// clean up Direct2D objects


	// clean up Direct2D


	SafeRelease(m_pNuiSensor);
}

/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int CSkeletonBasics::Run()
{
	MSG       msg = {0};
	WNDCLASS  wc  = {0};



	const int eventCount = 1;
	HANDLE hEvents[eventCount];
	int kinectTry =0;
	while(CreateFirstConnected()<0){
		kinectTry++;
		if (kinectTry>1000) {
			connectFailed = true;
			break;
		}
	}
	// TODO: ulkopuolinen käsky kuolemiselle, kun ei luoda tälle omaa ikkunaa
	while (!connectFailed)
	{
		hEvents[0] = m_hNextSkeletonEvent;
		//std::cerr << "update?";
		// Check to see if we have either a message (by passing in QS_ALLEVENTS)
		// Or a Kinect event (hEvents)
		// Update() will check for Kinect events individually, in case more than one are signalled
		//MsgWaitForMultipleObjects(eventCount, hEvents, FALSE, INFINITE, QS_ALLINPUT);

		// Explicitly check the Kinect frame event since MsgWaitForMultipleObjects
		// can return for other reasons even though it is signaled.
		Update();


	}

	return static_cast<int>(msg.wParam);
}

/// <summary>
/// Main processing function
/// </summary>
void CSkeletonBasics::Update()
{
	if (NULL == m_pNuiSensor)
	{
		std::cout << "0";
		return;
	}

	// Wait for 0ms, just quickly test if it is time to process a skeleton
	if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0) )
	{
		std::cout << "1";
		ProcessSkeleton();
	}
}


/// <summary>
/// Create the first connected Kinect found 
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT CSkeletonBasics::CreateFirstConnected()
{
	INuiSensor * pNuiSensor;
	std::cout << "Create first." << std::endl;
	int iSensorCount = 0;
	HRESULT hr = NuiGetSensorCount(&iSensorCount);
	if (FAILED(hr))
	{
		return hr;
	}

	// Look at each Kinect sensor
	for (int i = 0; i < iSensorCount; ++i)
	{
		// Create the sensor so we can check status, if we can't create it, move on to the next
		hr = NuiCreateSensorByIndex(i, &pNuiSensor);
		if (FAILED(hr))
		{
			continue;
		}

		// Get the status of the sensor, and if connected, then we can initialize it
		hr = pNuiSensor->NuiStatus();
		if (S_OK == hr)
		{
			m_pNuiSensor = pNuiSensor;
			break;
		}

		// This sensor wasn't OK, so release it since we're not using it
		pNuiSensor->Release();
	}

	if (NULL != m_pNuiSensor)
	{
		// Initialize the Kinect and specify that we'll be using skeleton
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON); 
		if (SUCCEEDED(hr))
		{
			// Create an event that will be signaled when skeleton data is available
			m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

			// Open a skeleton stream to receive skeleton data
			hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0); 
		}
	}

	if (NULL == m_pNuiSensor || FAILED(hr))
	{

		std::cerr << "No kinects found";
		return E_FAIL;
	}

	return hr;
}

/// <summary>
/// Handle new skeleton data
/// </summary>
void CSkeletonBasics::ProcessSkeleton()
{
	NUI_SKELETON_FRAME skeletonFrame = {0};

	HRESULT hr = m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
	if ( FAILED(hr) )
	{
		return;
	}

	// smooth out the skeleton data
	m_pNuiSensor->NuiTransformSmooth(&skeletonFrame, NULL);
	if (skelNmbr >=5) {
		for (int i=0;i<4;i++)
			skeletons[i]= skeletons[i+1];
		skelNmbr=4;
	}
	try {
		skeletons[skelNmbr] = skeletonFrame; // 4 if not one of first 4 times
		skelNmbr++;
	} catch (...) {
		std::cout << "Some error" << std::endl;
	}

	for (int i = 0 ; i < NUI_SKELETON_COUNT; ++i)
	{
		for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j)
		{
			m_Points[j] = SkeletonToScreen(skeletonFrame.SkeletonData[i].SkeletonPositions[j], 640, 480);
		}

		if (skelNmbr >=5) {
			for (int i=0;i<4;i++)
				m_PointArray[i]= m_PointArray[i+1];
			skelNmbr=4;
		}
		m_PointArray[skelNmbr] = m_Points;
		skelNmbr++;

		NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;

		if (NUI_SKELETON_TRACKED == trackingState && skelNmbr>=5)
		{
			*movement = 0;

			/*	Head left right movement
			*	Compare head.x to shoulderCenter.x
			*	
			*/
			std::cout << "Head:"<< m_Points[NUI_SKELETON_POSITION_HEAD].x << " Shoulder:"<< m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x << std::endl;
			if (!(m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x - m_Points[NUI_SKELETON_POSITION_HEAD].x <12 && m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x-m_Points[NUI_SKELETON_POSITION_HEAD].x >-12)) 
			{ 
				if (m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x > m_Points[NUI_SKELETON_POSITION_HEAD].x) 
				{
					*movement |= 1;
				} else
				{
					*movement |= 2;
				}
			}
			/*	Right hand movement
			*	wrist position tracking
			*	
			*/
			float rWristy	= m_Points[NUI_SKELETON_POSITION_WRIST_RIGHT].y-m_Points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y;
			float rWristx	= m_Points[NUI_SKELETON_POSITION_WRIST_RIGHT].x-m_Points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x;
			float rShoulderx = m_Points[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x-m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x;

			//Movement Up
			if (rWristy<-30)
				//			if (m_Points[NUI_SKELETON_POSITION_WRIST_RIGHT].y<220)
			{
				*movement |=4;
			}
			//Movement Down
			if (rWristy>10)
				//			if (m_Points[NUI_SKELETON_POSITION_WRIST_RIGHT].y>260)
			{
				*movement |=8;
				std::cout << *movement << std::endl;
			}
			//Movement Left
			if (rWristx - rShoulderx < 0)
				*movement |=32;
			//Movement Right
			if (rWristx - rShoulderx > 20)
				*movement |=16;

			/*	Left hand movement
			*	wrist position tracking
			*	
			*/
			float lWristy	= m_Points[NUI_SKELETON_POSITION_WRIST_LEFT].y-m_Points[NUI_SKELETON_POSITION_SHOULDER_LEFT].y;
			float lWristx	= m_Points[NUI_SKELETON_POSITION_WRIST_LEFT].x-m_Points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x;
			float lShoulderx = m_Points[NUI_SKELETON_POSITION_SHOULDER_LEFT].x-m_Points[NUI_SKELETON_POSITION_SHOULDER_CENTER].x;

			//Movement Up
			if (lWristy<-30)
			{
				*movement |=64;
			}
			//Movement Down
			if (lWristy>10)
			{
				*movement |=128;
				std::cout << *movement << std::endl;
			}
			//Movement Left
			if (lWristx - lShoulderx < 0)
				*movement |=256;
			//Movement LEFT
			if (lWristx - lShoulderx > 20)
				*movement |=512;


			std::cout << *movement << std::endl;
		}
		else if (NUI_SKELETON_POSITION_ONLY == trackingState)
		{
			//Not tracking skeleton, don't move anything and inform it's an error ( negative values are errors )
			*movement = -1;


		}
	}


}

// <summary>
// Draws a skeleton
// </summary>
// <param name="skel">skeleton to draw</param>
// <param name="windowWidth">width (in pixels) of output buffer</param>
// <param name="windowHeight">height (in pixels) of output buffer</param>

CSkeletonBasics::Point CSkeletonBasics::SkeletonToScreen(Vector4 skeletonPoint, int width, int height)
{
	LONG x, y;
	USHORT depth;
	Point pp;
	// Calculate the skeleton's position on the screen
	// NuiTransformSkeletonToDepthImage returns coordinates in NUI_IMAGE_RESOLUTION_320x240 space
	NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);

	pp.x = static_cast<float>((x * width) / 320);
	pp.y = static_cast<float>((y * height) / 240);

	return pp;
}


