/*=================================================
 * Final Project
 * 3D Tetris
 * CSE170 - Introduction to Computer Graphics
 * UC Merced, M. Kallmann 
 * Written by: Derek Burch
 *=================================================*/

#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "draw.h"
#include "math.h"
#include <iostream>
#include <string>
#include <fstream>
//==========================================================================
// We use a global pointer to maintain the a single instance of the 
// state of our application, see comments in glutapp.h
//==========================================================================
AppState* App;

//default cylinder settings
int currentFaceCount = 12;
double currentLen = 8;
double currentTR = 3;
double currentBR = 3;
bool smoothShading = true;

double timeTillFall = 0;
double lastTime = 0;

//used to determine which state the game is in
//0=menu 1=playing the game
int gameState = 0;

//substate is used to determine what sub menu to display and
//to know when to display gameover to the player
int gameSubState = 0;

//GAME VARIABLES
Grid* grid;
FallingObject* fallingPiece;


void appKeyboardFunc ( unsigned char key, int x, int y )
 {
	 if(gameState == 0){
	 
					grid = new Grid(8,16,8);
					grid->difficulty = 0;
					fallingPiece = new FallingObject();
					fallingPiece->setPosition(grid,2,10,2);
					gameState = 1;
			
	 }else{       

	
	   const double inc=0.25;
	   int xChange = 0;
	   int zChange = 0;
	
		   switch ( key )
			{ 
		   case ' ': timeTillFall=0; break;
		   case '1': grid->removeLayer(0);break;
			  case 'w': 
				  xChange=0;
				  zChange=1;
				  break;
			  case 'a': 
				  xChange=1;
				  zChange=0;
				  break;
			  case 's': 
				  xChange=0;
				  zChange=-1;
				  break;
			  case 'd': 
				  xChange=-1;
				  zChange=0;
				  break;
			  case 'e': fallingPiece->rotateY(grid); break;
			  case 'q': fallingPiece->rotateX(grid); break;

//			  case 'z': glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);break;
			  case 'x': glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);glShadeModel(GL_FLAT);smoothShading=false;break;
			  case 'c': glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);glShadeModel(GL_SMOOTH);smoothShading=true;break;
			  case 27 : exit(1); // Esc was pressed
			   
			  default: return; // return without rendering
			}
			if(zChange != 0 || xChange != 0){
			   double angle = (App->roty)*3.14/180;
			   double xmove = (xChange*cos(angle) - zChange*sin(angle));
			   double zmove = (xChange*sin(angle) + zChange*cos(angle));
			   //printf("xm %f zm %f \n",xmove,zmove);
			   if(xmove*xmove <zmove*zmove){
				   xmove=0;
				   if(zmove<0)zmove=-1;
				   else zmove=1;
			   }
			   else{
				   zmove=0;
				   if(xmove<0)xmove=-1;
				   else xmove=1;
			   }
			   
			   fallingPiece->move(grid,(int)xmove,0,(int)zmove);
		   }  
	   }
//	 }
	 glutPostRedisplay();
}





//==========================================================================
// This appKeyboardFunc is for handling special keys. Note that this is 
// C++ and the parameter list differs from the other appKeyboardFunc
//==========================================================================
void appKeyboardFunc ( int key, int x, int y )
 {
	
	 {  
	   const double incr=5;
	   const double incf=0.2;
	   switch ( key )
		{ case GLUT_KEY_LEFT:      App->roty-=incr; break;
		  case GLUT_KEY_RIGHT:     App->roty+=incr; break;
		  case GLUT_KEY_UP:        App->rotx+=incr; break;
		  case GLUT_KEY_DOWN:      App->rotx-=incr; break;  
		  case GLUT_KEY_PAGE_UP:   App->fovy-=incf; break;
		  case GLUT_KEY_PAGE_DOWN: App->fovy+=incf; break;
		  default: return; // return without rendering
		}
	 }
   // at this point the mark position changed and so we render the scene again:
   glutPostRedisplay();
}

