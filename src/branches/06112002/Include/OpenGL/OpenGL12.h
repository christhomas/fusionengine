#ifndef _OPENGL12_H_
	#define _OPENGL12_H_

#ifdef _WIN32
	#include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>

#define	GL_BGR					0x80E0
#define	GL_BGRA					0x80E1

#define	GL_TEXTURE0_ARB	0x84C0
#define	GL_TEXTURE1_ARB	0x84C1

typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC)				(GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC)	(GLenum texture);

extern PFNGLACTIVETEXTUREARBPROC				glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB;

#endif // #ifndef _OPENGL12_H_