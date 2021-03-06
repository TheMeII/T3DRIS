#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <assert.h>
#include <thread>
#include <ctime>

//OPENGL HELPER LIB INCLUDES
#include <GL/glew.h> //handels management of OpenGL's extensions in system
#include <GL\freeglut.h> //Windowing for multiple platforms

//OWN INCLUDES
#include "math3d.h"
#include "pipeline.h"
#include "Camera.h"
#include "Object.h"
#include "InitResources.h"
#include "Shapes.h"
#include "Game.h"
#include "RenderObject.h"
#include "KinectControl.h"

using namespace std;

#pragma comment(lib, "glew32.lib") //ensure success of glew

//GLOBALS
GLuint gWVPLocation; //Global uniform variable
GLuint gSampler;
Camera* leftCamera;
Camera* rightCamera;
int leftActive = 1; //lets start with left camera active
Game* game;
RenderObject *renderObject;

//Kinect globals
KinectControl* kinect;
int movement=0;

int nextRand = 1;

bool created = false;
bool stereo = true;

//Shader codes
//VERTEX SHADER
static const char* VeSh = "				  \n\
#version 330							  \n\
layout (location = 0) in vec3 Position;	  \n\
layout (location = 1) in vec2 TexCoord;	  \n\
                                          \n\
uniform mat4 gWVP;					      \n\
out vec2 TexCoord0;                     \n\
                                          \n\
void main()                               \n\
{                                         \n\
	gl_Position = gWVP * vec4(Position, 1.0);  \n\
	TexCoord0 = TexCoord;				  \n\
}";

//FRAGMENT SHADER
static const char* FrSh = "				     \n\
#version 330							     \n\
in vec2 TexCoord0;						 \n\
out vec4 FragColor;						     \n\
uniform sampler2D gSampler;					 \n\
									         \n\
void main()							         \n\
{                                            \n\
	FragColor = texture2D(gSampler, TexCoord0.xy);\n\
}";


/*void LoadTexture(string fileName)
{
	pTexture = new Texture(GL_TEXTURE_2D, "cube_white.png");
	if(!pTexture->Load())
	{
		exit(0);
	}

}*/

void kinectMovement()
{
	if (movement >0){
		//cout << "               MOOOOOOOOOVE " << movement << endl; //debug
		
		(movement&4?	game->setPosZ(-1.0f*0.05f)	:NULL);		//Movement up		(negative Z)
		(movement&8?	game->setPosZ( 1.0f*0.05f)	:NULL);		//Movement down		(positive Z)
		(movement&16?	game->setPosX(-1.0f*0.05f)	:NULL);		//Movement right	(negative X)
		(movement&32?	game->setPosX( 1.0f*0.05f)	:NULL);		//Movement left		(positive X)

		(movement&64 ?	game->setRotX(-90.0f)	:NULL);			//Rotation up		(negative X-axle)
		(movement&128?	game->setRotX( 90.0f)	:NULL);			//Rotation down		(positive X-axle)
		(movement&256?	game->setRotY( 90.0f)	:NULL);			//Rotation right	(positive Y-axle)
		(movement&512?	game->setRotY(-90.0f)	:NULL);			//Rotation left		(negative Y-axle)
		
	}
}

void createNewDynamic()
{
	nextRand++;
	nextRand%=7;

	Shapes currentShape = Shapes();
	currentShape.selectShape(nextRand,&currentShape);
	renderObject->pushObject(currentShape.returnShape());
	game->incNumberOfObjects();
}

	



