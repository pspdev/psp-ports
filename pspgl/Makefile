PSPPATH := $(shell psp-config --psp-prefix)
PSPSDK := $(shell psp-config --pspsdk-path)
ARCH = psp-

CC = $(ARCH)gcc -std=gnu99
AR = $(ARCH)ar
RANLIB = $(ARCH)ranlib
RM = rm -f
CFLAGS = -g -Wall -Wmissing-prototypes -Os -G0 -fsingle-precision-constant -I. -I $(PSPPATH)/include -I $(PSPSDK)/include -funit-at-a-time
LFLAGS = -g -Wall -Os -G0 -L$(PSPPATH)/lib

DEPDIR = .deps

API_OBJS = \
	eglBindTexImage.o \
	eglChooseConfig.o \
	eglCreateContext.o \
	eglCreatePbufferSurface.o \
	eglCreateWindowSurface.o \
	eglDestroyContext.o \
	eglDestroySurface.o \
	eglGetConfigAttrib.o \
	eglGetConfigs.o \
	eglGetError.o \
	eglGetDisplay.o \
	eglInitialize.o \
	eglMakeCurrent.o \
	eglQueryString.o \
	eglSwapBuffers.o \
	eglSwapInterval.o \
	eglTerminate.o \
	eglWaitGL.o \
	eglWaitNative.o \
	glAlphaFunc.o \
	glArrayElement.o \
	glBegin.o \
	glDrawBezierArrays.o \
	glDrawBezierElements.o \
	glDrawSplineArrays.o \
	glDrawSplineElements.o \
	glBindBufferARB.o \
	glBindTexture.o \
	glBlendEquation.o \
	glBlendFunc.o \
	glBufferDataARB.o \
	glBufferSubDataARB.o \
	glClear.o \
	glClearColor.o \
	glClearDepth.o \
	glClearDepthf.o \
	glClearStencil.o \
	glColor.o \
	glColorMask.o \
	glColorPointer.o \
	glColorTable.o \
	glCompressedTexImage2D.o \
	glCopyTexImage2D.o \
	glCullFace.o \
	glDeleteBuffersARB.o \
	glDeleteTextures.o \
	glDepthFunc.o \
	glDepthMask.o \
	glDepthRange.o \
	glDepthRangef.o \
	glDrawArrays.o \
	glDrawBuffer.o \
	glDrawElements.o \
	glEnable.o \
	glEnableClientState.o \
	glEnd.o \
	glFinish.o \
	glFlush.o \
	glFog.o \
	glFrontFace.o \
	glFrustumf.o \
	glFrustum.o \
	glGenBuffersARB.o \
	glGenTextures.o \
	glGetBufferSubDataARB.o \
	glGetFloatv.o \
	glGetIntegerv.o \
	glGetError.o \
	glGetString.o \
	glInterleavedArrays.o \
	glIsTexture.o \
	glLight.o \
	glLightModel.o \
	glLineWidth.o \
	glLoadIdentity.o \
	glLoadMatrixf.o \
	glLockArraysEXT.o \
	glLogicOp.o \
	glMapBufferARB.o \
	glMaterial.o \
	glMatrixMode.o \
	glMultMatrixf.o \
	glNormal.o \
	glNormald.o \
	glNormalPointer.o \
	glOrtho.o \
	glOrthof.o \
	glPixelStore.o \
	glPushAttrib.o \
	glPushClientAttrib.o \
	glPopMatrix.o \
	glPushMatrix.o \
	glPolygonMode.o \
	glPolygonOffset.o \
	glPrioritizeTextures.o \
	glReadBuffer.o \
	glReadPixels.o \
	glRotatef.o \
	glScalef.o \
	glScaled.o \
	glScissor.o \
	glShadeModel.o \
	glStencilFunc.o \
	glStencilMask.o \
	glStencilOp.o \
	glTexCoord.o \
	glTexCoordPointer.o \
	glTexEnv.o \
	glTexGen.o \
	glTexImage2D.o \
	glTexSubImage2D.o \
	glTexParameter.o \
	glTranslatef.o \
	glUnmapBufferARB.o \
	glVertex.o \
	glVertexd.o \
	glVertexi.o \
	glVertexPointer.o \
	glViewport.o \
	glWeightPointerPSP.o \
	pspgl_buffers.o \
	pspgl_context.o \
	pspgl_copy_pixels.o \
	pspgl_dlist.o \
	pspgl_ge_init.o \
	pspgl_hash.o \
	pspgl_matrix.o \
	pspgl_misc.o \
	pspgl_stats.o \
	pspgl_texobj.o \
	pspgl_varray.o \
	pspgl_varray_draw.o \
	pspgl_varray_draw_elts.o \
	pspgl_varray_draw_range_elts.o \
	pspgl_vidmem.o