int lastMouseX=0;
int lastMouseY=0;
int mouseXChange = 0;
int mouseYChange = 0;
bool shiftDown = false;
void appMouseFunc ( int button, int state, int x, int y ) // mouse clicks
 {
	  int mod = glutGetModifiers();
	 if (gameSubState==0 && mod == GLUT_ACTIVE_SHIFT){
		 shiftDown=true;
	 }else{
		shiftDown=false;
	 }                            
	 mouseXChange=0;
	 mouseYChange=0;
   glutPostRedisplay();
   lastMouseX=x;
   lastMouseY=y;
 }  

void appMotionFunc ( int x, int y ) // mouse dragging
 {
	 if(gameState == 1){
		 if(shiftDown){
			mouseXChange+=x-lastMouseX;
			mouseYChange+=y-lastMouseY;
			if(mouseXChange>50){
				double cv = cos(App->roty*3.14/180);
				if(cv>=0)fallingPiece->rotateY(grid);
				else fallingPiece->rotateYReverse(grid);
				mouseXChange=0;
			}
			if(mouseXChange<-50){
				double cv = cos(App->roty*3.14/180);
				if(cv>=0)fallingPiece->rotateYReverse(grid);
				else fallingPiece->rotateY(grid);
				mouseXChange=0;
			}
			if(mouseYChange>50){
				double sv = sin(App->roty*3.14/180);
				double cv = cos(App->roty*3.14/180);
				
				if(sv*sv>0.5){
					if(cv>=0)fallingPiece->rotateZReverse(grid);
					else fallingPiece->rotateZ(grid);
				}
				else {
					if(cv>=0)fallingPiece->rotateXReverse(grid);
					else fallingPiece->rotateX(grid);
				}
				mouseYChange=0;
				mouseYChange=0;
			}
			if(mouseYChange<-50){
				double sv = sin(App->roty*3.14/180);
				double cv = cos(App->roty*3.14/180);
			
				if(sv*sv>0.5){
					if(cv>=0)fallingPiece->rotateZ(grid);
					else fallingPiece->rotateZReverse(grid);
				}
				else {
					if(cv>=0)fallingPiece->rotateX(grid);
					else fallingPiece->rotateXReverse(grid);
				}
				mouseYChange=0;
			}
		 }else{
			App->roty += x-lastMouseX;
			App->rotx += y-lastMouseY;
		 }
	 }
	  glutPostRedisplay();
	   
	   lastMouseX=x;
		lastMouseY=y;
 }



void appResizeWindow ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );	
}



