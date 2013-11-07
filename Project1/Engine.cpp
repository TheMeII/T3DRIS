#define _USE_MATH_DEFINES

#include<GL\glew.h>
//STANTARD INCLUDES
#include <math.h>
#include <iostream>
#include <string>

//OPENGL INCLUDES
#include<GL\freeglut.h>
#include <glm\glm.hpp>

//OWN INCLUDES
#include "Mesh.h"
//#include "Window.h"
#include "Vec.h"
#include "MainSetups.h"
/*static void RenderSceneCB() //TODO: Put to diffrent file (make renderer class even?)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

static void initGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
}*/



int main(int argc, char* argv[])
{
	g_PreviousTime = std::clock();
	std::string windowname = "test";

	InitGL(argc, argv, windowname); //create window with name
	glutMainLoop(); //enter openGL's main loop

	return 0;
}

void Cleanup(int errorCode, bool bExit )
{
	if(g_iGLUTWindowHandle != 0)
	{
		glutDestroyWindow(g_iGLUTWindowHandle);
		g_iGLUTWindowHandle = 0;
	}

	if( bExit )
	{
		exit(errorCode);
	}
}

//init GLUT library and initiate communication with wundowing system
void InitGL(int argc, char* argv[], std::string name)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(g_iWindowWidth, g_iWindowHeight);
	glutInitWindowPosition(100, 100);
	g_iGLUTWindowHandle = glutCreateWindow(name.c_str()); //set GLUTWindowHandle as created window

	//Init GLEW
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		Cleanup(0);
	}


	//Register GLUT callbacks
	glutDisplayFunc(DisplayGL);
	glutIdleFunc(IdleGL);
	glutMouseFunc(SetMouseGL);
	glutMotionFunc(SetMotionGL);
	glutKeyboardFunc(SetKeyboardGL);
	glutReshapeFunc(ReshapeWindowGL);

	//Setup Initial GL state
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //backcround color / clear color
	glClearDepth(1.0f);

	glShadeModel(GL_SMOOTH);
}

void DisplayGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear currenr render target /buffers
	//change rendering scene based on globals value
	switch(g_eCurrentScene)
	{
	case ST_Scene1:
		{
			RenderScene1();
		}
		break;

	case ST_Scene2:
		{
			RenderScene2();
		}
		break;
	case ST_Scene3:
		{
			RenderScene3();
		}
		break;
	case ST_Scene4:
		{
			RenderScene4();
		}
		break;
	}

	glutSwapBuffers(); //flip buffers (current buffer to off-screen buffer
	glutPostRedisplay(); //tell GLUT that we are ready to render another frame. Marks current window to be redisplayed

}

void IdleGL()
{
	//update simulation
	//Calculate amount of time passed since last time method called, ensure smooth workflow indepented from framerate
	g_CurrentTime = std::clock();
	float deltaTime = (float)(g_CurrentTime - g_PreviousTime);
	g_PreviousTime = g_CurrentTime;

	float fDeltaTime = deltaTime / (float)CLOCKS_PER_SEC;

	//Rate of rotation in (degrees) in second
	const float fRotationRate = 50.0f;

	//Update rotation parameters
	//demonstrate rotation that is equal to each axis
	g_fRotate1 += (fRotationRate * fDeltaTime);
	g_fRotate1 = fmodf( g_fRotate1, 360.0f);

	g_fRotate2 += (fRotationRate * fDeltaTime);
	g_fRotate2 = fmodf( g_fRotate2, 360.0f);

	g_fRotate3 += (fRotationRate * fDeltaTime);
	g_fRotate3 = fmodf( g_fRotate3, 360.0f);

	//mark current window to be redisplayed (tell that we want to invoke DispalyGL method again)
	glutPostRedisplay();
}

