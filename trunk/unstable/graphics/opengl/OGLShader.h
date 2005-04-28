#ifndef _OGLSHADER_H_
	#define _OGLSHADER_H_

#include <graphics/IShader.h>

class OGLShader: public IShader{
public:
						OGLShader	();
	virtual				~OGLShader	();
	virtual void		AddLayer	(unsigned int layer, float *tc, ITexture *t);
	virtual void		RemoveLayer	(unsigned int layer);
	virtual TexLayer *	GetLayer	(unsigned int layer);
	virtual void		Set			(void);
};

#endif // #ifndef _OGLSHADER_H_

