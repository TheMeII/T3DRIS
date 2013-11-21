#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <assert.h>
#include <thread>

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
using namespace std;

#define PI 3.14159265358979323846264338327950288
#pragma comment(lib, "glew32.lib") //ensure success of glew

//GLOBALS
GLuint gWVPLocation; //Global uniform variable
GLuint gSampler;
Camera* pGameCamera;
Game* game;
RenderObject *renderObject;
float rotX = 0.0f, rotY = 0.0f,rotZ=0.0f;
float posX = 1.0f, posY = 1.0f, posZ = 1.0f;

bool created = false;

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





static void RenderSceneCB()
{
	pGameCamera->OnRender();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	static float scale = 0.0f; //set static, so its value is recerved
	renderObject->renderObjects[2].Rotate(Vector3f(rotX,rotY,0.0f));
	renderObject->renderObjects[2].Translate(Vector3f(posX,posY,posZ));
//	scale += 45.0f;

	//WITH THESE COMMANDS OBJECTS ARE MODIFIED SEPARETLY

	
	/*
	if(game->isSetup() == 0)
	{
		//CreateSkybox();
		CreateFloor();
		game->setSetup(true);
	}

	//GIVE TRASFORMATIONS USED FOR STATIC OBJECTS


	*/

	//glUniform1f(gScaleLocation, sinf(scale)); //Pass uniform value to shader using glUniform* functions.
	Pipeline p;
	
	//game->getObjects(renderObjects);
	//cout << game->gameObjects.size() << endl;

	int size = renderObject->getSize();

	cout << size << endl;

	for(unsigned int i = 0; i < size; i++)
	{
		Object render = renderObject->getObject(i);
		vector<Vector3f> objTrans;
		render.getTransforms(objTrans);
		p.WorldPos(objTrans[0].x, objTrans[0].y, objTrans[0].z);		
		p.Rotate(objTrans[1].x, objTrans[1].y, objTrans[1].z);
		p.Scale(objTrans[2].x, objTrans[2].y, objTrans[2].z);
		p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
		p.SetPerpectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 250.0f);

		glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans()); //Send uniform 4x4 matrix to shader. Can be also used to multiply matrices in one call (translation, rotation, scale, projection)
						//Params: 1:Location of uniform variable (retrieved after shader compilation using glGetUniformLocation()), 2:number of matrices that are updated, 3:row-major or collum-major order (True is row major)
						//Params: 4:Starting adress of the matrix in memory (Rest are located after this address).
		render.Render();
	}

	glutSwapBuffers();
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
	pGameCamera->OnKeyboard(Key);
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch(Key)
	{
	case'q':
		exit(0);
		break;
	case's':
		rotX += 90.0f;
		break;

	case'a':
		rotY -= 90.0f;
		break;
	case'w':
		rotX -= 90.0f;
		break;

	case'd':
		rotY += 90.0f;
		break;


	case'i':
		//posX = 0.0f;
		//posY = 0.0f;
		posZ -= 1.0f;
		break;

	case'k':
		//posX = 3.0f;
		//posY = 3.0f;
		posZ += 1.0f;
		break;
	case'j':
		posX += 1.0f;
		//posY = 0.0f;
		//posZ -= 1.0f;
		break;

	case'l':
		posX -= 1.0f;
		//posY = 3.0f;
		//posZ += 1.0f;
		break;

	case'c':
		
		break;

	}


}

static void PassiveMouseCB(int x, int y)
{
	pGameCamera->OnMouse(x, y);
}

static void TimerFunc(int value)
{

}



static void initializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB); //set rendering function
	glutIdleFunc(RenderSceneCB); //Set idle funtion that is called´, when no events are recived from windowing system
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
	
	

	Magick::InitializeMagick(*argv);
	//init GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("T3DRIS");
	glutGameModeString("1920x1080:32@75");
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

	pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Vector3f(0.0f ,5.0f ,0.0f), Vector3f(0.0f ,0.0f ,1.0f), Vector3f(0.0f , 1.0f , 0.0f));

	
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



/**
  * returns a unit vector
  */
Vector3f& getUnitVector3D(struct Vector3f &vector) {
    Vector3f newv;
    double d = sqrt(vector.x * vector.x + vector.y * vector.y +  vector.z * vector.z);
    newv.x = vector.x / d;
    newv.y = vector.y / d;
    newv.z = vector.z / d;
    return newv;
}


/**
  * using quaternions to rotate around an arbirtuary axis
  * Given angle theta in radians and unit vector u = ai + bj + ck or (a,b,c)
  *
  * q0 = cos(r/2),  q1 = sin(r/2) a,  q2 = sin(r/2) b,  q3 = sin(r/2) c
  *
  * Q =
  * [
  *   (q0^2 + q1^2 - q2^2 - q3^2)        2(q1q2 - q0q3)     2(q1q3 + q0q2)
  *   2(q2q1 + q0q3)           (q0^2 - q1^2 + q2^2 - q3^2)  2(q2q3 - q0q1)
  *   2(q3q1 - q0q2)             2(q3q2 + q0q1)     (q0^2 - q1^2 - q2^2 + q3^2)
  * ]
  *
  * Q u = u
  *
  * @param Vector3D 1
  * @param Vector3D 2
  * @param theta
  * @return Vector
  */
Vector3f& rotateAroundVector(Vector3f &vect1, Vector3f &vect2, double theta) {
    Vector3f newv;
    Vector3f unit = getUnitVector3D(vect2);
    //theta = Math.toRadians(theta);
    double q0 = cos(theta/2);
    double q1 = sin(theta/2)*unit.x;
    double q2 = sin(theta/2)*unit.y;
    double q3 = sin(theta/2)*unit.z;

    // column vect
    newv.x = (q0*q0 + q1*q1 - q2*q2 - q3*q3)* +    2*(q2*q1 + q0*q3) * vect1.y +                       2*(q3*q1 - q0*q2) * vect1.z;
    newv.y = 2*(q1*q2 - q0*q3)*vect1.x +             (q0*q0 - q1*q1 + q2*q2 - q3*q3) * vect1.y +       2*(q3*q2 + q0*q1) * vect1.z;
    newv.z = 2*(q1*q3 + q0*q2)*vect1.x +             2*(q2*q3 - q0*q1) * vect1.y +                     (q0*q0 - q1*q1 - q2*q2 + q3*q3) * vect1.z;
    return newv;
}


double degToRad(double deg) {
    return deg * PI / 180;
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