void SetKeyboardGL(unsigned char c, int x, int y)
{
	//Store current scene so it can be tested later (has it changed)
	ESceneType currentScene = g_eCurrentScene;

	switch( c )
	{
	case '1':
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			g_eCurrentScene = ST_Scene1;
		}
		break;
	case'2':
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			g_eCurrentScene = ST_Scene2;
		}
		break;
	case'3':
		{
			glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
			g_eCurrentScene = ST_Scene3;
		}
		break;
	case'4':
		{
			glClearColor(0.73f, 0.73f, 0.73f, 1.0f);
			g_eCurrentScene = ST_Scene4;
		}
		break;
	case 's':
	case 'S':
		{
			std::cout << "Shader Model: GL_SMOOTH" << std::endl;
			glShadeModel(GL_SMOOTH);
		}
		break;
	case'f':
	case 'F':
		{
			std::cout << "Shader Model: GL_FLAT" << std::endl;
			glShadeModel(GL_FLAT);
		}
		break;
	case 'r':
	case 'R':
		{
			std::cout << "Reset rotation parameters..." << std::endl;
			g_fRotate1 = g_fRotate2 = g_fRotate3 = 0.0f;
		}
		break;
	case '/033': //escape
	case'/015': //enter
	case 'Q':
	case 'q':
		{
			//all four above cleanup and quit
			Cleanup(0);
		}
		break;
	}
	if(currentScene != g_eCurrentScene)
	{
		std::cout << "Changed render scene: " << (g_eCurrentScene + 1) << std::endl;
	}
	glutPostRedisplay();
}

void SetMouseGL(int button, int state, int x, int y)
{

}

void SetMotionGL(int x ,int y)
{

}

void ReshapeWindowGL(int width, int height)
{
	std::cout << "Reshape window: W: " << width << " H: " << height << std::endl;

	if(height == 0)
		height=1;

	g_iWindowWidth = width;
	g_iWindowHeight = height;

	//Setup viewport rectangle for current OpenGL context. Firts two specify bottom-left corner of the viewport
	//Next two define width and height of viewportn in pixels
	glViewport(0, 0, g_iWindowWidth, g_iWindowHeight);

	//setup the projection matrix
	//Set matrix mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//initialize the projection matrix to perpetive projection matrix.
	//Parameters in order:
	//fovy: field of view in the y-direction (degrees)
	//aspect: The aspect ratio of viewport. Usual always set to div of viewport dimensions
	//zNear: distance from viewer to "Near" clipping plane. Objects in front of this plane are clipped by rasterizer 
	//zFar: distance from viewer to "Far" clipping plane. Object farther away than this are clipped from view by rasterizer 
	gluPerspective(60.0, (GLdouble)g_iWindowWidth/(GLdouble)g_iWindowHeight, 0.1, 100.0);

	//tell glut current window can be redisplayed
	glutPostRedisplay(); 
}

//Helper methods that draw basic primitives using OpenGL immidiate mode
//These are called from scene rendering method, to cretae complex scenes
//immidiate mode used to render primitives, begins with glBegin, and ends to glEnd call
//Parameter passed to glBegin determine the type of the primitive to be drawn (GL_POINTS, GL_LINES, GL_LINE_STRIP and so on)

//DRAW SIGLE 2D TRIAGLE
void DrawTriangle(vec2f p1, vec2f p2, vec2f p3)
{
	glBegin(GL_TRIANGLES); //define triagle primitive
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
	glEnd();
}

//DRAW SINGLE 2D RECTANGLE
void DrawRectangle(float width, float height)
{
	const float w = width / 2.0f;
	const float h = height / 2.0f;

	glBegin(GL_QUADS); //define rectangle primitive
		glVertex2f(-w, h); //T-L
		glVertex2f(w, h); //T-R
		glVertex2f(w, -h); //B-R
		glVertex2f(-w, -h); //B-L
	glEnd();
}

//DRAW SIGLE 2D CIRCLE
void DrawCircle(float radius, int numSides /* =8 */)
{
	const float step = (float)M_PI / numSides; //STEP IS PI DIVIED TO WANTED NUMBER OF SIDES
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.0f, 0.0f);
		for(float angle = 0.0f; angle < (2.0f * M_PI ); angle += step) //WHILE ANGLE SMALLER THAN 2*PI
		{
			glVertex2f(radius * sinf(angle), radius * cosf(angle)); //create new vertex
		}
		glVertex2f(0.0f, radius); //CLOSING VERTEX
	glEnd();
}

