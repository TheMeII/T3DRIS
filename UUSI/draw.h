//Derek Burch
// Ensure the header file is included only once in multi-file projects
// Pragma once is a non-standard but widely supported preprocessor directive
# pragma once

// draw 3D axis centered at origin and with each axis with given length
void draw_axis ( double r );

double get_time ();
//texture functions
GLuint loadTexture_FromBMP(char* file);
GLuint loadTexture_FromBMP(char* file, unsigned char ar,unsigned char ag,
						   unsigned char ab,bool useAlpha);
//function to write text to the screen
void renderStrokeFontString(
		float x, 
		float y, 
		float z, 
		float scale,
		void *font, 
		char *string);

/////////////////////////////////
//Texture Storage Functions
/////////////////////////////////
void initTextures();
class textures{
public:
	GLuint cubeTexture;
	GLuint bombTexture;

	textures(){
		cubeTexture=loadTexture_FromBMP("square.bmp");
		bombTexture=loadTexture_FromBMP("bomb.bmp");
	}
};

/////////////////////////////////
//Code for the grid object
//the class is done inline rather than dividing it between
//the header and cpp file
/////////////////////////////////
class Grid{
public:
	int ***occupied;
	float ****color;
	int xSize;
	int ySize;
	int zSize;
	int difficulty;
	unsigned int score;
	Grid(int xsize,int ysize,int zsize){
		occupied = new int**[xsize];
		color = new float***[xsize];
		for(int x=0;x<xsize;x++){
			occupied[x] = new int*[ysize];
			color[x] = new float**[ysize];
			for(int y=0;y<ysize;y++){
				occupied[x][y] = new int[zsize];			
				color[x][y] = new float*[zsize];
				for(int z=0;z<zsize;z++){
					occupied[x][y][z]=0;
					color[x][y][z] = new float[4];
					color[x][y][z][0] = 1.0*x/xsize;
					color[x][y][z][1] = 1.0*y/ysize;
					color[x][y][z][2] = 1.0*z/zsize;
					color[x][y][z][3] = 1.0;
				}
			}
		}
		xSize = xsize;
		ySize = ysize;
		zSize = zsize;
		score=0;
		difficulty=0;
	}
	void removeLayer(int layer){
		for(int x=0;x<xSize;x++){
			for(int z=0;z<zSize;z++){
				if(occupied[x][layer][z]>0){
					score+=40 + difficulty*difficulty*10;
				}
			}
		}
		for(int y=layer;y<ySize-1;y++){
			for(int x=0;x<xSize;x++){
				for(int z=0;z<zSize;z++){
					occupied[x][y][z]=occupied[x][y+1][z];
					for(int c=0;c<4;c++) color[x][y][z][c]= color[x][y+1][z][c];
				}
			}
		}
		for(int x=0;x<xSize;x++){
			for(int z=0;z<zSize;z++){
				occupied[x][ySize-1][z]=0;
				for(int c=0;c<4;c++) color[x][ySize-1][z][c]= 1.0;
			}
		}
	}
	//check for layer removal
	void updateBlocks(){
		//completed layer
		for(int y=0;y<ySize;y++){
			bool layerFilled = true;
			for(int x=0;x<xSize;x++){
				for(int z=0;z<zSize;z++){
					if(occupied[x][y][z]==0) layerFilled=false;
				}
			}
			if(layerFilled){
				removeLayer(y);
			}else{
				for(int x=0;x<xSize;x++){
					for(int z=0;z<zSize;z++){
						if(occupied[x][y][z]==2){
							bool xrowFilled = true;
							for(int z2=0;z2<zSize;z2++){
								if(occupied[x][y][z2]==0) xrowFilled=false;
							}		
							bool zrowFilled = true;
							for(int x2=0;x2<xSize;x2++){
								if(occupied[x2][y][z]==0) zrowFilled=false;
							}
							if((difficulty==0 && (xrowFilled || zrowFilled)) || 
								(difficulty==1 && xrowFilled && zrowFilled)){
								removeLayer(y);
								z=zSize;
								x=xSize;
							}
						}
					}
				}
			}
		}	
	}
};
/////////////////////////////
//Falling Object class
//Used to store information about the falling piece
//Also used to rotate and move the object
/////////////////////////////
class FallingObject{
public:
	float color[4];
	int pieceLoc[5][3];
	int position[3];
	int bombLocation;
	FallingObject(){
		//randomColor();
		color[3]=1.0f;
		randomBombLoc(20);
		position[0]=0;
		position[1]=0;
		position[2]=0;
		setShape(rand() % 6);
	};
	void setShape(int shape){
		switch(shape){
			case 0:
				//	||
				//	||
				//	||
				//	||
				//	||
				pieceLoc[0][0] = 0;
				pieceLoc[0][1] = -2;
				pieceLoc[0][2] = 0;
				pieceLoc[1][0] = 0;
				pieceLoc[1][1] = -1;
				pieceLoc[1][2] = 0;
				pieceLoc[2][0] = 0;
				pieceLoc[2][1] = 0;
				pieceLoc[2][2] = 0;
				pieceLoc[3][0] = 0;
				pieceLoc[3][1] = 1;
				pieceLoc[3][2] = 0;
				pieceLoc[4][0] = 0;
				pieceLoc[4][1] = 2;
				pieceLoc[4][2] = 0;

				color[0]=1.0;color[1]=0.0;color[2]=0.0;
				break;
			case 1:
				// ||
				// ||
				// || || ||
				pieceLoc[0][0] = -1;
				pieceLoc[0][1] = 1;
				pieceLoc[0][2] = 0;
				pieceLoc[1][0] = -1;
				pieceLoc[1][1] = 0;
				pieceLoc[1][2] = 0;
				pieceLoc[2][0] = -1;
				pieceLoc[2][1] = -1;
				pieceLoc[2][2] = 0;
				pieceLoc[3][0] = 0;
				pieceLoc[3][1] = -1;
				pieceLoc[3][2] = 0;
				pieceLoc[4][0] = 1;
				pieceLoc[4][1] = -1;
				pieceLoc[4][2] = 0;
				color[0]=0.0;color[1]=1.0;color[2]=0.0;
				break;
			case 2:
				//		|| 
				//	||	||	||
				//		||
				pieceLoc[0][0] = 0;
				pieceLoc[0][1] = 1;
				pieceLoc[0][2] = 0;
				pieceLoc[1][0] = -1;
				pieceLoc[1][1] = 0;
				pieceLoc[1][2] = 0;
				pieceLoc[2][0] = 0;
				pieceLoc[2][1] = 0;
				pieceLoc[2][2] = 0;
				pieceLoc[3][0] = 1;
				pieceLoc[3][1] = 0;
				pieceLoc[3][2] = 0;
				pieceLoc[4][0] = 0;
				pieceLoc[4][1] = -1;
				pieceLoc[4][2] = 0;

				color[0]=0.0;color[1]=0.0;color[2]=1.0;
				break;
			case 3:
				//	||	 
				//	||	||	||
				//	||	
				pieceLoc[0][0] = -1;
				pieceLoc[0][1] = 1;
				pieceLoc[0][2] = 0;
				pieceLoc[1][0] = -1;
				pieceLoc[1][1] = 0;
				pieceLoc[1][2] = 0;
				pieceLoc[2][0] = -1;
				pieceLoc[2][1] = -1;
				pieceLoc[2][2] = 0;
				pieceLoc[3][0] = 0;
				pieceLoc[3][1] = 0;
				pieceLoc[3][2] = 0;
				pieceLoc[4][0] = 1;
				pieceLoc[4][1] = 0;
				pieceLoc[4][2] = 0;

				color[0]=1.0;color[1]=1.0;color[2]=1.0;
				break;
			case 4:
				// ||
				// || || ||
				//       ||
				pieceLoc[0][0] = -1;
				pieceLoc[0][1] = 1;
				pieceLoc[0][2] = 0;
				pieceLoc[1][0] = -1;
				pieceLoc[1][1] = 0;
				pieceLoc[1][2] = 0;
				pieceLoc[2][0] = 0;
				pieceLoc[2][1] = 0;
				pieceLoc[2][2] = 0;
				pieceLoc[3][0] = 1;
				pieceLoc[3][1] = 0;
				pieceLoc[3][2] = 0;
				pieceLoc[4][0] = 1;
				pieceLoc[4][1] = -1;
				pieceLoc[4][2] = 0;

				color[0]=0.96;color[1]=0.478;color[2]=0.0;
				break;
			case 5:
				// || ||
				// || ||
				//    ||
				pieceLoc[0][0] = -1;
				pieceLoc[0][1] = 1;
				pieceLoc[0][2] = 0;
				pieceLoc[1][0] = -1;
				pieceLoc[1][1] = 0;
				pieceLoc[1][2] = 0;
				pieceLoc[2][0] = 0;
				pieceLoc[2][1] = 1;
				pieceLoc[2][2] = 0;
				pieceLoc[3][0] = 0;
				pieceLoc[3][1] = 0;
				pieceLoc[3][2] = 0;
				pieceLoc[4][0] = 0;
				pieceLoc[4][1] = -1;
				pieceLoc[4][2] = 0;

				color[0]=1.0;color[1]=1.0;color[2]=0.0;
				break;
		}
	}
	void randomColor(){
		switch(rand() % 4){
			case 0:
				color[0]=1.0;color[1]=0.0;color[2]=0.0;
				break;
			case 1:
				color[0]=0.0;color[1]=1.0;color[2]=0.0;
				break;
			case 2:
				color[0]=0.0;color[1]=0.0;color[2]=1.0;
				break;
			case 3:
				color[0]=1.0;color[1]=1.0;color[2]=1.0;
				break;
		}
		color[3]=1.0;
	}
	void randomBombLoc(int chance){
		bombLocation = rand() % chance;
	}
	void randomRotation(Grid* g){
		int r = rand() % 4;
		for(int i=0;i<r;i++)rotateY(g);
		r = rand() % 4;
		for(int i=0;i<r;i++)rotateX(g);
	}
	//returns false if the move was unsucessful
	bool move(Grid* g,int x,int y,int z){
		bool valid = false;
		int nextpos[3];
		nextpos[0]=position[0]+x;
		nextpos[1]=position[1]+y;
		nextpos[2]=position[2]+z;
		valid=checkLocation(g,nextpos,pieceLoc);

		if(valid){
			position[0]+=x;
			position[1]+=y;
			position[2]+=z;
			//printf("x=%d,y=%d,z=%d\n",position[0],position[1],position[2]);
		}
		return valid;
	};
	bool setPosition(Grid* g,int x,int y,int z){
			position[0]=x;
			position[1]=y;
			position[2]=z;
			//printf("SET x=%d,y=%d,z=%d\n",position[0],position[1],position[2]);
			return true;
	};
	bool checkAboveGrid(Grid* g){
		bool above = false;
		for(int p=0;p<5;p++){
			if(position[1]+pieceLoc[p][1] >= g->ySize) above = true;
		}	
		return above;
	};
	bool checkLocation(Grid* g,int pos[3], int locValues[5][3]){
			bool valid = true;
			for(int p=0;p<5;p++){
				int x = pos[0]+locValues[p][0];
				int y = pos[1]+locValues[p][1];
				int z = pos[2]+locValues[p][2];
					if(x<0 || y<0 || z<0 || x>=g->xSize || z>=g->zSize) valid=false;
					if(valid && y<g->ySize && g->occupied[pos[0]+locValues[p][0]][pos[1]+locValues[p][1]][pos[2]+locValues[p][2]]>0) valid=false;
			}	
			return valid;
	};

