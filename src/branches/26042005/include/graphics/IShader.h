#ifndef _ISHADER_H_
	#define _ISHADER_H_

#include <vector>

class ITexture;

struct TexLayer{
	float		*tc;
	ITexture	*t;
};

class IShader{
protected:
	std::vector<TexLayer> m_layer;

public:
					IShader		(){};
	virtual			~IShader		(){};
	virtual void		AddLayer		(unsigned int layer, float *tc, ITexture *t)	=	0;
	virtual void		RemoveLayer	(unsigned int layer)					=	0;
	virtual TexLayer *	GetLayer		(unsigned int layer)					=	0;
	virtual void		Set			(void)							= 0;
};

#endif // #ifndef _ISHADER_H_

