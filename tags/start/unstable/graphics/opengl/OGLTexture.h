#ifndef _OGLTEXTURE_H_
	#define _OGLTEXTURE_H_

#include <graphics/ITexture.h>

class OGLTexture: public ITexture{
public:
				OGLTexture	(int width, int height, int numcomp, char *filename);
	virtual		~OGLTexture	();
	virtual void	SetFilename	(char *filename);
	virtual void	CreateTexture	(unsigned char *buffer);
	virtual void	DeleteTexture	(void);
	virtual void	Set			(void);
	virtual int		Reload		(void);
	virtual void	GetDimensions	(int &x, int &y);
	virtual int		GetWidth		(void);
	virtual int		GetHeight		(void);
};

#endif // #ifndef _OGLTEXTURE_H_
