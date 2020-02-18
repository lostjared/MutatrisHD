#ifndef __HEADER__H__
#define __HEADER__H__


#include"glew.h"
#include<cmath>

void new_gluPerspective( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );
void new_gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
void new_gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,GLfloat centerx, GLfloat centery,GLfloat centerz,GLfloat upx, GLfloat upy, GLfloat upz);

namespace MX_i {
		
        extern int FPS;
        extern float dt;
        extern int cursor_x, cursory, cursor;
        
        void Init(int *argc, char **argv, int w, int h);
		void SetCallbacks(void (*update)(), void (*idle)(), void (*resize)(int w, int h));
		void Loop();
		void Quit();
		void SwapBuffers();
		void Redisplay();
		void CreateTimer(unsigned int (*timer)(unsigned int i), int timeout);
		GLubyte *LoadBMP(const char *src, int *w, int *h);
		unsigned int GetTicks();
		enum Buttons { B_UP=0, B_DOWN, B_LEFT, B_RIGHT, B_1,B_2, B_3, B_4, B_5, B_6};
		bool PollController(enum Buttons b);
	}

void mouseDown(int x, int y, int state);
void mouseUp(int x, int y, int state);
void movementUp(int x, int y, int state);
void movementDown(int x, int y, int state);

#endif