void appDrawScene ()
 {
   // Clear the rendering buffer:
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   if(gameState == 0){
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glOrtho (0, 600, 0, 600, 0, 1);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glDisable(GL_LIGHTING);
		
		glColor3d(1.0,1.0,1.0);  

		

   }else  
		{
	   // Set the viewing frustum for a coordinate range in [-10,10]^3
	   int w = glutGet ( GLUT_WINDOW_WIDTH );
	   int h = glutGet ( GLUT_WINDOW_HEIGHT );
	   double aspect = double(w)/double(h);
	   glMatrixMode ( GL_PROJECTION );
	   glLoadIdentity ();
	   gluPerspective ( App->fovy, aspect, 2/*znear*/, 300/*zfar*/ );
	   gluLookAt ( 0, 0, -50, // eye
				   0, 0, 0,    // center
				   0, 1, 0 );  // up vector

	   // Clear the model transformation stack:
	   glMatrixMode( GL_MODELVIEW );
	   glLoadIdentity();

	   // Rotate the scene 
	   glRotated ( App->rotx, 1, 0, 0 );
	   glRotated ( App->roty, 0, 1, 0 );

	   
	   glDisable(GL_LIGHTING);
	 
		glEnable(GL_LIGHTING);

	 
	    float mcolor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		float specReflection[] = { 0.8f, 0.8f, 0.8f, 1.0f };
		float diffuseReflection[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
	//	glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseReflection);
	//	glMateriali(GL_FRONT, GL_SHININESS, 30);

		draw_grid(grid,fallingPiece);

		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glOrtho (0, 600, 0, 600, 0, 1);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glDisable(GL_LIGHTING);

		if(gameSubState == 0){
			glColor3d(1.0,1.0,1.0);
	
		}else{

			glColor3d(0.1,0.1,0.1);
			glBegin(GL_QUADS);
				glVertex3d(0,200,1);
				glVertex3d(0,000,1);
				glVertex3d(600,000,1);
				glVertex3d(600,200,1);
			glEnd();
			glColor3d(1.0,1.0,1.0);
	
		}
   }
   glFlush();         
   glutSwapBuffers(); 
}


void appUpdate(){
	double elapsedTime = get_time() - lastTime;
	if(gameState==0){

	}else if (gameState == 1 && gameSubState == 0){

	
		timeTillFall -= elapsedTime;
		if(timeTillFall<=0){
			if(!fallingPiece->move(grid,0,-1,0)){	
				for(int p=0;p<5;p++){
					if(!fallingPiece->checkAboveGrid(grid)){
						grid->occupied[fallingPiece->position[0]+fallingPiece->pieceLoc[p][0]][fallingPiece->position[1]+fallingPiece->pieceLoc[p][1]][fallingPiece->position[2]+fallingPiece->pieceLoc[p][2]]
						= (fallingPiece->bombLocation == p)?2:1;
						float* color = grid->color[fallingPiece->position[0]+fallingPiece->pieceLoc[p][0]][fallingPiece->position[1]+fallingPiece->pieceLoc[p][1]][fallingPiece->position[2]+fallingPiece->pieceLoc[p][2]];
						for(int c=0;c<4;c++){
							color[c] = fallingPiece->color[c];
						}
					}else{
						gameSubState=1;
						break;
					}
				}
				grid->updateBlocks();
				fallingPiece->setShape(rand() % 6);
				fallingPiece->randomColor();
				fallingPiece->randomBombLoc(20);			
				fallingPiece->setPosition(grid,2,10,2);
				fallingPiece->randomRotation(grid);
			}
			timeTillFall = 0.5;
		
			glutPostRedisplay();
		}
	}
	lastTime = get_time();
}
//==========================================================================
// Main routine
//==========================================================================
int main ( int argc, char** argv )
 {
   App = new AppState;
   App->fovy = 60;
   App->rotx = 0;
   App->roty = 0;
   App->viewaxis = true;

   // Init GLUT:
   glutInit ( &argc,argv );
   glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

   glutInitWindowPosition( 20, 60 );
   glutInitWindowSize( 600, 600 );
   glutCreateWindow( "T3DRIS" );

   glEnable ( GL_DEPTH_TEST );
   glEnable ( GL_POINT_SMOOTH );
   glEnable ( GL_LINE_SMOOTH );
   glHint ( GL_LINE_SMOOTH_HINT, GL_NICEST );
   glHint ( GL_POINT_SMOOTH_HINT, GL_NICEST );
   glPointSize ( 6 );
   glLineWidth ( 1 );

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -5.0f, 5.0f, -12.0f, 1.0f };

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	

   // Set up GLUT callback functions for key presses
   glutKeyboardFunc ( appKeyboardFunc );
   glutSpecialFunc ( appKeyboardFunc );

  // Set up GLUT mouse callbacks
   glutMouseFunc ( appMouseFunc );
   glutMotionFunc ( appMotionFunc );

   // Set up GLUT callback function for resizing windows
   glutReshapeFunc( appResizeWindow );

   // Set up GLUT callback for drawing the scene
   glutDisplayFunc( appDrawScene );

   initTextures();

   srand (get_time());
   glutIdleFunc( appUpdate );
   lastTime = get_time();

   glutMainLoop ();
}

