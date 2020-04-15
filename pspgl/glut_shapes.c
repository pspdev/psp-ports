/**
(c) Copyright 2020, iyenal (IDTechnologies)
GLUT Copyrights and Trigonometric functions:
(c) Copyright 1993, Silicon Graphics, Inc.
(c) Copyright Mark J. Kilgard, 1994. 
(c) The OpenGL Group
ALL RIGHTS RESERVED
Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies and that
both the copyright notice and this permission notice appear in
supporting documentation, and that the name of Silicon
Graphics, Inc. not be used in advertising or publicity
pertaining to distribution of the software without specific,
written prior permission.
THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU
"AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR
OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
EVENT SHALL SILICON GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE
ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL, INDIRECT OR
CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE,
SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR
NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF THE POSSIBILITY
OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE OR
PERFORMANCE OF THIS SOFTWARE.
US Government Users Restricted Rights
Use, duplication, or disclosure by the Government is subject to
restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer
Software clause at DFARS 252.227-7013 and/or in similar or
successor clauses in the FAR or the DOD or NASA FAR
Supplement.  Unpublished-- rights reserved under the copyright
laws of the United States.  Contractor/manufacturer is Silicon
Graphics, Inc., 2011 N.  Shoreline Blvd., Mountain View, CA
94039-7311.
OpenGL(TM) is a trademark of Silicon Graphics, Inc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* Make it not a power of two to avoid cache thrashing on the chip */
#define CACHE_SIZE	240


struct GLUquadric {
    GLint 	normals;
    GLboolean	textureCoords;
    GLint	orientation;
    GLint	drawStyle;
    void	(GLAPI *errorCallback)( GLint );
};

GLUquadric * GLAPI
gluNewQuadric(void)
{
    GLUquadric *newstate;

    newstate = (GLUquadric *) malloc(sizeof(GLUquadric));
    if (newstate == NULL) {
	/* Can't report an error at this point... */
	return NULL;
    }
    newstate->normals = GLU_SMOOTH;
    newstate->textureCoords = GL_FALSE;
    newstate->orientation = GLU_OUTSIDE;
    newstate->drawStyle = GLU_FILL;
    newstate->errorCallback = NULL; 
    return newstate;
}

static GLUquadricObj *quadObj;

#define QUAD_OBJ_INIT() { if(!quadObj) initQuadObj(); if(!quadObj) return; }
static void
initQuadObj(void)
{
  quadObj = gluNewQuadric();
  //if (!quadObj)
  //__glutFatalError("out of memory.");
}


void GLAPI
gluDeleteQuadric(GLUquadric *state)
{
    free(state);
}

static void gluQuadricError(GLUquadric *qobj, GLenum which)
{
    if (qobj->errorCallback) {
	qobj->errorCallback(which);
    }
}

void GLAPI
gluQuadricCallback(GLUquadric *qobj, GLenum which, void (GLAPI *fn)())
{
    switch (which) {
      case GLU_ERROR:
	qobj->errorCallback = (void (GLAPI *)(GLint)) fn;
	break;
      default:
	gluQuadricError(qobj, GLU_INVALID_ENUM);
	return;
    }
}

void GLAPI
gluQuadricNormals(GLUquadric *qobj, GLenum normals)
{
    switch (normals) {
      case GLU_SMOOTH:
      case GLU_FLAT:
      case GLU_NONE:
	break;
      default:
	gluQuadricError(qobj, GLU_INVALID_ENUM);
	return;
    }
    qobj->normals = normals;
}

void GLAPI
gluQuadricTexture(GLUquadric *qobj, GLboolean textureCoords)
{
    qobj->textureCoords = textureCoords;
}

void GLAPI
gluQuadricOrientation(GLUquadric *qobj, GLenum orientation)
{
    switch(orientation) {
      case GLU_OUTSIDE:
      case GLU_INSIDE:
	break;
      default:
	gluQuadricError(qobj, GLU_INVALID_ENUM);
	return;
    }
    qobj->orientation = orientation;
}

void GLAPI
gluQuadricDrawStyle(GLUquadric *qobj, GLenum drawStyle)
{
    switch(drawStyle) {
      case GLU_POINT:
      case GLU_LINE:
      case GLU_FILL:
      case GLU_SILHOUETTE:
	break;
      default:
	gluQuadricError(qobj, GLU_INVALID_ENUM);
	return;
    }
    qobj->drawStyle = drawStyle;
}

