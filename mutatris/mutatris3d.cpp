#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<unistd.h>
#include "MX_interface.h"
#include "mpmut.h"
#include"cmx_video.h"
#include"cmx_font.h"
#include"fixedsys.h"
#include<acidcam/ac.h>
#define WIDTH 1920
#define HEIGHT 1080


enum { INTRO, GAME, GAME_OVER };
int cur_screen = INTRO;
mp::mxMut mutatris;
GLuint textures[10];
GLuint background_texture;
GLuint logo_texture;
cmx::font::Font the_font;
cmx::video::Surface score_surface;

cv::VideoCapture start_cap, cap;

namespace MX_i {

    int FPS = 30;
    float dt = 0;
    
}

using MX_i::FPS;
using MX_i::dt;
int old_t = 0;


GLfloat vertices[] = { -1.0f, -1.0f, 1.0f, // front face
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,

					-1.0f, -1.0f, 1.0f,
					1.0f, -1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,

					-1.0f, -1.0f, -1.0f, // left side
					-1.0f, -1.0f, 1.0f,
					-1.0f, 1.0f, -1.0f,

					-1.0f, 1.0f, -1.0f,
					-1.0f, -1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,

					-1.0f, 1.0f, -1.0f, // top
					-1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,

					1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f,

					-1.0f, -1.0f, -1.0f, // bottom
					-1.0f, -1.0f, 1.0f,
					1.0f, -1.0f, 1.0f,

					1.0f, -1.0f, 1.0f,
					1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f, -1.0f,

					1.0f, -1.0f, -1.0f, // right
					1.0f, -1.0f, 1.0f,
					1.0f, 1.0f, -1.0f,

					1.0f, 1.0f, -1.0f,
					1.0f, -1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,

					-1.0f, -1.0f, -1.0f, // back face
					1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f,

					-1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, 1.0f, -1.0f,

	};

GLfloat texCoords[] = {
				0, 0, // front
				1, 1,
				0, 1,

				0, 0,
				1, 0,
				1, 1,

				0, 0, // left
				1, 0,
				0, 1,

				0, 1,
				1, 0,
				1, 1,

				0,0, // top
				0,1,
				1,1,

				1, 1,
				1, 0,
				0, 0,

				0, 0,// bottom
				0, 1,
				1, 1,

				1,1,
				1,0,
				0,0,

				0,0,// right
				1,0,
				0,1,

				0,1,
				1,0,
				1,1,

				0,0, // back
				1,1,
				0,1,

				0,0,
				1,0,
				1,1
	};


GLfloat rot[4] = {0,-10,0,0};//{ -25, -20, 0, 0};
int width=WIDTH, height=HEIGHT;

static std::vector<std::string> mirror_array{"MirrorLeft", "MirrorRight", "MirrorTopToBottom", "MirrorBottomToTop", "MirrorSwitch", "MirrorSwitchFlip", "MirrorLeftMirrorRightBlend", "MirrorTopMirrorBottomBlend", "MirrorAll", "MirrorLeftBottomToTop", "MirrorRightTopToBottom", "MirrorFadeLeftRight", "MirrorFadeUpDown", "MirrorFadeAll", "MirrorSwitchMode", "MirrorSwitchLeftRight", "MatrixCollectionFramesMirrorLeft", "MirrorDiamond", "MirrorDiamondRight", "MirrorDiamondReverse", "MirrorLeftTopToBottom", "MirrorRightBottomToTop", "MirrorFlipLeft", "MirrorFlipRight", "MirrorFlipBottomLeft", "MirrorFlipBottomRight", "MirrorFlipXMirrorLeft", "MirrorFlipXMirrorRight", "MirrorFlipYMirrorLeft", "MirrorFlipYMirrorRight", "MirrorFlipXLeftTopToBottom", "MirrorFlipXLeftBottomToTop", "MirrorFlipXRightTopToBottom", "MirrorFlipXRightBottomToTop", "MirrorFlipYLeftTopToBottom", "MirrorFlipYLeftBottomToTop", "MirrorFlipYRightTopToBottom","MirrorFlipYRightBottomToTop","MedianBlendMultiThread","SelfAlphaBlend","MedianBlendMultiThreadByEight","DarkColorFibonacci"
};

int current_filter = 0;


