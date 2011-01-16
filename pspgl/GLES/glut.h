#ifndef __glut_h__
#define __glut_h__

#include <GL/gl.h>

/* Display mode bit masks. */
#define GLUT_RGB			0
#define GLUT_RGBA			GLUT_RGB
//#define GLUT_INDEX			1
#define GLUT_SINGLE			0
#define GLUT_DOUBLE			2
#define GLUT_ACCUM			4
#define GLUT_ALPHA			8
#define GLUT_DEPTH			16
#define GLUT_STENCIL			32

/* mouse buttons and state */
#define GLUT_LEFT_BUTTON	0
#define GLUT_MIDDLE_BUTTON	1
#define GLUT_RIGHT_BUTTON	2
#define GLUT_DOWN		0
#define GLUT_UP			1

/* special keys */
#define GLUT_KEY_F1		1
#define GLUT_KEY_F2		2
#define GLUT_KEY_F3		3
#define GLUT_KEY_F4		4
#define GLUT_KEY_F5		5
#define GLUT_KEY_F6		6
#define GLUT_KEY_F7		7
#define GLUT_KEY_F8		8
#define GLUT_KEY_F9		9
#define GLUT_KEY_F10		10
#define GLUT_KEY_F11		11
#define GLUT_KEY_F12		12
#define GLUT_KEY_LEFT		100
#define GLUT_KEY_UP		101
#define GLUT_KEY_RIGHT		102
#define GLUT_KEY_DOWN		103
#define GLUT_KEY_PAGE_UP	104
#define GLUT_KEY_PAGE_DOWN	105
#define GLUT_KEY_HOME		106
#define GLUT_KEY_END		107
#define GLUT_KEY_INSERT		108


extern void glutInit (int *argcp, char **argv);

extern void glutInitDisplayMode (unsigned int mode);

extern void glutInitWindowPosition (int x, int y);

extern void glutInitWindowSize (int width, int height);

extern int glutCreateWindow (const char *title);

extern void glutMainLoop (void);

extern void glutPostRedisplay (void);

extern void glutSwapBuffers(void);

extern void glutDisplayFunc (void (*func) (void));

extern void glutReshapeFunc (void (*func) (int width, int height));

extern void glutKeyboardFunc (void (*func) (unsigned char key, int x, int y));

extern void glutMouseFunc (void (*func) (int button, int state, int x, int y));

extern void glutMotionFunc (void (*func) (int x, int y));

extern void glutPassiveMotionFunc (void (*func) (int x, int y));

extern void glutIdleFunc (void (*func) (void));

extern void glutTimerFunc (unsigned int millis, void (*func) (int value), int value);

extern void glutSpecialFunc (void (*func) (int key, int x, int y));

extern void glutKeyboardUpFunc (void (*func) (unsigned char key, int x, int y));

extern void glutSpecialUpFunc (void (*func) (int key, int x, int y));

extern void glutJoystickFunc (void (*func) (unsigned int buttonMask, int x, int y, int z), int pollInterval);


#endif

