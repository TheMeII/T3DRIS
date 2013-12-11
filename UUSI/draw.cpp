//Derek Burch

# include <math.h>
# include <stdio.h>
# include <fstream>
# include "main.h"
# include "draw.h"

#ifdef WIN32
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

textures* Textures;
double get_time ()
{
  #ifdef WIN32
   // if better precision is needed in Windows, use QueryPerformanceCounter
   _timeb tp;
   _ftime_s(&tp);
   return 0.001*(double)tp.millitm + (double)tp.time;
  #else
   timeval tp;
   if ( gettimeofday(&tp,0)==-1 ) return 0;
   return 0.000001*(double)tp.tv_usec + (double)tp.tv_sec;
  #endif
} 

void initTextures(){
Textures = new textures();
}
void draw_axis ( double r )
 {
   double d=r/20.0;
   glBegin ( GL_LINES );

   glColor3f ( 1, 0, 0 );		
   glVertex3d ( -r, 0, 0 ); glVertex3d ( r, 0, 0 );
   glVertex3d ( r-d, -d, 0 ); glVertex3d ( r, 0, 0 );
   glVertex3d ( r-d, d, 0 ); glVertex3d ( r, 0, 0 );

   glColor3f ( 0, 1, 0 );		
   glVertex3d ( 0, -r, 0 ); glVertex3d ( 0, r, 0 );
   glVertex3d ( 0, r-d, -d ); glVertex3d ( 0, r, 0 );
   glVertex3d ( 0, r-d, d ); glVertex3d ( 0, r, 0 );

   glColor3f ( 0, 0, 1 );		
   glVertex3d ( 0, 0, -r ); glVertex3d ( 0, 0, r );
   glVertex3d ( 0, -d, r-d ); glVertex3d ( 0, 0, r );
   glVertex3d ( 0, d, r-d ); glVertex3d ( 0, 0, r );

   glEnd();
 }

//==========================================================================
// loadTexture_FromBMP is a helper function to make it simple to load bmp
// files without setting a color to alpha
// file points the a string with the name of the file to load.
//==========================================================================
GLuint loadTexture_FromBMP(char* file){
	return loadTexture_FromBMP(file,0,0,0,false);
}

//==========================================================================
// loadTexture_FromBMP is a function to load a bmp file into a texture
// the arguments ar,ag,ab determine which color will be considered as
// transparent (full alpha). useAlpha mus be true for the alpha colors to be
// considered. file points the a string with the name of the file to load.
// The function can only read 24-bit BMP files that have sizes in powers of 2
//==========================================================================
GLuint loadTexture_FromBMP(char* file, unsigned char ar,unsigned char ag,unsigned char ab,bool useAlpha){
	std::ifstream bmpFile(file, std::ios::in |std::ios::binary);

	if(bmpFile.is_open()){
		unsigned int FileSize = 0;
		unsigned int rawPixelDataSize = 0;
		unsigned int width = 0;
		unsigned int height = 0;
		
		GLuint texture = 0;

		//the file type is never tested and is assumed to be a 24-bit bmp
		//dont pass in other file types

		//move to correct positions and read their values
		bmpFile.seekg(2);
		bmpFile.read((char *)&FileSize,4);
		bmpFile.seekg(18);
		bmpFile.read((char *)&width,4);
		bmpFile.read((char *)&height,4);
		bmpFile.seekg(54);
		rawPixelDataSize = FileSize-54;
		printf("texture %s | found\n",file);
		printf("size=%d,width=%d,height=%d\n",FileSize,width,height);
		//load in the raw pixel data including any padding
		unsigned char* rawpixelData = new unsigned char[rawPixelDataSize];
		bmpFile.read((char*)rawpixelData,rawPixelDataSize);
		bmpFile.close(); //the file is no longer needed

		//determine the paddedWidth of the image(as a number of bytes)
		unsigned int paddedWidth = width*3;
		while(paddedWidth%4)paddedWidth++;

		//the images width must be a power of 2
		if(paddedWidth == width*3){
			unsigned char* purepixelData;
			if(useAlpha) purepixelData = new unsigned char[rawPixelDataSize+rawPixelDataSize/3];
			else purepixelData = new unsigned char[rawPixelDataSize];
			//if alpha is used colors matching ar,ag,ab will gain an alpha of 255
			//   otherwise an alpha of 0 is assigned
			if(useAlpha){
				for(unsigned int i=0;i < rawPixelDataSize/3;i++){
					purepixelData[i*4+2]=rawpixelData[i*3];
					purepixelData[i*4+1]=rawpixelData[i*3+1];
					purepixelData[i*4]=rawpixelData[i*3+2];
					if(purepixelData[i*4]==ar && purepixelData[i*4+1]==ag && purepixelData[i*4+2]==ab){ 
						purepixelData[i*4+3]=0;
					}else{
						purepixelData[i*4+3]=255;
					}
				}
			}else{
				for(unsigned int i=0;i < rawPixelDataSize;i+=3){
					purepixelData[i+2]=rawpixelData[i];
					purepixelData[i+1]=rawpixelData[i+1];
					purepixelData[i]=rawpixelData[i+2];
				}
			}
			glGenTextures( 1, &texture );
			glBindTexture( GL_TEXTURE_2D, texture );
			glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );//mix colors for shading
			//mip map settings
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				GL_LINEAR_MIPMAP_NEAREST );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			//repeat the texture if coords are greater than 1
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

			//create the 2d midmaps with the above parameters
			if(useAlpha)gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,width,
				height, GL_RGBA, GL_UNSIGNED_BYTE, purepixelData );
			else
				printf("Texture Read Result:%s\n",gluErrorString(gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width,
					height, GL_RGB, GL_UNSIGNED_BYTE, purepixelData )));
			delete [] purepixelData;
		}else{
			printf("Error - Invalid texture size: %s\n",file);
		}
		delete [] rawpixelData;		
		return texture;
	}
	//file read failed
	printf("Error - Unable to load texture: %s\n",file);
	return 0;
}

