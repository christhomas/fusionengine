#ifndef _OPENGL12_H_
	#define _OPENGL12_H_

//	Solving the winsock2 madness, fighting together! for a better future!
#include <PlatformData.h>

#include <gl/gl.h>
#include <gl/glu.h>

//	Downloaded from oss.sgi.com
#include <glext.h>

#define GL_BGR			0x80E0
#define GL_BGRA			0x80E1

#define	GL_TEXTURE0_ARB	0x84C0
#define	GL_TEXTURE1_ARB	0x84C1

extern PFNGLACTIVETEXTUREARBPROC		glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTexture;

#endif // #ifndef _OPENGL12_H_