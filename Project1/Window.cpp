#include "Window.h"

#include <string>

#include <GL\freeglut.h>



void MakeWindow(int argc, char** argv, std::string windowname) //Create basic window with freeglut
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(windowname.c_str()); 

	initGlutCallbacks();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutMainLoop();
}

static void initGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
}

static void RenderSceneCB() //TODO: Put to diffrent file (make renderer class even?)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}


void resizeWindow(int width, int height)
{

}