libGL.a_OBJS = \
	$(API_OBJS) \
	eglGetProcAddress.o

#	glGenLists.o
#	glIsList.o
#	glDeleteLists.o

libGLU.a_OBJS = \
	gluBuildMipmaps.o \
	gluLookAt.o \
	gluLookAtf.o \
	gluPerspective.o \
	gluPerspectivef.o \
	gluScaleImage.o \
	pspglu.o

libglut.a_OBJS = \
	glut.o \
	glutGetProcAddress.o

all: $(DEPDIR) $(libGL.a_OBJS) $(libGLU.a_OBJS) $(libglut.a_OBJS) libGL.a libGLU.a libglut.a


%.a: $(libGL.a_OBJS) $(libGLU.a_OBJS) $(libglut.a_OBJS)
	$(RM) $@
	$(AR) cru $@ $($@_OBJS)
	$(RANLIB) $@
	@$(ARCH)nm -o -fp -g --defined-only $@ | \
		awk '$$2~/^(gl|egl|glut|__pspgl)/ { next } \
				{ if (!bad) print "Bad symbols:"; print "\t", $$1, $$2; bad++ } \
			END	{ if (bad) { \
					print bad," bad symbol(s)"; exit(1) \
				  } else { \
					print "Namespace OK" } \
				}'


eglGetProcAddress.o: eglGetProcAddress.c pspgl_proctable_decls.h pspgl_proctable.h

# Extract all the public GL and EGL API symbols which are extensions (ends with PSP, ARB or EXT)
# Symbols must be sorted by name so that bsearch can be used to look for them.
pspgl_proctable.h: $(API_OBJS) Makefile
	$(ARCH)nm -fp -g --defined-only $(API_OBJS) | sort -k1 | \
		awk '$$2=="T" && $$1 ~ /^(gl|egl)[A-Z][a-zA-Z]/ \
			{ print "\t{ \"" $$1 "\", "$$1 " }," }' > $@ \
			|| rm -f $@

pspgl_proctable_decls.h: $(API_OBJS) Makefile
	$(ARCH)nm -fp -g --defined-only $(API_OBJS) | sort -k1 | \
		awk '$$2=="T" && $$1 ~ /^(gl|egl)[A-Z][a-zA-Z]/ \
			{ print "extern void " $$1 " (void);" }' > $@ \
			|| rm -f $@

$(DEPDIR):
	mkdir $(DEPDIR)

.c.o:
	$(CC) $(CFLAGS) -MD -MF $(DEPDIR)/$*.d -c $<

.S.o:
	$(CC) $(CFLAGS) -c $<

tar: clean
	( cd .. && tar cvfz pspgl-`date "+%Y-%m-%d"`.tar.gz pspgl --exclude "*.DS_Store" && cd - )

clean:
	$(RM) -rf *.o *.a $(DEPDIR) pspgl_proctable.h pspgl_proctable_decls.h
	make -C tools clean
	make -C tests clean
	make -C test-q3 clean
	make -C test-vfpu clean

install: all
	mkdir -p $(PSPPATH)/include $(PSPPATH)/lib
	mkdir -p $(PSPPATH)/include/GL $(PSPPATH)/include/GLES
	cp GL/*.h $(PSPPATH)/include/GL
	cp GLES/*.h $(PSPPATH)/include/GLES
	cp libGL.a $(PSPPATH)/lib
	cp libGLU.a $(PSPPATH)/lib
	cp libglut.a $(PSPPATH)/lib

-include $(wildcard $(DEPDIR)/*.d) dummy