unsigned int timer_callback(unsigned int t) {

	if(cur_screen != GAME) return t;

	if(MX_i::PollController(MX_i::B_LEFT)) {
        current_filter = ac::filter_map[mirror_array[rand()%mirror_array.size()]];
		mutatris.moveLeft();
	}
	else if(MX_i::PollController(MX_i::B_RIGHT)) {
        current_filter = ac::filter_map[mirror_array[rand()%mirror_array.size()]];
		mutatris.moveRight();
	}
	else if(MX_i::PollController(MX_i::B_UP)) {
        current_filter = ac::filter_map[mirror_array[rand()%mirror_array.size()]];
		mutatris.moveInward();

	} else if(MX_i::PollController(MX_i::B_DOWN)) {
        current_filter = ac::filter_map[mirror_array[rand()%mirror_array.size()]];
			mutatris.moveDown();
	} else if(MX_i::PollController(MX_i::B_1)) mutatris.shiftColor();

	static int counter = 0;
    
	mutatris.grid.update();

	if ((++counter%4) == 0)
		mutatris.update();

	if(mutatris.game_over) {
		cur_screen = GAME_OVER;
	}

	return t;
}

void idle() {
/*	if(cur_screen == GAME) {

		static unsigned int time_var = (unsigned int) time(0);
		unsigned int wait = (unsigned int)time(0);
		if(wait != time_var) {
			mutatris.update();
			time_var = (unsigned int) time(0);
		}

	} */
    
    
    int t = MX_i::GetTicks();
    int passed = t - old_t;
    old_t = t ;
    dt = passed;
    
    
	if(MX_i::PollController(MX_i::B_2)) rot[0] += 0.1 * dt;
	else if(MX_i::PollController(MX_i::B_3)) rot[0] -= 0.1f * dt;
	else if(MX_i::PollController(MX_i::B_4)) rot[1] += 0.1f * dt;
	else if(MX_i::PollController(MX_i::B_5)) rot[1] -= 0.1f * dt;
	else if(MX_i::PollController(MX_i::B_6)) { rot[0] = 0; rot[1] = 0; rot[2] = 0; }
    
    
	MX_i::Redisplay();
}
static int x1, x2, y1x, y2;

void mouseDown(int x, int y, int state) {
    x1 = x;
    y1x = y;
}

void mouseUp(int x, int y, int state) {
    x2 = x;
    y2 = y;
    if(x1 < x2 && abs(x1-x2) > 10) {
        rot[1] -= 0.1f * dt;
        x1 = x;
        y1x = y;
    }
    else if(x1 > x2 && abs(x2-x1) > 10) {
        rot[1] += 0.1f * dt;
        x1 = x;
        y1x = y;
    }

    if(y1x < y2 && abs(y1x-y2) > 10) {
        rot[0] -= 0.1f * dt;
        x1 = x;
        y1x = y;
    }
    else if(y1x > y2 && abs(y2-y1x) > 10) {
        rot[0] += 0.1f * dt;
        x1 = x;
        y1x = y;
    }
    current_filter = ac::filter_map[mirror_array[rand()%mirror_array.size()]];
}

void movementUp(int x, int y, int state) {
    x2 = x;
    y2 = y;
    if(x1 < x2 && abs(x1-x2) > 40) {
        x1 = x;
        y1x = y;
        mutatris.moveRight();

    }
    else if(x1 > x2 && abs(x2-x1) > 40) {
        mutatris.moveLeft();
        x1 = x;
        y1x = y;
    }

    if(y1x < y2 && abs(y1x-y2) > 40) {
        x1 = x;
        y1x = y;
        mutatris.moveDown();
    }
    else if(y1x > y2 && abs(y2-y1x) > 40) {
        mutatris.shiftColor();
        x1 = x;
        y1x = y;
    }
    current_filter = ac::filter_map[mirror_array[rand()%mirror_array.size()]];
}

void movementDown(int x, int y, int state) {
    x1 = x;
    y1x = y;
}

void movementScroll(int y) {
    if(y < 0) {
        mutatris.moveInward();
    } else if(y > 0) {
        mutatris.moveOutward();
    }
}


GLfloat frontFace[] = {
		-1.0f, -1.0f, 1.0f, // front face
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
};

GLfloat frontTexture[] = {
		0, 0, // front
		1, 1,
		0, 1,

		0, 0,
		1, 0,
		1, 1,
};

