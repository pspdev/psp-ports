#ifndef __egltypes_h_
#define __egltypes_h_

/*
** Types and resources
*/
typedef int EGLBoolean;
typedef int EGLint;

typedef void *EGLDisplay;
typedef void *EGLSurface;
typedef void *EGLContext;

typedef int EGLConfig;
typedef int NativeDisplayType;
typedef int NativeWindowType;
typedef int NativePixmapType;

/*
** EGL and native handle values
*/
#define EGL_DEFAULT_DISPLAY ((NativeDisplayType)0)
#define EGL_NO_CONTEXT ((EGLContext)0)
#define EGL_NO_DISPLAY ((EGLDisplay)0)
#define EGL_NO_SURFACE ((EGLSurface)0)

#ifndef APIENTRY
#define APIENTRY
#endif

#endif