static void RenderSceneCB()
{
	static clock_t last; //last time rendered
	//static clock_t current; //current time
	static int fps = 0;
	static int lr=0, rr=0;
	static clock_t frames_start;
	static clock_t frames_end;

	if(!created)
	{
		last = clock();
		created = true;
	}

	//current = clock();

	if((float)clock()-(float)last>= (1000.0f/60.0f))
	{
		Camera* activeCamera;
	
		
		static float scale = 0.00f; //set static, so its value is recerved
		static float scale2 = 0.00f; //set static, so its value is recerved
		if(game->getCreateNew())
		{
			createNewDynamic();
			game->setCreateNew(false);
		}

		
		scale += 0.01f;
		scale2 += 0.1f;
		
		Pipeline p;
	
		//Call Kinect Movement
		kinectMovement();

		//game->getObjects(renderObjects);
		//cout << game->gameObjects.size() << endl;

		//renderObject->doTransforms(); //do transforms for all objects
		int size = game->getNumberOfObjects();

		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//cout << size << endl;
		for (int j=0;j<2;j++)
		{
			if(stereo){
				if (j==0)
				{
					activeCamera = leftCamera;
					//cout << "left" << endl;
					lr++;
					activeCamera->OnRender();
					if(stereo)
						glDrawBuffer(GL_BACK_LEFT);
						glViewport(0, 0, WINDOW_WIDTH/2, WINDOW_HEIGHT);
				
				}

				else
				{
					activeCamera = rightCamera;
					//cout << "right" << endl;
					rr++;
					activeCamera->OnRender();
					glDrawBuffer(GL_BACK_RIGHT);
					glViewport(WINDOW_WIDTH/2, 0, WINDOW_WIDTH/2, WINDOW_HEIGHT);
				}
			}

			else{
				activeCamera = leftCamera;
				activeCamera->OnRender();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			}

			for(unsigned int i = 0; i < size; i++)
			{
				Object render = renderObject->getObject(i);
				vector<Vector3f> objTrans;
				render.getTransforms(objTrans);
				p.WorldPos(objTrans[0].x, objTrans[0].y, objTrans[0].z);
				p.Rotate(objTrans[1].x, objTrans[1].y, objTrans[1].z);
				p.Scale(objTrans[2].x, objTrans[2].y, objTrans[2].z);
				p.SetCamera(activeCamera->GetPos(), activeCamera->GetTarget(), activeCamera->GetUp());
				p.SetPerpectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 250.0f);

				glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans()); //Send uniform 4x4 matrix to shader. Can be also used to multiply matrices in one call (translation, rotation, scale, projection)
								//Params: 1:Location of uniform variable (retrieved after shader compilation using glGetUniformLocation()), 2:number of matrices that are updated, 3:row-major or collum-major order (True is row major)
								//Params: 4:Starting adress of the matrix in memory (Rest are located after this address).
				render.Render();
			}
			if(!stereo) //break if no stero renderng active (second time not needed)
			{
				break;
			}
		}
		glutSwapBuffers();
		last = clock();

		if(fps == 0)
		{
			frames_start = clock();
			fps++;
			return;
		}
		fps++;
		frames_end = clock();
		if(1000.0 * (frames_end - frames_start) / CLOCKS_PER_SEC >= 1000.0)
		{
			cout << "frames per sec: " << fps << " Left: " << lr <<" Right: " << rr << endl;
			fps = 0;
			lr =0;
			rr=0;
			
		}


	}
}

/*static void Idle()
{
	//pGameCamera->OnRender();
	static float scale = 0.0f; //set static, so its value is recerved

	scale += 0.0001f;


	//glUniform1f(gScaleLocation, sinf(scale)); //Pass uniform value to shader using glUniform* functions.
	Pipeline p;
	for(int i = 0; i < boxes.size(); i++)
	{
	p.Rotate(0.0f, sinf(scale)*90.0f, sinf(scale)*90.0f);
	p.WorldPos(0.0f, 0.0f, sinf(scale));
	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
	p.SetPerpectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans()); //Send uniform 4x4 matrix to shader. Can be also used to multiply matrices in one call (translation, rotation, scale, projection)
					//Params: 1:Location of uniform variable (retrieved after shader compilation using glGetUniformLocation()), 2:number of matrices that are updated, 3:row-major or collum-major order (True is row major)
					//Params: 4:Starting adress of the matrix in memory (Rest are located after this address).
	}
	
	glutPostRedisplay();//call redispaly func to draw screen again
}*/

static void SpecialKeyboardCB(int Key, int x, int y)
{
	leftCamera->OnKeyboard(Key);
	rightCamera->OnKeyboard(Key);
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch(Key)
	{
	case'q':
		exit(0);
		break;
	case's':
		game->rotX += 90.0f;
		break;

	case'a':
		game->rotY -= 90.0f;
		break;
	case'w':
		game->rotX -= 90.0f;
		break;

	case'd':
		game->rotY += 90.0f;
		break;


	case'i':
		//posX = 0.0f;
		//posY = 0.0f;
		game->posZ -= 1.0f;
		break;

	case'k':
		//posX = 3.0f;
		//posY = 3.0f;
		game->posZ += 1.0f;
		break;
	case'j':
		game->posX += 1.0f;
		//posY = 0.0f;
		//posZ -= 1.0f;
		break;

	case'l':
		game->posX -= 1.0f;
		//posY = 3.0f;
		//posZ += 1.0f;
		break;

	case'c':
		stereo = false;
		break;

	}


}