	bool rotateY(Grid* g){
		int nextpieceLoc[5][3];
		for(int p=0;p<5;p++){
				nextpieceLoc[p][0]=pieceLoc[p][2];
				nextpieceLoc[p][1]=pieceLoc[p][1];
				nextpieceLoc[p][2]=-pieceLoc[p][0];
		}
		if(checkLocation(g,position,nextpieceLoc)){
			for(int p=0;p<5;p++){
					pieceLoc[p][0]=nextpieceLoc[p][0];
					pieceLoc[p][1]=nextpieceLoc[p][1];
					pieceLoc[p][2]=nextpieceLoc[p][2];
			}
			return true;
		}
			else return false;
	};
	bool rotateYReverse(Grid* g){
		int nextpieceLoc[5][3];
		for(int p=0;p<5;p++){
				nextpieceLoc[p][0]=-pieceLoc[p][2];
				nextpieceLoc[p][1]=pieceLoc[p][1];
				nextpieceLoc[p][2]=pieceLoc[p][0];
		}
		if(checkLocation(g,position,nextpieceLoc)){
			for(int p=0;p<5;p++){
					pieceLoc[p][0]=nextpieceLoc[p][0];
					pieceLoc[p][1]=nextpieceLoc[p][1];
					pieceLoc[p][2]=nextpieceLoc[p][2];
			}
			return true;
		}
			else return false;
	};
	bool rotateZ(Grid* g){
		int nextpieceLoc[5][3];
		for(int p=0;p<5;p++){
				nextpieceLoc[p][0]=pieceLoc[p][1];
				nextpieceLoc[p][1]=-pieceLoc[p][0];
				nextpieceLoc[p][2]=pieceLoc[p][2];
		}
		if(checkLocation(g,position,nextpieceLoc)){
			for(int p=0;p<5;p++){
					pieceLoc[p][0]=nextpieceLoc[p][0];
					pieceLoc[p][1]=nextpieceLoc[p][1];
					pieceLoc[p][2]=nextpieceLoc[p][2];
			}
			return true;
		}
			else return false;
	};
	bool rotateZReverse(Grid* g){
		int nextpieceLoc[5][3];
		for(int p=0;p<5;p++){
				nextpieceLoc[p][0]=-pieceLoc[p][1];
				nextpieceLoc[p][1]=pieceLoc[p][0];
				nextpieceLoc[p][2]=pieceLoc[p][2];
		}
		if(checkLocation(g,position,nextpieceLoc)){
			for(int p=0;p<5;p++){
					pieceLoc[p][0]=nextpieceLoc[p][0];
					pieceLoc[p][1]=nextpieceLoc[p][1];
					pieceLoc[p][2]=nextpieceLoc[p][2];
			}
			return true;
		}
			else return false;
	};
	bool rotateX(Grid* g){
		int nextpieceLoc[5][3];
		for(int p=0;p<5;p++){
				nextpieceLoc[p][0]=pieceLoc[p][0];
				nextpieceLoc[p][1]=-pieceLoc[p][2];
				nextpieceLoc[p][2]=pieceLoc[p][1];
		}
		if(checkLocation(g,position,nextpieceLoc)){
			for(int p=0;p<5;p++){
					pieceLoc[p][0]=nextpieceLoc[p][0];
					pieceLoc[p][1]=nextpieceLoc[p][1];
					pieceLoc[p][2]=nextpieceLoc[p][2];
			}
			return true;
		}
			else return false;
	};
	bool rotateXReverse(Grid* g){
		int nextpieceLoc[5][3];
		for(int p=0;p<5;p++){
				nextpieceLoc[p][0]=pieceLoc[p][0];
				nextpieceLoc[p][1]=pieceLoc[p][2];
				nextpieceLoc[p][2]=-pieceLoc[p][1];
		}
		if(checkLocation(g,position,nextpieceLoc)){
			for(int p=0;p<5;p++){
					pieceLoc[p][0]=nextpieceLoc[p][0];
					pieceLoc[p][1]=nextpieceLoc[p][1];
					pieceLoc[p][2]=nextpieceLoc[p][2];
			}
			return true;
		}
			else return false;
	};
};
void draw_grid(Grid* g, FallingObject* o);