void RenderScene1()
{
	glMatrixMode(GL_MODELVIEW); //SET MODELVIEW MATRIX
	glLoadIdentity(); //RESET MODELVIEW MATRIX, RESETING ANY TRANSLATIONS, SCALES AND ROTATIONS BACK TO DEFAULT STATE

	glTranslatef(-1.5, 1.0f, -6.0f); //TRANSLATE MODELVIEW MATRIX 6 UNITS AWAY FROM THE VIEVER (z-axis), ONE UNIT UP(y-axis), and 1.5 UNITS LEFT(x-axis). This sets the position for primitive
	glColor3f(1.0f, 0.0f, 0.0f); //SPECIFY COLOR FOR VERTICES SEND TO OPENGL DISPLAY LIST
	DrawTriangle(vec2f(0.0f, 1.0f), vec2f(-1.0f, -1.0f), vec2f(1.0f, -1.0f));

	glTranslatef(3.0, 0.0f, 0.0f); //NOTE THAT THIS HAPPENS RELATIVE TO LAST PRIMITIVE DRAWN, SO ACTUAL POSITION IS (1.5, 1.0 -6.0)
	glColor3f(1.0f, 1.0f, 0.0f);
	DrawRectangle(2.0f, 2.0f);

	glTranslatef(-1.5, -3.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	DrawCircle(1.0f, 8);

}

void RenderScene2()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-1.5f, 1.0f, -6.0f);
	//draw triangles with different color in each angle.
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.0f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);
	glEnd();

	glTranslatef(3.0f, 0.0f, 0.0f);
	//rectangle
	glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-1.0f, 1.0f);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(-1.0f, -1.0f);
	glEnd();

	glTranslatef(-1.5f, -3.0f, 0.0f);
	const float radius = 1.0f;
	const float step = (float)M_PI / 16; //STEP IS PI DIVIED TO WANTED NUMBER OF SIDES
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(0.0f, 0.0f);
		for(float angle = 0.0f; angle < (2.0f * M_PI ); angle += step) //WHILE ANGLE SMALLER THAN 2*PI
		{
			float fSin = sinf(angle);
            float fCos = cosf(angle);

			glColor3f((fCos + 1.0f)* 0.5f, (fSin + 1.0f) * 0.5f, 0.0f);
			glVertex2f(radius * fSin, radius * fCos); //create new vertex
		}
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex2f(0.0f, radius); //CLOSING VERTEX
	glEnd();
}

void RenderScene3()
{
		glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-1.5f, 1.0f, -6.0f);
	glPushMatrix(); //push curent trasformation to matrix stack
	glRotatef(g_fRotate1, 0.0f, 0.0f, 1.0f); //rotate primitive z-axis by angle g_fRotate1 (angle, x, y, z)
	//draw triangles with different color in each angle.
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(0.0f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-1.0f, -1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(1.0f, -1.0f);
	glEnd();
	glPopMatrix(); //push out fro matrix, or matrix will bahave weirdly

	glTranslatef(3.0f, 0.0f, 0.0f);
	glPushMatrix(); //push curent trasformation to matrix stack
	glRotatef(g_fRotate2, 0.0f, 0.0f, 1.0f); //rotate primitive x-axis by rotation g_fRotate2
	//rectangle
	glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-1.0f, 1.0f);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(-1.0f, -1.0f);
	glEnd();
	glPopMatrix();

	glTranslatef(-1.5f, -3.0f, 0.0f);
	glPushMatrix(); //push curent trasformation to matrix stack
	glRotatef(g_fRotate3, 0.0f, 0.0f, 1.0f); //rotate primitive x-axis by rotation g_fRotate3
	const float radius = 1.0f;
	const float step = (float)M_PI / 16; //STEP IS PI DIVIED TO WANTED NUMBER OF SIDES
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(0.0f, 0.0f);
		for(float angle = 0.0f; angle < (2.0f * M_PI ); angle += step) //WHILE ANGLE SMALLER THAN 2*PI
		{
			float fSin = sinf(angle);
            float fCos = cosf(angle);

			glColor3f((fCos + 1.0f)* 0.5f, (fSin + 1.0f) * 0.5f, 0.0f);
			glVertex2f(radius * fSin, radius * fCos); //create new vertex
		}
		glColor3f(1.0f, 0.5f, 0.0f);
		glVertex2f(0.0f, radius); //CLOSING VERTEX
	glEnd();
	glPopMatrix();
}

void RenderScene4()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST); //THIS MAKES SURE PRIMITIVES CLOSER TO VIEW ARE DRAWN OVER PRIMITIVES THAT ARE FURTHER

	glTranslatef(-1.5f, 1.0f, -6.0f);
	glPushMatrix(); //push matrix to stack
	glRotatef(g_fRotate2, 1.0f, 1.0f, 1.0f); //rotaion to all all axises (x,y,z)
	glBegin(GL_QUADS);
		//TOP FACE
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);


		//BOTTOM FACE
		glColor3f(0.0f, 0.5f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		//FRONT FACE
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		//BACK FACE
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);

		//LEFT FACE
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);

		//RIGHT FACE
		glColor3f(0.0f, 0.5f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();
	glPopMatrix();
}

