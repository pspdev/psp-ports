#include <GL/glut.h>
#include <GLES/egl.h>


GLUTAPI GLUTproc GLUTAPIENTRY glutGetProcAddress(const char *procName)
{
	return eglGetProcAddress (procName);
}