void GLAPI
gluCylinder(GLUquadric *qobj, GLdouble baseRadius, GLdouble topRadius, 
		GLdouble height, GLint slices, GLint stacks)
{
    GLint i,j,max;
    GLfloat sinCache[CACHE_SIZE];
    GLfloat cosCache[CACHE_SIZE];
    GLfloat sinCache2[CACHE_SIZE];
    GLfloat cosCache2[CACHE_SIZE];
    GLfloat sinCache3[CACHE_SIZE];
    GLfloat cosCache3[CACHE_SIZE];
    GLfloat angle;
    GLfloat x, y, zLow, zHigh;
    GLfloat sintemp, costemp;
    GLfloat length;
    GLfloat deltaRadius;
    GLfloat zNormal;
    GLfloat xyNormalRatio;
    GLfloat radiusLow, radiusHigh;
    int needCache2, needCache3;

    if (slices >= CACHE_SIZE) slices = CACHE_SIZE-1;

    if (slices < 2 || stacks < 1 || baseRadius < 0.0 || topRadius < 0.0 ||
	    height < 0.0) {
	gluQuadricError(qobj, GLU_INVALID_VALUE);
	return;
    }

    /* Compute length (needed for normal calculations) */
    deltaRadius = baseRadius - topRadius;
    length = sqrt(deltaRadius*deltaRadius + height*height);
    if (length == 0.0) {
	gluQuadricError(qobj, GLU_INVALID_VALUE);
	return;
    }

    /* Cache is the vertex locations cache */
    /* Cache2 is the various normals at the vertices themselves */
    /* Cache3 is the various normals for the faces */
    needCache2 = needCache3 = 0;
    if (qobj->normals == GLU_SMOOTH) {
	needCache2 = 1;
    }

    if (qobj->normals == GLU_FLAT) {
	if (qobj->drawStyle != GLU_POINT) {
	    needCache3 = 1;
	}
	if (qobj->drawStyle == GLU_LINE) {
	    needCache2 = 1;
	}
    }

    zNormal = deltaRadius / length;
    xyNormalRatio = height / length;

    for (i = 0; i < slices; i++) {
	angle = 2 * 3.14159265358979323846 * i / slices;
	if (needCache2) {
	    if (qobj->orientation == GLU_OUTSIDE) {
		sinCache2[i] = xyNormalRatio * sin(angle);
		cosCache2[i] = xyNormalRatio * cos(angle);
	    } else {
		sinCache2[i] = -xyNormalRatio * sin(angle);
		cosCache2[i] = -xyNormalRatio * cos(angle);
	    }
	} 
	sinCache[i] = sin(angle);
	cosCache[i] = cos(angle);
    }

    if (needCache3) {
	for (i = 0; i < slices; i++) {
	    angle = 2 * 3.14159265358979323846 * (i-0.5) / slices;
	    if (qobj->orientation == GLU_OUTSIDE) {
		sinCache3[i] = xyNormalRatio * sin(angle);
		cosCache3[i] = xyNormalRatio * cos(angle);
	    } else {
		sinCache3[i] = -xyNormalRatio * sin(angle);
		cosCache3[i] = -xyNormalRatio * cos(angle);
	    }
	}
    } 

    sinCache[slices] = sinCache[0];
    cosCache[slices] = cosCache[0];
    if (needCache2) {
	sinCache2[slices] = sinCache2[0];
	cosCache2[slices] = cosCache2[0];
    }
    if (needCache3) {
	sinCache3[slices] = sinCache3[0];
	cosCache3[slices] = cosCache3[0];
    }

    switch (qobj->drawStyle) {
      case GLU_FILL:
	/* Note:
	** An argument could be made for using a TRIANGLE_FAN for the end
	** of the cylinder of either radii is 0.0 (a cone).  However, a 
	** TRIANGLE_FAN would not work in smooth shading mode (the common 
	** case) because the normal for the apex is different for every
	** triangle (and TRIANGLE_FAN doesn't let me respecify that normal).
	** Now, my choice is GL_TRIANGLES, or leave the GL_QUAD_STRIP and
	** just let the GL trivially reject one of the two triangles of the
	** QUAD.  GL_QUAD_STRIP is probably faster, so I will leave this code
	** alone.
	*/
	for (j = 0; j < stacks; j++) {
	    zLow = j * height / stacks;
	    zHigh = (j + 1) * height / stacks;
	    radiusLow = baseRadius - deltaRadius * ((float) j / stacks);
	    radiusHigh = baseRadius - deltaRadius * ((float) (j + 1) / stacks);

	    glBegin(GL_QUAD_STRIP);
	    for (i = 0; i <= slices; i++) {
		switch(qobj->normals) {
		  case GLU_FLAT:
		    glNormal3f(sinCache3[i], cosCache3[i], zNormal);
		    break;
		  case GLU_SMOOTH:
		    glNormal3f(sinCache2[i], cosCache2[i], zNormal);
		    break;
		  case GLU_NONE:
		  default:
		    break;
		}
		if (qobj->orientation == GLU_OUTSIDE) {
		    if (qobj->textureCoords) {
			glTexCoord2f(1 - (float) i / slices,
				(float) j / stacks);
		    }
		    glVertex3f(radiusLow * sinCache[i], 
			    radiusLow * cosCache[i], zLow);
		    if (qobj->textureCoords) {
			glTexCoord2f(1 - (float) i / slices,
				(float) (j+1) / stacks);
		    }
		    glVertex3f(radiusHigh * sinCache[i], 
			    radiusHigh * cosCache[i], zHigh);
		} else {
		    if (qobj->textureCoords) {
			glTexCoord2f(1 - (float) i / slices,
				(float) (j+1) / stacks);
		    }
		    glVertex3f(radiusHigh * sinCache[i], 
			    radiusHigh * cosCache[i], zHigh);
		    if (qobj->textureCoords) {
			glTexCoord2f(1 - (float) i / slices,
				(float) j / stacks);
		    }
		    glVertex3f(radiusLow * sinCache[i], 
			    radiusLow * cosCache[i], zLow);
		}
	    }
	    glEnd();
	}
	break;
      case GLU_POINT:
	glBegin(GL_POINTS);
	for (i = 0; i < slices; i++) {
	    switch(qobj->normals) {
	      case GLU_FLAT:
	      case GLU_SMOOTH:
		glNormal3f(sinCache2[i], cosCache2[i], zNormal);
		break;
	      case GLU_NONE:
	      default:
		break;
	    }
	    sintemp = sinCache[i];
	    costemp = cosCache[i];
	    for (j = 0; j <= stacks; j++) {
		zLow = j * height / stacks;
		radiusLow = baseRadius - deltaRadius * ((float) j / stacks);

		if (qobj->textureCoords) {
		    glTexCoord2f(1 - (float) i / slices,
			    (float) j / stacks);
		}
		glVertex3f(radiusLow * sintemp, 
			radiusLow * costemp, zLow);
	    }
	}
	glEnd();
	break;
      case GLU_LINE:
	for (j = 1; j < stacks; j++) {
	    zLow = j * height / stacks;
	    radiusLow = baseRadius - deltaRadius * ((float) j / stacks);

	    glBegin(GL_LINE_STRIP);
	    for (i = 0; i <= slices; i++) {
		switch(qobj->normals) {
		  case GLU_FLAT:
		    glNormal3f(sinCache3[i], cosCache3[i], zNormal);
		    break;
		  case GLU_SMOOTH:
		    glNormal3f(sinCache2[i], cosCache2[i], zNormal);
		    break;
		  case GLU_NONE:
		  default:
		    break;
		}
		if (qobj->textureCoords) {
		    glTexCoord2f(1 - (float) i / slices,
			    (float) j / stacks);
		}
		glVertex3f(radiusLow * sinCache[i], 
			radiusLow * cosCache[i], zLow);
	    }
	    glEnd();
	}
	/* Intentionally fall through here... */
      case GLU_SILHOUETTE:
	for (j = 0; j <= stacks; j += stacks) {
	    zLow = j * height / stacks;
	    radiusLow = baseRadius - deltaRadius * ((float) j / stacks);

	    glBegin(GL_LINE_STRIP);
	    for (i = 0; i <= slices; i++) {
		switch(qobj->normals) {
		  case GLU_FLAT:
		    glNormal3f(sinCache3[i], cosCache3[i], zNormal);
		    break;
		  case GLU_SMOOTH:
		    glNormal3f(sinCache2[i], cosCache2[i], zNormal);
		    break;
		  case GLU_NONE:
		  default:
		    break;
		}
		if (qobj->textureCoords) {
		    glTexCoord2f(1 - (float) i / slices,
			    (float) j / stacks);
		}
		glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 
			zLow);
	    }
	    glEnd();
	}
	for (i = 0; i < slices; i++) {
	    switch(qobj->normals) {
	      case GLU_FLAT:
	      case GLU_SMOOTH:
		glNormal3f(sinCache2[i], cosCache2[i], 0.0);
		break;
	      case GLU_NONE:
	      default:
		break;
	    }
	    sintemp = sinCache[i];
	    costemp = cosCache[i];
	    glBegin(GL_LINE_STRIP);
	    for (j = 0; j <= stacks; j++) {
		zLow = j * height / stacks;
		radiusLow = baseRadius - deltaRadius * ((float) j / stacks);

		if (qobj->textureCoords) {
		    glTexCoord2f(1 - (float) i / slices,
			    (float) j / stacks);
		}
		glVertex3f(radiusLow * sintemp, 
			radiusLow * costemp, zLow);
	    }
	    glEnd();
	}
	break;
      default:
	break;
    }
}

