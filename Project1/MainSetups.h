#pragma once

#include <ctime>
#include <string>

//WINDOW GLOBALS
int g_iWindowWidth = 512;
int g_iWindowHeight = 512;
int g_iGLUTWindowHandle = 0;

//ERROR GLOBALS
int g_iErrorCode = 0;

//ROTATION GLOBALS TO KEP TRACK OF ROTATIONS
float g_fRotate1 = 0.0f;
float g_fRotate2 = 0.0f;
float g_fRotate3 = 0.0f;

//RENDERING SCENE CONSTANTS AND STARTING SCENE GLOBAL
enum ESceneType
{
	ST_Scene1 = 0,
	ST_Scene2,
	ST_Scene3,
	ST_Scene4,
    ST_NumScenes
};
ESceneType g_eCurrentScene = ST_Scene1;

//Time between frames globals
std::clock_t g_PreviousTime;
std::clock_t g_CurrentTime;

//WINDOW AND INPUT FUNCTIONS / INIT CALLBACK FUNCTIONS

//Initialize dispaly window
void InitGL(int argc, char* argv[], std::string name); 
//Rendering function, invoked by GLUT(freeglut)
void DisplayGL();
//Idle is invoked, when windows system events are not recieved
void IdleGL();
//Callback method for keyboard inputs
void SetKeyboardGL(unsigned char c, int x, int y);
//Mouse press detection
void SetMouseGL(int button, int state, int x, int y);
//Detect mouse movement while button pressed
void SetMotionGL(int x, int y);
//Setup window size /projection matrix
void ReshapeWindowGL(int width, int height);

//SHAPE FUNCTIONS TODO:MAKE OWN CLASS FOR SHAPES LATER
//2D
void DrawTriangle(vec2f p1, vec2f p2, vec2f p3);
void DrawRectangle(float width, float height);
void DrawCircle(float radius, int numSides);
//3D
void DrawCube(float width, float height, float depth);

//SCENE FUNCTIONS
void RenderScene1();
void RenderScene2();
void RenderScene3();
void RenderScene4();

//Cleanup function for cleaning resources
void Cleanup(int exitCode, bool bExit = true);