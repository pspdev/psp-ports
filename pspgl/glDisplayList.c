// functions dealing with display lists

#include "pspgl_internal.h"

//GLint __pspgl_is_in_glNewList = 0; // equal 1 if we are in glNewList / glEndList
//GLint __pspgl_nblists = 0; // number of lists
//GLuint __pspgl_calllists_base = 0; // display-list base for glCallLists

struct stDisplayLists *__pspgl_displaylists; // where Display lists will be stored
struct stDisplayLists *__pspgl_actuallist;	// display list opened with glNewList

// functions' array
void __pspglBegin(struct stDisplayElement *de) {
	glBegin(de->parami1);
}
void __pspglEnd(struct stDisplayElement *de) {
	glEnd();
}
void __pspglTexCoord(struct stDisplayElement *de) {
	glTexCoord4f(de->paramf1, de->paramf2, de->paramf3, de->paramf4);
}
void __pspglVertex(struct stDisplayElement *de){
	glVertex3f(de->paramf1, de->paramf2, de->paramf3);
}

void __pspglColor(struct stDisplayElement *de){
	__pspgl_update_color(de->parami1);
}

void __pspglEnable(struct stDisplayElement *de){
	__pspgl_enable_state (de->parami1, de->parami2);
}

void __pspglTranslate(struct stDisplayElement *de){
	glTranslatef (de->paramf1, de->paramf2, de->paramf3);
}

void (*__pspglFunctionToCall[])(struct stDisplayElement *de) = { 
	__pspglBegin, __pspglEnd, __pspglTexCoord, __pspglVertex, __pspglColor,
	__pspglEnable, __pspglTranslate
};

// external functions for display list
GLAPI GLboolean GLAPIENTRY glIsList( GLuint list )
{
	if (list < pspgl_curctx->displaylists.nblists)
		return __pspgl_displaylists[list].is_used;
	else 
		return GL_FALSE;
}

GLAPI void GLAPIENTRY glDeleteLists( GLuint list, GLsizei range )
{
	int i;
	struct stDisplayElement *curr, *temp;
	
	for(i=list; i < (list+range); i++) {
		curr = __pspgl_displaylists[i].first;
		while (curr){
			temp = curr;
			curr = curr->next;
			free(temp);
		}
		__pspgl_displaylists[i].is_used = GL_FALSE;
		__pspgl_displaylists[i].first = NULL;
		__pspgl_displaylists[i].last = NULL;
	}
}

GLAPI GLuint GLAPIENTRY glGenLists( GLsizei range )
{
	int lastnblists;
	int i;

	lastnblists = pspgl_curctx->displaylists.nblists;
	pspgl_curctx->displaylists.nblists += range;

	if (lastnblists) // if display lists are existing
		__pspgl_displaylists = realloc (__pspgl_displaylists, pspgl_curctx->displaylists.nblists * sizeof(struct stDisplayLists) );
	else
		__pspgl_displaylists = malloc (sizeof(struct stDisplayLists) * pspgl_curctx->displaylists.nblists);

	// init new lists
	for (i=lastnblists; i< pspgl_curctx->displaylists.nblists; i++){
		__pspgl_displaylists[i].is_used = GL_FALSE;
		__pspgl_displaylists[i].first = NULL;
		__pspgl_displaylists[i].last = NULL;
	}

	return lastnblists;
}

GLAPI void GLAPIENTRY glNewList( GLuint list, GLenum mode )
{
	if (list < pspgl_curctx->displaylists.nblists){ // verif if "list" is in displaylists range...
		if (__pspgl_displaylists[list].is_used != GL_FALSE) // use it only if free !
			return;
	}
	else { // ...else create it
		glGenLists(1);
	}
	
	__pspgl_actuallist = &__pspgl_displaylists[list];
	__pspgl_actuallist->is_used = GL_TRUE;
	pspgl_curctx->displaylists.is_in_glNewList = mode;
	
	pspgl_curctx->displaylists.index = list;
}

GLAPI void GLAPIENTRY glEndList( void )
{
	pspgl_curctx->displaylists.is_in_glNewList = 0;
	__pspgl_actuallist = NULL;
	
	pspgl_curctx->displaylists.index = 0;
}

GLAPI void GLAPIENTRY glCallList( GLuint list )
{
	struct stDisplayElement *curr;
	
	curr = __pspgl_displaylists[list].first;
	while (curr){
		(__pspglFunctionToCall[curr->command_num])(curr);
		curr = curr->next;
	}
}

/*
 * Translate the nth element of list from type to GLuint.
 * used by glCallLists
 */
static GLuint __pspglTranslate_id( GLsizei n, GLenum type, const GLvoid *list )
{
   GLbyte *bptr;
   GLubyte *ubptr;
   GLshort *sptr;
   GLushort *usptr;
   GLint *iptr;
   GLuint *uiptr;
   GLfloat *fptr;

   switch (type) {
      case GL_BYTE:
         bptr = (GLbyte *) list;
         return (GLuint) *(bptr+n);
      case GL_UNSIGNED_BYTE:
         ubptr = (GLubyte *) list;
         return (GLuint) *(ubptr+n);
      case GL_SHORT:
         sptr = (GLshort *) list;
         return (GLuint) *(sptr+n);
      case GL_UNSIGNED_SHORT:
         usptr = (GLushort *) list;
         return (GLuint) *(usptr+n);
      case GL_INT:
         iptr = (GLint *) list;
         return (GLuint) *(iptr+n);
      case GL_UNSIGNED_INT:
         uiptr = (GLuint *) list;
         return (GLuint) *(uiptr+n);
      case GL_FLOAT:
         fptr = (GLfloat *) list;
         return (GLuint) *(fptr+n);
      case GL_2_BYTES:
         ubptr = ((GLubyte *) list) + 2*n;
         return (GLuint) *ubptr * 256 + (GLuint) *(ubptr+1);
      case GL_3_BYTES:
         ubptr = ((GLubyte *) list) + 3*n;
         return (GLuint) *ubptr * 65536
              + (GLuint) *(ubptr+1) * 256
              + (GLuint) *(ubptr+2);
      case GL_4_BYTES:
         ubptr = ((GLubyte *) list) + 4*n;
         return (GLuint) *ubptr * 16777216
              + (GLuint) *(ubptr+1) * 65536
              + (GLuint) *(ubptr+2) * 256
              + (GLuint) *(ubptr+3);
      default:
         return 0;
   }
}

GLAPI void GLAPIENTRY glCallLists( GLsizei n, GLenum type,
                                     const GLvoid *lists )
{
   GLint i;

   switch (type) {
      case GL_BYTE:
      case GL_UNSIGNED_BYTE:
      case GL_SHORT:
      case GL_UNSIGNED_SHORT:
      case GL_INT:
      case GL_UNSIGNED_INT:
      case GL_FLOAT:
      case GL_2_BYTES:
      case GL_3_BYTES:
      case GL_4_BYTES:
         break;
      default:
         type = GL_UNSIGNED_BYTE;
   }

   for (i=0;i<n;i++) {
		// transform array element in GLuint
      GLuint list = __pspglTranslate_id( i, type, lists );
	  list += pspgl_curctx->displaylists.calllists_base; // display-list base for glCallLists
	  
	  // Call the right list
	  glCallList(list);
   }
}

GLAPI void GLAPIENTRY glListBase( GLuint base )
{
	pspgl_curctx->displaylists.calllists_base = base; // display-list base for glCallLists
}