void GLAPI
gluDisk(GLUquadric *qobj, GLdouble innerRadius, GLdouble outerRadius, 
	    GLint slices, GLint loops)
{
    gluPartialDisk(qobj, innerRadius, outerRadius, slices, loops, 0.0, 360.0);
}

void GLAPI
gluPartialDisk(GLUquadric *qobj, GLdouble innerRadius, 
		   GLdouble outerRadius, GLint slices, GLint loops,
		   GLdouble startAngle, GLdouble sweepAngle)
{
    GLint i,j,max;
    GLfloat sinCache[CACHE_SIZE];
    GLfloat cosCache[CACHE_SIZE];
    GLfloat angle;
    GLfloat x, y;
    GLfloat sintemp, costemp;
    GLfloat deltaRadius;
    GLfloat radiusLow, radiusHigh;
    GLfloat texLow, texHigh;
    GLfloat angleOffset;
    GLint slices2;
    GLint finish;

    if (slices >= CACHE_SIZE) slices = CACHE_SIZE-1;
    if (slices < 2 || loops < 1 || outerRadius <= 0.0 || innerRadius < 0.0 ||
	    innerRadius > outerRadius) {
	gluQuadricError(qobj, GLU_INVALID_VALUE);
	return;
    }

    if (sweepAngle < -360.0) sweepAngle = 360.0;
    if (sweepAngle > 360.0) sweepAngle = 360.0;
    if (sweepAngle < 0) {
	startAngle += sweepAngle;
	sweepAngle = -sweepAngle;
    }

    if (sweepAngle == 360.0) {
	slices2 = slices;
    } else {
	slices2 = slices + 1;
    }

    /* Compute length (needed for normal calculations) */
    deltaRadius = outerRadius - innerRadius;

    /* Cache is the vertex locations cache */

    angleOffset = startAngle / 180.0 * 3.14159265358979323846;
    for (i = 0; i <= slices; i++) {
	angle = angleOffset + ((3.14159265358979323846 * sweepAngle) / 180.0) * i / slices;
	sinCache[i] = sin(angle);
	cosCache[i] = cos(angle);
    }

    if (sweepAngle == 360.0) {
	sinCache[slices] = sinCache[0];
	cosCache[slices] = cosCache[0];
    }

    switch(qobj->normals) {
      case GLU_FLAT:
      case GLU_SMOOTH:
	if (qobj->orientation == GLU_OUTSIDE) {
	    glNormal3f(0.0, 0.0, 1.0);
	} else {
	    glNormal3f(0.0, 0.0, -1.0);
	}
	break;
      default:
      case GLU_NONE:
	break;
    }

    switch (qobj->drawStyle) {
      case GLU_FILL:
	if (innerRadius == 0.0) {
	    finish = loops - 1;
	    /* Triangle strip for inner polygons */
	    glBegin(GL_TRIANGLE_FAN);
	    if (qobj->textureCoords) {
		glTexCoord2f(0.5, 0.5);
	    }
	    glVertex3f(0.0, 0.0, 0.0);
	    radiusLow = outerRadius - 
		    deltaRadius * ((float) (loops-1) / loops);
	    if (qobj->textureCoords) {
		texLow = radiusLow / outerRadius / 2;
	    }

	    if (qobj->orientation == GLU_OUTSIDE) {
		for (i = slices; i >= 0; i--) {
		    if (qobj->textureCoords) {
			glTexCoord2f(texLow * sinCache[i] + 0.5,
				texLow * cosCache[i] + 0.5);
		    }
		    glVertex3f(radiusLow * sinCache[i], 
			    radiusLow * cosCache[i], 0.0);
		}
	    } else {
		for (i = 0; i <= slices; i++) {
		    if (qobj->textureCoords) {
			glTexCoord2f(texLow * sinCache[i] + 0.5,
				texLow * cosCache[i] + 0.5);
		    }
		    glVertex3f(radiusLow * sinCache[i], 
			    radiusLow * cosCache[i], 0.0);
		}
	    }
	    glEnd();
	} else {
	    finish = loops;
	}
	for (j = 0; j < finish; j++) {
	    radiusLow = outerRadius - deltaRadius * ((float) j / loops);
	    radiusHigh = outerRadius - deltaRadius * ((float) (j + 1) / loops);
	    if (qobj->textureCoords) {
		texLow = radiusLow / outerRadius / 2;
		texHigh = radiusHigh / outerRadius / 2;
	    }

	    glBegin(GL_QUAD_STRIP);
	    for (i = 0; i <= slices; i++) {
		if (qobj->orientation == GLU_OUTSIDE) {
		    if (qobj->textureCoords) {
			glTexCoord2f(texLow * sinCache[i] + 0.5,
				texLow * cosCache[i] + 0.5);
		    }
		    glVertex3f(radiusLow * sinCache[i], 
			    radiusLow * cosCache[i], 0.0);

		    if (qobj->textureCoords) {
			glTexCoord2f(texHigh * sinCache[i] + 0.5,
				texHigh * cosCache[i] + 0.5);
		    }
		    glVertex3f(radiusHigh * sinCache[i], 
			    radiusHigh * cosCache[i], 0.0);
		} else {
		    if (qobj->textureCoords) {
			glTexCoord2f(texHigh * sinCache[i] + 0.5,
				texHigh * cosCache[i] + 0.5);
		    }
		    glVertex3f(radiusHigh * sinCache[i], 
			    radiusHigh * cosCache[i], 0.0);

		    if (qobj->textureCoords) {
			glTexCoord2f(texLow * sinCache[i] + 0.5,
				texLow * cosCache[i] + 0.5);
		    }
		    glVertex3f(radiusLow * sinCache[i], 
			    radiusLow * cosCache[i], 0.0);
		}
	    }
	    glEnd();
	}
	break;
      case GLU_POINT:
	glBegin(GL_POINTS);
	for (i = 0; i < slices2; i++) {
	    sintemp = sinCache[i];
	    costemp = cosCache[i];
	    for (j = 0; j <= loops; j++) {
		radiusLow = outerRadius - deltaRadius * ((float) j / loops);

		if (qobj->textureCoords) {
		    texLow = radiusLow / outerRadius / 2;

		    glTexCoord2f(texLow * sinCache[i] + 0.5,
			    texLow * cosCache[i] + 0.5);
		}
		glVertex3f(radiusLow * sintemp, radiusLow * costemp, 0.0);
	    }
	}
	glEnd();
	break;
      case GLU_LINE:
	if (innerRadius == outerRadius) {
	    glBegin(GL_LINE_STRIP);

	    for (i = 0; i <= slices; i++) {
		if (qobj->textureCoords) {
		    glTexCoord2f(sinCache[i] / 2 + 0.5,
			    cosCache[i] / 2 + 0.5);
		}
		glVertex3f(innerRadius * sinCache[i],
			innerRadius * cosCache[i], 0.0);
	    }
	    glEnd();
	    break;
	}
	for (j = 0; j <= loops; j++) {
	    radiusLow = outerRadius - deltaRadius * ((float) j / loops);
	    if (qobj->textureCoords) {
		texLow = radiusLow / outerRadius / 2;
	    }

	    glBegin(GL_LINE_STRIP);
	    for (i = 0; i <= slices; i++) {
		if (qobj->textureCoords) {
		    glTexCoord2f(texLow * sinCache[i] + 0.5,
			    texLow * cosCache[i] + 0.5);
		}
		glVertex3f(radiusLow * sinCache[i], 
			radiusLow * cosCache[i], 0.0);
	    }
	    glEnd();
	}
	for (i=0; i < slices2; i++) {
	    sintemp = sinCache[i];
	    costemp = cosCache[i];
	    glBegin(GL_LINE_STRIP);
	    for (j = 0; j <= loops; j++) {
		radiusLow = outerRadius - deltaRadius * ((float) j / loops);
		if (qobj->textureCoords) {
		    texLow = radiusLow / outerRadius / 2;
		}

		if (qobj->textureCoords) {
		    glTexCoord2f(texLow * sinCache[i] + 0.5,
			    texLow * cosCache[i] + 0.5);
		}
		glVertex3f(radiusLow * sintemp, radiusLow * costemp, 0.0);
	    }
	    glEnd();
	}
	break;
      case GLU_SILHOUETTE:
	if (sweepAngle < 360.0) {
	    for (i = 0; i <= slices; i+= slices) {
		sintemp = sinCache[i];
		costemp = cosCache[i];
		glBegin(GL_LINE_STRIP);
		for (j = 0; j <= loops; j++) {
		    radiusLow = outerRadius - deltaRadius * ((float) j / loops);

		    if (qobj->textureCoords) {
			texLow = radiusLow / outerRadius / 2;
			glTexCoord2f(texLow * sinCache[i] + 0.5,
				texLow * cosCache[i] + 0.5);
		    }
		    glVertex3f(radiusLow * sintemp, radiusLow * costemp, 0.0);
		}
		glEnd();
	    }
	}
	for (j = 0; j <= loops; j += loops) {
	    radiusLow = outerRadius - deltaRadius * ((float) j / loops);
	    if (qobj->textureCoords) {
		texLow = radiusLow / outerRadius / 2;
	    }

	    glBegin(GL_LINE_STRIP);
	    for (i = 0; i <= slices; i++) {
		if (qobj->textureCoords) {
		    glTexCoord2f(texLow * sinCache[i] + 0.5,
			    texLow * cosCache[i] + 0.5);
		}
		glVertex3f(radiusLow * sinCache[i], 
			radiusLow * cosCache[i], 0.0);
	    }
	    glEnd();
	    if (innerRadius == outerRadius) break;
	}
	break;
      default:
	break;
    }
}