//==========================================================================
//function used to render a string of text to the screen
//source = http://www.lighthouse3d.com/opengl/glut/index.php?strokefont
//==========================================================================
void renderStrokeFontString(
		float x, 
		float y, 
		float z, 
		float scale,
		void *font, 
		char *string) {  
  char *c;
  glPushMatrix();
  glTranslatef(x, y,z);
  glScaled(scale,scale,scale);
  for (c=string; *c != '\0'; c++) {
    glutStrokeCharacter(font, *c);
  }
  glPopMatrix();
}

//==========================================================================
// setMaterial_Color is a shortcut function for setting the current fill color
//  used when drawing solids (sets material property)
//==========================================================================
void setMaterial_Color(float r,float g,float b,float a){
	float color[4];
	color[0]=r;
	color[1]=g;
	color[2]=b;
	color[3]=a;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

//==========================================================================
// Draws a solid box with the given paramenters
// size=width/height/depth
// tex=texture that is applied to each side of the cube
// the box is centered at the origin
//==========================================================================
void draw_texturedbox(double size,GLuint tex){
	glEnable( GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex );

	//front face
	glBegin(GL_POLYGON);
		glNormal3d(0,0,-1);
		glTexCoord2d(0,0);
		glVertex3d(size/2,-size/2,-size/2);
		glTexCoord2d(1,0);
		glVertex3d(-size/2,-size/2,-size/2);
		glTexCoord2d(1,1);
		glVertex3d(-size/2,size/2,-size/2);
		glTexCoord2d(0,1);
		glVertex3d(size/2,size/2,-size/2);
	glEnd();
	//back face
	glBegin(GL_POLYGON);
		glNormal3d(0,0,1);
		glTexCoord2d(0,0);
		glVertex3d(size/2,-size/2,size/2);
		glTexCoord2d(1,0);
		glVertex3d(-size/2,-size/2,size/2);
		glTexCoord2d(1,1);
		glVertex3d(-size/2,size/2,size/2);
		glTexCoord2d(0,1);
		glVertex3d(size/2,size/2,size/2);
	glEnd();
	//left face
	glBegin(GL_POLYGON);
		glNormal3d(1,0,0);
		glTexCoord2d(0.0,0.0);
		glVertex3d(size/2,-size/2,size/2);
		glTexCoord2d(1.0,0.0);
		glVertex3d(size/2,-size/2,-size/2);
		glTexCoord2d(1.0,1.0);
		glVertex3d(size/2,size/2,-size/2);
		glTexCoord2d(0.0,1.0);
		glVertex3d(size/2,size/2,size/2);
	glEnd();
	//right face
	glBegin(GL_POLYGON);
		glNormal3d(-1,0,0);
		glTexCoord2d(0,1);
		glVertex3d(-size/2,-size/2,size/2);
		glTexCoord2d(0,0);
		glVertex3d(-size/2,-size/2,-size/2);
		glTexCoord2d(1,0);
		glVertex3d(-size/2,size/2,-size/2);
		glTexCoord2d(1,1);
		glVertex3d(-size/2,size/2,size/2);
	glEnd();
	//top face
	glBegin(GL_POLYGON);
		glNormal3d(0,1,0);
		glTexCoord2d(0,1);
		glVertex3d(size/2,size/2,-size/2);
		glTexCoord2d(0,0);
		glVertex3d(-size/2,size/2,-size/2);
		glTexCoord2d(1,0);
		glVertex3d(-size/2,size/2,size/2);
		glTexCoord2d(1,1);
		glVertex3d(size/2,size/2,size/2);
	glEnd();
	//front face
	glBegin(GL_POLYGON);
		glNormal3d(0,-1,0);
		glTexCoord2d(0,1);
		glVertex3d(size/2,-size/2,-size/2);
		glTexCoord2d(0,0);
		glVertex3d(-size/2,-size/2,-size/2);
		glTexCoord2d(1,0);
		glVertex3d(-size/2,-size/2,size/2);
		glTexCoord2d(1,1);
		glVertex3d(size/2,-size/2,size/2);
	glEnd();
	glDisable( GL_TEXTURE_2D);
}

//==========================================================================
//function used to render the grid object and the falling object
//==========================================================================
void draw_grid(Grid* g, FallingObject *o){
	float squareSize = 2.5;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glTranslated(-(g->xSize-1)*squareSize/2,-6,-(g->zSize-1)*squareSize/2);
	
	//draw bottom grid
	glPushMatrix();
	glTranslated(-squareSize/2,-squareSize/2,-squareSize/2);
	for(int z=0;z<g->zSize;z++){
		for(int x=0;x<g->xSize;x++){
			glDisable(GL_LIGHTING);
			glColor3d(0.1,0.1,0.1);
		   glBegin ( GL_LINES );
	
		   glVertex3d ( squareSize*x, 0, squareSize*z ); glVertex3d ( squareSize*(x+1), 0, squareSize*z );
		   glVertex3d ( squareSize*(x+1), 0, squareSize*z );glVertex3d ( squareSize*(x+1), 0, squareSize*(z+1));
		   glVertex3d ( squareSize*(x+1), 0, squareSize*(z+1)); glVertex3d ( squareSize*x, 0, squareSize*(z+1) );
		   glVertex3d ( squareSize*x, 0, squareSize*(z+1) ); glVertex3d ( squareSize*x, 0, squareSize*z );

		   glEnd();
		   glEnable(GL_LIGHTING);
		}
	}
	//draw the elements in the grid
	glPopMatrix();
	for(int z=0;z<g->zSize;z++){
		for(int y=0;y<g->ySize;y++){
			for(int x=0;x<g->xSize;x++){
				if(g->occupied[x][y][z] > 0){
					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, g->color[x][y][z]);
					glPushMatrix();
					glTranslated(x*squareSize,y*squareSize,z*squareSize);
					if(g->difficulty <2 && g->occupied[x][y][z]==2) draw_texturedbox(squareSize,Textures->bombTexture);
					else draw_texturedbox(squareSize,Textures->cubeTexture);
					glPopMatrix();
				}
			}
		}
	}
	//draw the falling object
	for(int p = 0;p<5;p++){
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, o->color);
		glPushMatrix();
		glTranslated((o->position[0]+o->pieceLoc[p][0])*squareSize,(o->position[1]+o->pieceLoc[p][1])*squareSize,(o->position[2]+o->pieceLoc[p][2])*squareSize);
	//	if(g->difficulty <2 && o->bombLocation==p) draw_texturedbox(squareSize,Textures->bombTexture);
	//	else 
			draw_texturedbox(squareSize,Textures->cubeTexture);
		glPopMatrix();		
	}
	//draw the outline for where the falling object will land
	int helperPos[3] ;
	for(int i=0;i<3;i++)helperPos[i]=o->position[i];
	while(o->checkLocation(g,helperPos,o->pieceLoc)){
		helperPos[1]--;
	}
	helperPos[1]++;
	for(int p = 0;p<5;p++){
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, o->color);
		glPushMatrix();
		glTranslated((helperPos[0]+o->pieceLoc[p][0])*squareSize,(helperPos[1]+o->pieceLoc[p][1])*squareSize,(helperPos[2]+o->pieceLoc[p][2])*squareSize);
		glutWireCube(squareSize);
		glPopMatrix();		
	}
	glPopMatrix();
	glDisable(GL_BLEND);
}