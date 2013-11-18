//------------------------------------------------------------------------------
// <copyright file="SkeletonBasics.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once
#include <windows.h>
#include <string>
#include "NuiApi.h"

class CSkeletonBasics
{

public:
	typedef struct Point {
		float x;
		float y;
	}Point;

    /// <summary>
    /// Constructor
    /// </summary>
    CSkeletonBasics(int *move);

    /// <summary>
    /// Destructor
    /// </summary>
    ~CSkeletonBasics();


	//TODO: poista ikkunanluonti mutta jätä skeletonin lueskelu
    int                     Run();

private:
	int						*movement;
	Point					m_Points[NUI_SKELETON_POSITION_COUNT];
	NUI_SKELETON_FRAME		skeletons[5];
	int						skelNmbr;

    HWND                    m_hWnd;

    bool                    m_bSeatedMode;

    // Current Kinect
    INuiSensor*             m_pNuiSensor;

    // Skeletal drawing
    
    HANDLE                  m_pSkeletonStreamHandle;
    HANDLE                  m_hNextSkeletonEvent;
    
    /// <summary>
    /// Main processing function
    /// </summary>
    void                    Update();

    /// <summary>
    /// Create the first connected Kinect found 
    /// </summary>
    /// <returns>S_OK on success, otherwise failure code</returns>
    HRESULT                 CreateFirstConnected();

    /// <summary>
    /// Handle new skeleton data
    /// </summary>
    void                    ProcessSkeleton();

	CSkeletonBasics::Point CSkeletonBasics::SkeletonToScreen(Vector4 skeletonPoint, int width, int height);


};

// Safe release for interfaces
template<class Interface>
inline void SafeRelease( Interface *& pInterfaceToRelease )
{
    if ( pInterfaceToRelease != NULL )
    {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = NULL;
    }
}