static void PassiveMouseCB(int x, int y)
{
	leftCamera->OnMouse(x, y);
	rightCamera->OnMouse(x, y);
}

static void TimerFunc(int value)
{

}



static void initializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB); //set rendering function
	glutIdleFunc(RenderSceneCB); //Set idle funtion that is called�, when no events are recived from windowing system
	glutPassiveMotionFunc(PassiveMouseCB);
	glutSpecialFunc(SpecialKeyboardCB);
	glutKeyboardFunc(KeyboardCB);
	

	
}


//
//
//static void createShape(){
//	Shapes shape1;
//	shape1.selectShape(4,&shape1);
//	boxes.push_back(shape1.returnShape());
//}
//
//



//Ass shader program to rendering pipeline
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObject = glCreateShader(ShaderType); //create shader object. 

	if(ShaderObject == 0)
	{
		cerr << "Error creating shader type " << ShaderType;
		exit(0);
	}

	//Create source array and source length array, and initialize those
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObject, 1, p, Lengths); //takes shader object as parameter, and then the shader program is provide for function in character array.
			//Params: Shader object, number of slots in two arrays, char array containing shader source(s), array containing each sorce arrays lenght
	glCompileShader(ShaderObject); //compile shaders
	GLint success;

	//Check sorces for possible errors in it, and return error if there is one
	glGetShaderiv(ShaderObject, GL_COMPILE_STATUS, &success);
	if(!success) 
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObject, 1024, NULL, InfoLog);
		cerr << "Error compiling shdaer type " << ShaderType;
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObject); //Attach Shader object to program object


}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram(); // create shader program. All shader programs are linked to this

	if(ShaderProgram == 0) //make sure program is created
	{
		cerr << "Error cretating program" << endl;
		exit(0);
	}

	AddShader(ShaderProgram, VeSh, GL_VERTEX_SHADER); //link vertex shader to program
	AddShader(ShaderProgram, FrSh, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = {0};

	glLinkProgram(ShaderProgram); //Link shader program that is used

	//Check that shader program is linked correclty.
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if(Success == 0)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		cerr << "Error linking shader program: " << ErrorLog;
		exit(0);
	}
	glValidateProgram(ShaderProgram); //validate shader program, so that shader is used in correct draw call in pipeline (There may be multiple shaders in big programs)
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if(!Success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		cerr << "Invalid shader program: " << ErrorLog;
		exit(1);
	}

	glUseProgram(ShaderProgram);

	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP"); //query program object for location of the uniform variable
	assert(gWVPLocation != 0xFFFFFFFF); //catch error if statement false
}





int main(int argc, char* argv[])
{
	
	GLboolean stereo;
	//Create and start Kinect controller
	kinect = new KinectControl(&movement);
	kinect->StartThread();

	glGetBooleanv(GL_STEREO, &stereo);
	if (stereo)
		cout << "stereo toimii " << endl;
	Magick::InitializeMagick(*argv);
	//init GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA | GLUT_DEPTH);
	//glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("T3DRIS");
	if (true)
		glutGameModeString("1920x1080@60");
	else
		glutGameModeString("1280x1024@60");
	glutEnterGameMode();

	//GLEW check
	GLenum res = glewInit();
	if(res != GLEW_OK) //if init result not succesful
	{
		cerr << "Error: " << glewGetErrorString(res); //get error string set to error enum
		return 0;
	}


	//GLUT Callback init
	initializeGlutCallbacks();

	//SETUP CAMERAS
	leftCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(0.125f ,45.0f ,0.0f), Vector3f(0.0f ,0.0f ,1.0f), Vector3f(0.0f , 1.0f , 0.0f));
	rightCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(-0.125f ,45.0f ,0.0f), Vector3f(0.0f ,0.0f ,1.0f), Vector3f(0.0f , 1.0f , 0.0f));
	leftCamera->setAngleV(90.0f);
	rightCamera->setAngleV(90.0f);
	
	//Set Background/clear color of screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Black

	//WITH THESE BACK FACES ARE NOT RENDERS (INSIDE OF OBJECT)
	/*glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);*/

	//enables features like transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderObject = new RenderObject();
	game = new Game(renderObject);
	game->startThread();

	//CreateObject1(); //Create indices

	//createShape();

	//CreateObject3();

	CompileShaders(); //create shaders

	glUniform1i(gSampler, 0); //set texture unit uniform that is going to be used in shader
	glutMainLoop(); //enter OpenGL's main loop, that is runned 3d program
	
	return 0; //exit program

}