void GLAPI
gluSphere(GLUquadric *qobj, GLdouble radius, GLint slices, GLint stacks)
{
    GLint i,j,max;
    GLfloat sinCache1a[CACHE_SIZE];
    GLfloat cosCache1a[CACHE_SIZE];
    GLfloat sinCache2a[CACHE_SIZE];
    GLfloat cosCache2a[CACHE_SIZE];
    GLfloat sinCache3a[CACHE_SIZE];
    GLfloat cosCache3a[CACHE_SIZE];
    GLfloat sinCache1b[CACHE_SIZE];
    GLfloat cosCache1b[CACHE_SIZE];
    GLfloat sinCache2b[CACHE_SIZE];
    GLfloat cosCache2b[CACHE_SIZE];
    GLfloat sinCache3b[CACHE_SIZE];
    GLfloat cosCache3b[CACHE_SIZE];
    GLfloat angle;
    GLfloat x, y, zLow, zHigh;
    GLfloat sintemp1, sintemp2, sintemp3, sintemp4;
    GLfloat costemp1, costemp2, costemp3, costemp4;
    GLfloat zNormal;
    GLfloat xyNormalRatio;
    GLboolean needCache2, needCache3;
    GLint start, finish;

    if (slices >= CACHE_SIZE) slices = CACHE_SIZE-1;
    if (stacks >= CACHE_SIZE) stacks = CACHE_SIZE-1;
    if (slices < 2 || stacks < 1 || radius < 0.0) {
	gluQuadricError(qobj, GLU_INVALID_VALUE);
	return;
    }

    /* Cache is the vertex locations cache */
    /* Cache2 is the various normals at the vertices themselves */
    /* Cache3 is the various normals for the faces */
    needCache2 = needCache3 = GL_FALSE;

    if (qobj->normals == GLU_SMOOTH) {
	needCache2 = GL_TRUE;
    }

    if (qobj->normals == GLU_FLAT) {
	if (qobj->drawStyle != GLU_POINT) {
	    needCache3 = GL_TRUE;
	}
	if (qobj->drawStyle == GLU_LINE) {
	    needCache2 = GL_TRUE;
	}
    }

    for (i = 0; i < slices; i++) {
	angle = 2 * 3.14159265358979323846 * i / slices;
	sinCache1a[i] = sin(angle);
	cosCache1a[i] = cos(angle);
	if (needCache2) {
	    sinCache2a[i] = sinCache1a[i];
	    cosCache2a[i] = cosCache1a[i];
	}
    }

    for (j = 0; j <= stacks; j++) {
	angle = 3.14159265358979323846 * j / stacks;
	if (needCache2) {
	    if (qobj->orientation == GLU_OUTSIDE) {
		sinCache2b[j] = sin(angle);
		cosCache2b[j] = cos(angle);
	    } else {
		sinCache2b[j] = -sin(angle);
		cosCache2b[j] = -cos(angle);
	    }
	}
	sinCache1b[j] = radius * sin(angle);
	cosCache1b[j] = radius * cos(angle);
    }
    /* Make sure it comes to a point */
    sinCache1b[0] = 0;
    sinCache1b[stacks] = 0;

    if (needCache3) {
	for (i = 0; i < slices; i++) {
	    angle = 2 * 3.14159265358979323846 * (i-0.5) / slices;
	    sinCache3a[i] = sin(angle);
	    cosCache3a[i] = cos(angle);
	}
	for (j = 0; j <= stacks; j++) {
	    angle = 3.14159265358979323846 * (j - 0.5) / stacks;
	    if (qobj->orientation == GLU_OUTSIDE) {
		sinCache3b[j] = sin(angle);
		cosCache3b[j] = cos(angle);
	    } else {
		sinCache3b[j] = -sin(angle);
		cosCache3b[j] = -cos(angle);
	    }
	}
    } 

    sinCache1a[slices] = sinCache1a[0];
    cosCache1a[slices] = cosCache1a[0];
    if (needCache2) {
	sinCache2a[slices] = sinCache2a[0];
	cosCache2a[slices] = cosCache2a[0];
    }
    if (needCache3) {
	sinCache3a[slices] = sinCache3a[0];
	cosCache3a[slices] = cosCache3a[0];
    }

    switch (qobj->drawStyle) {
      case GLU_FILL:
	/* Do ends of sphere as TRIANGLE_FAN's (if not texturing)
	** We don't do it when texturing because we need to respecify the
	** texture coordinates of the apex for every adjacent vertex (because
	** it isn't a constant for that point)
	*/
	if (!(qobj->textureCoords)) {
	    start = 1;
	    finish = stacks - 1;

	    /* Low end first (j == 0 iteration) */
	    sintemp2 = sinCache1b[1];
	    zHigh = cosCache1b[1];
	    switch(qobj->normals) {
	      case GLU_FLAT:
		sintemp3 = sinCache3b[1];
		costemp3 = cosCache3b[1];
		break;
	      case GLU_SMOOTH:
		sintemp3 = sinCache2b[1];
		costemp3 = cosCache2b[1];
		glNormal3f(sinCache2a[0] * sinCache2b[0],
			cosCache2a[0] * sinCache2b[0],
			cosCache2b[0]);
		break;
	      default:
		break;
	    }
	    glBegin(GL_TRIANGLE_FAN);
	    glVertex3f(0.0, 0.0, radius);
	    if (qobj->orientation == GLU_OUTSIDE) {
		for (i = slices; i >= 0; i--) {
		    switch(qobj->normals) {
		      case GLU_SMOOTH:
			glNormal3f(sinCache2a[i] * sintemp3,
				cosCache2a[i] * sintemp3,
				costemp3);
			break;
		      case GLU_FLAT:
			if (i != slices) {
			    glNormal3f(sinCache3a[i+1] * sintemp3,
				    cosCache3a[i+1] * sintemp3,
				    costemp3);
			}
			break;
		      case GLU_NONE:
		      default:
			break;
		    }
		    glVertex3f(sintemp2 * sinCache1a[i],
			    sintemp2 * cosCache1a[i], zHigh);
		}
	    } else {
		for (i = 0; i <= slices; i++) {
		    switch(qobj->normals) {
		      case GLU_SMOOTH:
			glNormal3f(sinCache2a[i] * sintemp3,
				cosCache2a[i] * sintemp3,
				costemp3);
			break;
		      case GLU_FLAT:
			glNormal3f(sinCache3a[i] * sintemp3,
				cosCache3a[i] * sintemp3,
				costemp3);
			break;
		      case GLU_NONE:
		      default:
			break;
		    }
		    glVertex3f(sintemp2 * sinCache1a[i],
			    sintemp2 * cosCache1a[i], zHigh);
		}
	    }
	    glEnd();

	    /* High end next (j == stacks-1 iteration) */
	    sintemp2 = sinCache1b[stacks-1];
	    zHigh = cosCache1b[stacks-1];
	    switch(qobj->normals) {
	      case GLU_FLAT:
		sintemp3 = sinCache3b[stacks];
		costemp3 = cosCache3b[stacks];
		break;
	      case GLU_SMOOTH:
		sintemp3 = sinCache2b[stacks-1];
		costemp3 = cosCache2b[stacks-1];
		glNormal3f(sinCache2a[stacks] * sinCache2b[stacks],
			cosCache2a[stacks] * sinCache2b[stacks],
			cosCache2b[stacks]);
		break;
	      default:
		break;
	    }
	    glBegin(GL_TRIANGLE_FAN);
	    glVertex3f(0.0, 0.0, -radius);
	    if (qobj->orientation == GLU_OUTSIDE) {
		for (i = 0; i <= slices; i++) {
		    switch(qobj->normals) {
		      case GLU_SMOOTH:
			glNormal3f(sinCache2a[i] * sintemp3,
				cosCache2a[i] * sintemp3,
				costemp3);
			break;
		      case GLU_FLAT:
			glNormal3f(sinCache3a[i] * sintemp3,
				cosCache3a[i] * sintemp3,
				costemp3);
			break;
		      case GLU_NONE:
		      default:
			break;
		    }
		    glVertex3f(sintemp2 * sinCache1a[i],
			    sintemp2 * cosCache1a[i], zHigh);
		}
	    } else {
		for (i = slices; i >= 0; i--) {
		    switch(qobj->normals) {
		      case GLU_SMOOTH:
			glNormal3f(sinCache2a[i] * sintemp3,
				cosCache2a[i] * sintemp3,
				costemp3);
			break;
		      case GLU_FLAT:
			if (i != slices) {
			    glNormal3f(sinCache3a[i+1] * sintemp3,
				    cosCache3a[i+1] * sintemp3,
				    costemp3);
			}
			break;
		      case GLU_NONE:
		      default:
			break;
		    }
		    glVertex3f(sintemp2 * sinCache1a[i],
			    sintemp2 * cosCache1a[i], zHigh);
		}
	    }
	    glEnd();
	} else {
	    start = 0;
	    finish = stacks;
	}
	for (j = start; j < finish; j++) {
	    zLow = cosCache1b[j];
	    zHigh = cosCache1b[j+1];
	    sintemp1 = sinCache1b[j];
	    sintemp2 = sinCache1b[j+1];
	    switch(qobj->normals) {
	      case GLU_FLAT:
		sintemp4 = sinCache3b[j+1];
		costemp4 = cosCache3b[j+1];
		break;
	      case GLU_SMOOTH:
		if (qobj->orientation == GLU_OUTSIDE) {
		    sintemp3 = sinCache2b[j+1];
		    costemp3 = cosCache2b[j+1];
		    sintemp4 = sinCache2b[j];
		    costemp4 = cosCache2b[j];
		} else {
		    sintemp3 = sinCache2b[j];
		    costemp3 = cosCache2b[j];
		    sintemp4 = sinCache2b[j+1];
		    costemp4 = cosCache2b[j+1];
		}
		break;
	      default:
	        break;
	    }

	    glBegin(GL_QUAD_STRIP);
	    for (i = 0; i <= slices; i++) {
		switch(qobj->normals) {
		  case GLU_SMOOTH:
		    glNormal3f(sinCache2a[i] * sintemp3,
			    cosCache2a[i] * sintemp3,
			    costemp3);
		    break;
		  case GLU_FLAT:
		  case GLU_NONE:
		  default:
		    break;
		}
		if (qobj->orientation == GLU_OUTSIDE) {
		    if (qobj->textureCoords) {
			glTexCoord2f(1 - (float) i / slices,
				1 - (float) (j+1) / stacks);
		    }
		    glVertex3f(sintemp2 * sinCache1a[i],
			    sintemp2 * cosCache1a[i], zHigh);
		} else {
		    if (qobj->textureCoords) {
			glTexCoord2f(1 - (float) i / slices,
				1 - (float) j / stacks);
		    }
		    glVertex3f(sintemp1 * sinCache1a[i],
			    sintemp1 * cosCache1a[i], zLow);
		}
		switch(qobj->normals) {
		  case GLU_SMOOTH:
		    glNormal3f(sinCache2a[i] * sintemp4,
			    cosCache2a[i] * sintemp4,
			    costemp4);
		    break;
		  case GLU_FLAT:
		    glNormal3f(sinCache3a[i] * sintemp4,
			    cosCache3a[i] * sintemp4,
			    costemp4);
		    break;
		  case GLU_NONE:
		  default:
		    break;
		}
		if (qobj->orientation == GLU_OUTSIDE) {
		    if (qobj->textureCoords) {
			glTexCoord2f(1 - (float) i / slices,
				1 - (float) j / stacks);
		    }
		    glVertex3f(sintemp1 * sinCache1a[i],
			    sintemp1 * cosCache1a[i], zLow);
		} else {
		    if (qobj->textureCoords) {
			glTexCoord2f(1 - (float) i / slices,
				1 - (float) (j+1) / stacks);
		    }
		    glVertex3f(sintemp2 * sinCache1a[i],
			    sintemp2 * cosCache1a[i], zHigh);
		}
	    }
	    glEnd();
	}
	break;
      case GLU_POINT:
	glBegin(GL_POINTS);
	for (j = 0; j <= stacks; j++) {
	    sintemp1 = sinCache1b[j];
	    costemp1 = cosCache1b[j];
	    switch(qobj->normals) {
	      case GLU_FLAT:
	      case GLU_SMOOTH:
		sintemp2 = sinCache2b[j];
		costemp2 = cosCache2b[j];
		break;
	      default:
	        break;
	    }
	    for (i = 0; i < slices; i++) {
		switch(qobj->normals) {
		  case GLU_FLAT:
		  case GLU_SMOOTH:
		    glNormal3f(sinCache2a[i] * sintemp2,
			    cosCache2a[i] * sintemp2,
			    costemp2);
		    break;
		  case GLU_NONE:
		  default:
		    break;
		}

		zLow = j * radius / stacks;

		if (qobj->textureCoords) {
		    glTexCoord2f(1 - (float) i / slices,
			    1 - (float) j / stacks);
		}
		glVertex3f(sintemp1 * sinCache1a[i], 
			sintemp1 * cosCache1a[i], costemp1);
	    }
	}
	glEnd();
	break;
      case GLU_LINE:
      case GLU_SILHOUETTE:
	for (j = 1; j < stacks; j++) {
	    sintemp1 = sinCache1b[j];
	    costemp1 = cosCache1b[j];
	    switch(qobj->normals) {
	      case GLU_FLAT:
	      case GLU_SMOOTH:
		sintemp2 = sinCache2b[j];
		costemp2 = cosCache2b[j];
		break;
	      default:
		break;
	    }

	    glBegin(GL_LINE_STRIP);
	    for (i = 0; i <= slices; i++) {
		switch(qobj->normals) {
		  case GLU_FLAT:
		    glNormal3f(sinCache3a[i] * sintemp2,
			    cosCache3a[i] * sintemp2,
			    costemp2);
		    break;
		  case GLU_SMOOTH:
		    glNormal3f(sinCache2a[i] * sintemp2,
			    cosCache2a[i] * sintemp2,
			    costemp2);
		    break;
		  case GLU_NONE:
		  default:
		    break;
		}
		if (qobj->textureCoords) {
		    glTexCoord2f(1 - (float) i / slices,
			    1 - (float) j / stacks);
		}
		glVertex3f(sintemp1 * sinCache1a[i], 
			sintemp1 * cosCache1a[i], costemp1);
	    }
	    glEnd();
	}
	for (i = 0; i < slices; i++) {
	    sintemp1 = sinCache1a[i];
	    costemp1 = cosCache1a[i];
	    switch(qobj->normals) {
	      case GLU_FLAT:
	      case GLU_SMOOTH:
		sintemp2 = sinCache2a[i];
		costemp2 = cosCache2a[i];
		break;
	      default:
		break;
	    }

	    glBegin(GL_LINE_STRIP);
	    for (j = 0; j <= stacks; j++) {
		switch(qobj->normals) {
		  case GLU_FLAT:
		    glNormal3f(sintemp2 * sinCache3b[j],
			    costemp2 * sinCache3b[j],
			    cosCache3b[j]);
		    break;
		  case GLU_SMOOTH:
		    glNormal3f(sintemp2 * sinCache2b[j],
			    costemp2 * sinCache2b[j],
			    cosCache2b[j]);
		    break;
		  case GLU_NONE:
		  default:
		    break;
		}

		if (qobj->textureCoords) {
		    glTexCoord2f(1 - (float) i / slices,
			    1 - (float) j / stacks);
		}
		glVertex3f(sintemp1 * sinCache1b[j], 
			costemp1 * sinCache1b[j], cosCache1b[j]);
	    }
	    glEnd();
	}
	break;
      default:
	break;
    }
}