void renderGame() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	new_gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// draw in 2D
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background_texture);
    cv::Mat frame;
    if(!cap.read(frame)) {
        cap.open("img/bg1.mp4");
        cap.read(frame);
    }
    cv::Mat out = frame.clone();
    cv::flip(out, frame, 0);
    ac::CallFilter(current_filter, frame);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
     
    glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, frontFace);
	glTexCoordPointer(2, GL_FLOAT, 0, frontTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	new_gluPerspective(45.0f, float(width/height), 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  	new_gluLookAt(0, 0, 120, 0, 0, 0, 0, 1, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glTranslatef(-32.0f,30.0f, 0.0f);
  	int i,z,d;
    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
    glRotatef(rot[2], 0.0f, 0.0f, 1.0f);
    glPushMatrix();
    glDisable (GL_BLEND);
	 for(i = 0; i < mutatris.GRID_W; i++) {
		  for(z = 0; z < mutatris.GRID_H; z++) {
			  for(d = 0; d < mutatris.GRID_Z; d++) {
                  static float rotPos[mutatris.GRID_W][mutatris.GRID_H][mutatris.GRID_Z] = { {{0, 0, 0}} };
			  float blockPos = d*10.0f;
			  float blockXPos =i*2.5f;
			  float blockYPos = z*3.0;
			  if(mutatris.grid.data[i][z][d] != 0) {
				   if(mutatris.grid.data[i][z][d] == -1) {
					   glPushMatrix();
					   glTranslatef(blockXPos, -blockYPos, blockPos);
					   glRotatef(rotPos[i][z][d], 0, 1, 0);
					   rotPos[i][z][d] += 5.0f;
					   if(rotPos[i][z][d] >= 360) {
						   rotPos[i][z][d] = 0;
						   mutatris.grid.data[i][z][d] = 0;
					   }

                       glBindTexture(GL_TEXTURE_2D, textures[1+rand()%5]);
					   glDrawArrays(GL_TRIANGLES, 0, 36);
					   glPopMatrix();
				   } else {
					   glPushMatrix();
                       glTranslatef(blockXPos, -blockYPos, blockPos);
					   glBindTexture(GL_TEXTURE_2D, textures[mutatris.grid.data[i][z][d]-1]);
					   glDrawArrays(GL_TRIANGLES, 0, 36);
					   glPopMatrix();
				   }
			  }
			}
		  }
	 }
	 glPopMatrix();
	 for(int q = 0; q < 4; q++) {
		 int xPos = mutatris.current.x+mutatris.current.blocks[q].x;
		 int yPos = mutatris.current.y+mutatris.current.blocks[q].y;
		 int zPos = mutatris.current.z+mutatris.current.blocks[q].z;
		 float blockXpos = xPos*2.5f;
		 float blockYpos = yPos*3.0f;
		 float blockZpos = zPos*10.0f;
		 glPushMatrix();
		 glTranslatef(blockXpos, -blockYpos, blockZpos);
		 glBindTexture(GL_TEXTURE_2D, textures[mutatris.current.blocks[q].index_color-1]);
		 glDrawArrays(GL_TRIANGLES, 0, 36);
         glPopMatrix();
         
	 }
    
    glDisable(GL_BLEND);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	score_surface.clearRect(score_surface.objectRect(), 0x0);
	std::ostringstream stream;
	stream << "Score: " << mutatris.grid.score << " Cleared: " <<  mutatris.grid.blocks_cleared;
	the_font.printString((unsigned int*)score_surface.buffer, score_surface.w, 5, 5, 35, 30, _RGB(255,255,255), stream.str().c_str());
	glPushMatrix();
	glRasterPos2f(0, 0);
	glPixelZoom(1.0f,-1.0f);
	glDrawPixels(score_surface.w, score_surface.h, GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte*)score_surface.buffer);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);


}

static float intro_zPos = 0, intro_yRot = 0;