//ABOUT SHADERS

/*
We have completed the walkthru of the OpenGL calls related to shader management. The rest of this tutorial relates to the contents of the vertex and fragment shaders (contained in the 'pVS' and 'pFS' variables).

#version 330
This tells the compiler that we are targeting version 3.3 of GLSL. If the compiler does not support it it will emit an error.

layout (location = 0) in vec3 Position;
This statement appears in the vertex shader. It declares that a vertex specific attribute which is a vector of 3 floats will be known as 'Position' in the shader. 'Vertex specific' means that for every invocation 
of the shader in the GPU the value of a new vertex from the buffer will be supplied. The first section of the statement, layout (location = 0), creates the binding between the attribute name and attribute in the 
buffer. This is required for cases where our vertex contains several attributes (position, normal, texture coordinates, etc). We have to let the compiler know which attribute in the vertex in the buffer must be 
mapped to the declared attribute in the shader. There are two ways to do this. We can either set it explicitly as we do here (to zero). In that case we can use a hard coded value in our application (which 
we did with the first parameter to the call to glVertexAttributePointer). Or we can leave it out (and simply declare 'in vec3 Position' in the shader) and then query the location from the application at 
runtime using glGetAttribLocation. In that case we will need to supply the returned value to glVertexAttributePointer instead of using the hard coded value. We choose the simply way here but for more complex 
applications it better to let the compiler determine the attribute indices and query them during runtime. This makes it easier integrating shaders from multiple sources without adapting them to your buffer layout.

void main()
You can create your shader by linking together multiple shader objects. However, there can only be one main function for each shader stage (VS, GS, FS) which is used as the entry point to the shader. 
For example, you can create a lighting library with several functions and link it with your shader provided that none of the functions there is named 'main'.

gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, Position.z, 1.0);
Here we do hard coded transformation to the incoming vertex position. We cut the X and Y values by half and leave the Z unchanged. 'gl_Position' is a special built in variable that is supposed to contain the 
homogeneous (containing X, Y, Z and W components) vertex position. The rasterizer will look for that variable and will use it as the position in screen space (following a few more transformations). 
Cutting the X and Y values by half means that we will see a triangle which is one quarter the size of the triangle in the previous tutorial. Note that we are setting W to 1.0. This is extremely important to 
get the triangle displayed correctly. Getting the projection from 3D to 2D is actually accomplished in two seperate stages. First you need to multiple all your vertices by the projection matrix 
(which we will develop in a few tutorials) and then the GPU automatically performs what is known as "perspective divide" to the position attribute before it reaches the rasterizer. This means that it 
divides all the components of the gl_Position by the W component. In this tutorial we are not yet doing any projection in the vertex shader but the perspective divide stage is something that we cannot disable.
Whatever gl_Position value we output from the vertex shader will be divided by the HW using its W component. We need to remember that else we will not get the results we expect. 
In order to circumvent the effect of perspective divide we set W to 1.0. Division by 1.0 will not effect the other components of the position vector which will stay inside our normalized box.

If everything worked correctly, three vertices with the values (-0.5, -0.5), (0.5, -0.5) and (0.0, 0.5) reach the rasterizer. The clipper doesn't need to do anything because all vertices are well inside 
the normalized box. These values are mapped to screen space coordinates and the rasterizer starts running over all the points that are inside the triangle. For each point the fragment shader is executed. 
The following shader code is taken from the fragment shader.

out vec4 FragColor;
Usually the job of the fragment shader is to determine the color of the fragment (pixel). In addition, the fragment shader can discard the pixel altogether or change its Z value (which will effect the 
result of subsequent Z test). Outputing the color is done by declaring the above variable. The four components represent R, G, B and A (for alpha). The value that you set into this variable will be 
received by the rasterizer and evantually written to the framebuffer.

FragColor = vec4(1.0, 0.0, 0.0, 1.0);
In the previous couple of tutorials there wasn't a fragment shader so the everything was drawn in the default color of white. Here we set FragColor to red.


*/