void
glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
  QUAD_OBJ_INIT();
  gluQuadricDrawStyle(quadObj, GLU_FILL);
  gluQuadricNormals(quadObj, GLU_SMOOTH);
  /* If we ever changed/used the texture or orientation state
     of quadObj, we'd need to change it to the defaults here
     with gluQuadricTexture and/or gluQuadricOrientation. */
  gluSphere(quadObj, radius, slices, stacks);
}

static void
doughnut(GLfloat r, GLfloat R, GLint nsides,
  GLint rings, GLenum type)
{
  int i, j;
  GLfloat theta, phi, theta1, phi1;
  GLfloat p0[03], p1[3], p2[3], p3[3];
  GLfloat n0[3], n1[3], n2[3], n3[3];

  for (i = 0; i < rings; i++) {
    theta = (GLfloat) i *2.0 * M_PI / rings;
    theta1 = (GLfloat) (i + 1) * 2.0 * M_PI / rings;
    for (j = 0; j < nsides; j++) {
      phi = (GLfloat) j *2.0 * M_PI / nsides;
      phi1 = (GLfloat) (j + 1) * 2.0 * M_PI / nsides;

      p0[0] = cos(theta) * (R + r * cos(phi));
      p0[1] = -sin(theta) * (R + r * cos(phi));
      p0[2] = r * sin(phi);

      p1[0] = cos(theta1) * (R + r * cos(phi));
      p1[1] = -sin(theta1) * (R + r * cos(phi));
      p1[2] = r * sin(phi);

      p2[0] = cos(theta1) * (R + r * cos(phi1));
      p2[1] = -sin(theta1) * (R + r * cos(phi1));
      p2[2] = r * sin(phi1);

      p3[0] = cos(theta) * (R + r * cos(phi1));
      p3[1] = -sin(theta) * (R + r * cos(phi1));
      p3[2] = r * sin(phi1);

      n0[0] = cos(theta) * (cos(phi));
      n0[1] = -sin(theta) * (cos(phi));
      n0[2] = sin(phi);

      n1[0] = cos(theta1) * (cos(phi));
      n1[1] = -sin(theta1) * (cos(phi));
      n1[2] = sin(phi);

      n2[0] = cos(theta1) * (cos(phi1));
      n2[1] = -sin(theta1) * (cos(phi1));
      n2[2] = sin(phi1);

      n3[0] = cos(theta) * (cos(phi1));
      n3[1] = -sin(theta) * (cos(phi1));
      n3[2] = sin(phi1);

      glBegin(type);
      glNormal3fv(n3);
      glVertex3fv(p3);
      glNormal3fv(n2);
      glVertex3fv(p2);
      glNormal3fv(n1);
      glVertex3fv(p1);
      glNormal3fv(n0);
      glVertex3fv(p0);
      glEnd();
    }
  }
}

void
glutSolidTorus(GLdouble innerRadius, GLdouble outerRadius,
  GLint nsides, GLint rings)
{
  doughnut(innerRadius, outerRadius, nsides, rings, GL_QUADS);
}

/* ENDCENTRY */

void
glutSolidCone(GLdouble base, GLdouble height,
  GLint slices, GLint stacks)
{
  QUAD_OBJ_INIT();
  gluQuadricDrawStyle(quadObj, GLU_FILL);
  gluQuadricNormals(quadObj, GLU_SMOOTH);
  /* If we ever changed/used the texture or orientation state
     of quadObj, we'd need to change it to the defaults here
     with gluQuadricTexture and/or gluQuadricOrientation. */
  gluCylinder(quadObj, base, 0.0, height, slices, stacks);
}
