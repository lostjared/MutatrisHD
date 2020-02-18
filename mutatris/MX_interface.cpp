#include "MX_interface.h"
#include"SDL.h"
#include<sys/time.h>
#include<iostream>

void new_gluPerspective( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void new_gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
    glOrtho(left, right, bottom, top, -1, 1);
}

void new_gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
               GLfloat centerx, GLfloat centery, GLfloat centerz,
               GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {                   /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    glMultMatrixf(m);
    glTranslatef(-eyex, -eyey, -eyez);
}

namespace MX_i {

        SDL_Joystick *stick;
        SDL_GLContext glcontext;
        SDL_Window *window;
        SDL_TimerID timer_id;
        bool Buttons[0xFF];
        int cursor_x, cursory, cursor;
    
		void (*update_cb)();
		void (*idle_cb)();
		void (*resize_cb)(int, int);

		size_t total=0;

		int view_w, view_h;

		void Init(int *argc, char **argv, int w, int h) {
			SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER);
            view_w = w;
            view_h = h;
			SDL_JoystickEventState(SDL_ENABLE);
			stick = SDL_JoystickOpen(0);
		}

		void SetCallbacks( void (*update)(), void (*idle)(), void (*resize)(int w, int h)) {
			update_cb = update;
			resize_cb = resize;
			idle_cb = idle;

			
		    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
		    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

		    
            window = SDL_CreateWindow("Mutatris HD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, view_w, view_h, SDL_WINDOW_OPENGL);
            
		    glcontext = SDL_GL_CreateContext(window);
            glewExperimental = GL_TRUE;
            glewInit();
            std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";

		    SDL_GL_MakeCurrent(window, glcontext);
		    SDL_GL_SetSwapInterval(1);
		    resize_cb(view_w, view_h);
            
           // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		}

		bool done;

		void Loop() {

			SDL_Event e;

			done = false;

			while(!done) {

				while(SDL_PollEvent(&e)) {
					switch(e.type) {
					case SDL_QUIT:
						done = true;
					break;
                        case SDL_JOYBUTTONUP:
                            if(e.jbutton.button == 1) Buttons[B_1] = false;
                            break;
                        case SDL_JOYBUTTONDOWN:
                            if(e.jbutton.button == 1) Buttons[B_1] = true;
                            break;
                        case SDL_JOYHATMOTION:
                            
                            if(e.jhat.value & SDL_HAT_UP) { Buttons[B_UP] = true; break; }
                            else Buttons[B_UP] = false;
                         
                            
                            
                            if(e.jhat.value & SDL_HAT_LEFT) { Buttons[B_LEFT] = true; break; }
                            else  Buttons[B_LEFT] = false;
                                 
                            if(e.jhat.value & SDL_HAT_RIGHT) { Buttons[B_RIGHT] = true; break; }
                            else Buttons[B_RIGHT] = false;
                            
                            if(e.jhat.value & SDL_HAT_DOWN) { Buttons[B_DOWN] = true; break; }
                            else Buttons[B_DOWN] = false;
                            
                            
                            break;
                        case SDL_JOYAXISMOTION: {
                          
                            int numAxis = SDL_JoystickNumAxes(stick);
                            
                            if(e.jaxis.axis == 3) {
                                if(e.jaxis.value < -3200) {
                                    Buttons[B_3] = true;
                                } else Buttons[B_3] = false;
                                
                                if(e.jaxis.value > 3200) {
                                    Buttons[B_2] = true;
                                } else Buttons[B_2] = false;
                                
                            } else if(e.jaxis.axis == 2) {
                                if(e.jaxis.value < -3200) {
                                    Buttons[B_5] = true;
                                } else Buttons[B_5] = false;
            
                                if(e.jaxis.value > 3200)
                                    Buttons[B_4] = true;
                                else
                                    Buttons[B_4] = false;
                            }
                        
                        }
                            break;
					case SDL_KEYDOWN:
						if(e.key.keysym.sym == SDLK_ESCAPE) done = true;
						switch(e.key.keysym.sym) {
						case SDLK_LEFT:
							Buttons[B_LEFT] = true;
							break;
						case SDLK_RIGHT:
							Buttons[B_RIGHT] = true;
							break;
						case SDLK_UP:
							Buttons[B_UP] = true;
							break;
						case SDLK_DOWN:
							Buttons[B_DOWN] = true;
							break;
							break;
						case SDLK_a:
							Buttons[B_1] = true;
							break;
						case SDLK_s:
							Buttons[B_2] = true;
							break;
						case SDLK_d:
							Buttons[B_3] = true;
							break;
						case SDLK_z:
							Buttons[B_4] = true;
							break;
						case SDLK_x:
							Buttons[B_5] = true;
							break;
						case SDLK_e:
							Buttons[B_6] = true;
							break;
						default:
							break;

						}
						break;
						case SDL_KEYUP:
							switch(e.key.keysym.sym) {
							case SDLK_LEFT:
								Buttons[B_LEFT] = false;
								break;
							case SDLK_RIGHT:
								Buttons[B_RIGHT] = false;
								break;
							case SDLK_UP:
								Buttons[B_UP] = false;
								break;
							case SDLK_DOWN:
								Buttons[B_DOWN] = false;
								break;
							case SDLK_a:
								Buttons[B_1] = false;
								break;
							case SDLK_s:
								Buttons[B_2] = false;
								break;
							case SDLK_d:
								Buttons[B_3] = false;
								break;
							case SDLK_z:
								Buttons[B_4] = false;
								break;
							case SDLK_x:
								Buttons[B_5] = false;
								break;
							case SDLK_e:
								Buttons[B_6] = false;
								break;

							default:
								break;
							}
							break;
					case SDL_WINDOWEVENT:
						switch(e.window.event) {
						case SDL_WINDOWEVENT_CLOSE:
							done = true;
							break;
						}
						break;
                    case SDL_MOUSEMOTION:
                            cursor_x = e.motion.x;
                            cursory = e.motion.y;
                            cursor = e.motion.state;
                             if(cursor & SDL_BUTTON_RMASK)
                                mouseUp(e.motion.x, e.motion.y, e.motion.state);
                             else if(cursor & SDL_BUTTON_LMASK)
                                movementUp(e.motion.x, e.motion.y, e.motion.state);

                            break;
                    case SDL_MOUSEBUTTONDOWN:
                             mouseDown(e.button.x, e.button.y, e.button.state);
                            movementDown(e.button.x, e.button.y, e.button.state);
                            break;
                    case SDL_MOUSEBUTTONUP:
                            //mouseUp(e.button.x, e.button.y, e.button.state);
                            break;
                    }
					
				}
				idle_cb();
			}


		}

		void Quit() {
            SDL_RemoveTimer(timer_id);
    		SDL_JoystickClose(stick);
			SDL_GL_DeleteContext(glcontext);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		unsigned int (*timer_cb)(unsigned int t);

		Uint32 t_cb(Uint32 t, void *param) {
			return timer_cb(t);
		}
    
		void CreateTimer(unsigned int (*timer)(unsigned int t), int timeout) {
			timer_cb = timer;
			timer_id=SDL_AddTimer(timeout,t_cb,0);
    	}

		void SwapBuffers() {
			SDL_GL_SwapWindow(window);
		}

		void Redisplay() {
			update_cb();
		}

		unsigned int GetTicks() {
			return SDL_GetTicks();
		}

		bool PollController(enum Buttons b) {
			return Buttons[b];
		}
}