void renderIntro() {
    
   //glEnable (GL_BLEND);
   // glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);
  
   
    
    
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	//gluOrtho2D(0, 640, 0, 480);
	new_gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, logo_texture);
    cv::Mat frame;
    if(!start_cap.read(frame)) {
        start_cap.open("img/start_logo.mp4");
        start_cap.read(frame);
    }
    cv::Mat out = frame.clone();
    cv::flip(out, frame, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
    
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, frontFace);
	glTexCoordPointer(2, GL_FLOAT, 0, frontTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	new_gluPerspective(45.0f, (float)(width/height), 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glTranslatef(0, 0, intro_zPos);

	if(intro_zPos < -10) {
		glRotatef(intro_yRot, 0, 1, 0);
		intro_yRot += 0.1f * dt;
		if(intro_yRot > 360) {
			cur_screen = GAME;
		} else if(intro_yRot > 180) {
			static float xRot = 0.0f;
			xRot += 0.1f * dt;
			glRotatef(xRot, 1, 0, 0);
		}
	} else intro_zPos -= 0.01f * dt;

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glBindTexture(GL_TEXTURE_2D, logo_texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void renderGameOver() {

	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	new_gluOrtho2D(0, 640, 0, 480);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2f(5, 480);
	glPixelZoom(1.0f, -1.0f);
	std::ostringstream stream;
	stream << "Game Over your Score was: " << mutatris.grid.score;
	score_surface.clearRect(score_surface.objectRect(), 0x0);
	the_font.printString((unsigned int*)score_surface.buffer, score_surface.w, 5, 5, 35, 30, _RGB(255,255,255), stream.str().c_str());
	glDrawPixels(score_surface.w, score_surface.h, GL_RGBA, GL_UNSIGNED_BYTE, (GLubyte*)score_surface.buffer);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	new_gluPerspective(45.0f, (width/height), 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -2.5f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glBindTexture(GL_TEXTURE_2D, logo_texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if(MX_i::PollController(MX_i::B_1)) {
		cur_screen = INTRO;
        start_cap.open("img/start_logo.mp4");
		intro_zPos = intro_yRot = 0;
		mutatris.newGame();
	}
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
	glPushMatrix();
	switch(cur_screen) {
	case GAME:
		renderGame();
		break;
	case INTRO:
		renderIntro();
		break;
	case GAME_OVER:
		renderGameOver();
		break;
	}

	glPopMatrix();
	MX_i::SwapBuffers();
}

void resize(int w, int h) {
	if(w <= 0 || h <= 0) w = 1, h = 1;
	width = w, height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	new_gluPerspective(45.0f, float(w / h), 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void genTextureFromFile(const char *f, GLuint &tex) {
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int w=0,h=0;
	GLubyte *img = MX_i::LoadBMP(f, &w, &h);
	if(img == 0) {
		fprintf(stderr, "Error could not load: %s\n", f);
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	free(img);
}

void  init() {
	the_font.loadFont((const void *)fixedsys_bin, sizeof(fixedsys_bin));
	score_surface.createSurface(500, 45, 32);
	glClearColor(0, 0, 0, 1);
#ifdef FOR_WIZ
	glClearDepthf(1.0f);
#else
	glClearDepth(1.0f);
#endif
	glEnable(GL_DEPTH_TEST);
	resize(width, height);
	srand((unsigned int)time(0));
	mutatris.newGame();
	glEnable(GL_TEXTURE_2D);
	genTextureFromFile("img/gamebg.bmp", background_texture);
	genTextureFromFile("img/logo.bmp", logo_texture);
	glGenTextures(10, textures);
	for(unsigned int i=1; i<=10; i++) {
		glBindTexture(GL_TEXTURE_2D, textures[i-1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    char str[1024];
	    sprintf(str, "img/block%d.bmp", i);
	    int w, h;
	    GLubyte *bmp = MX_i::LoadBMP(str, &w, &h);
	    if(bmp == 0) {
	    	fprintf(stderr, "Error could not load %s \n", str);
	    	return;
	    }
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);
	    free(bmp);
	}
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	/*GLfloat mat_specular[] = { 1, 1, 1, 1 };
	GLfloat mat_shine[] = { 1 };
	GLfloat light_position[] = { 1,1,1, 0 };
	GLfloat white_light[] = { 1, 1, 1,1 };
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); */
	MX_i::CreateTimer(timer_callback, 150);
}

void clean() {
	glDeleteTextures(1, &background_texture);
	glDeleteTextures(10, textures);
}

int main(int argc, char **argv) {
/*
    
	std::string path = argv[0];
	std::string temp_path = path.substr(0, path.rfind("/"));
	path += "/../Resources";
	std::cout << temp_path << "\n";
	chdir(path.c_str()); */
  
    ac::init();
    srand(static_cast<int>(time(0)));
    current_filter = ac::filter_map[mirror_array[rand()%mirror_array.size()]];
    int cx = 0;
    int cy = 0;
    
    if(argc == 3 || argc == 4) {
        cx = atoi(argv[1]);
        cy = atoi(argv[2]);
        
        if(cx <= 0 || cy <= 0 ) {
            std::cerr << "Error must pass valid resolution\n";
            return -1;
        }
        
    } else {
        cx = WIDTH;
        cy = HEIGHT;
    }
    cap.open("img/bg1.mp4");
    start_cap.open("img/start_logo.mov");
    if(!cap.isOpened()) {
        std::cerr << "Could not load file...\n";
        exit(EXIT_FAILURE);
    }
    
	MX_i::Init(&argc, argv, cx, cy);
	MX_i::SetCallbacks(render, idle, resize);
	init();
    if(argc == 4 && std::string(argv[3]) =="-f") {
        MX_i::toggleFullScreen();
    }
	MX_i::Loop();
	clean();
	MX_i::Quit();
	return 0;
}

