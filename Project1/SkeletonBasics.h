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
    /// <summary>
    /// Constructor
    /// </summary>
    CSkeletonBasics(std::string *t);

    /// <summary>
    /// Destructor
    /// </summary>
    ~CSkeletonBasics();


	//TODO: poista ikkunanluonti mutta jätä skeletonin lueskelu
    int                     Run();

private:
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
