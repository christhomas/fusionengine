#include <OpenGL/OGLShader.h>
#include <OpenGL/OpenGL12.h>
#include <OpenGL/OGLGraphics.h>
#include <Graphics/ITexture.h>
#include <fusion.h>

OGLShader::OGLShader()
{
	OGLGraphics *g = (OGLGraphics *)fusion->Graphics;

	glActiveTextureARB				= (PFNGLACTIVETEXTUREARBPROC)				g->GetExtension("glActiveTextureARB");
	glClientActiveTextureARB	=	(PFNGLCLIENTACTIVETEXTUREARBPROC)	g->GetExtension("glClientActiveTextureARB");
}

OGLShader::~OGLShader()
{
	m_layer.clear();
}

void OGLShader::AddLayer(unsigned int layer, float *tc, ITexture *t)
{
	if(layer < m_layer.size()){
		m_layer[layer].tc = tc;
		m_layer[layer].t	=	t;
	}else{
		TexLayer tl;

		tl.tc	=	tc;
		tl.t	=	t;

		m_layer.push_back(tl);
	}

	if(m_layer.size() > 0) 	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void OGLShader::RemoveLayer(unsigned int layer)
{
	if(layer < m_layer.size()){
		m_layer.erase(m_layer.begin() + layer);
	}

	if(m_layer.size() == 0) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

TexLayer * OGLShader::GetLayer(unsigned int layer)
{
	if(layer < m_layer.size()) return &m_layer[layer];

	return NULL;
}

void OGLShader::Set(void)
{
	for(int a=0;a<m_layer.size();a++){
		int texturelayer = GL_TEXTURE0_ARB+a;

		try{
			glActiveTextureARB(texturelayer);
			glClientActiveTextureARB(texturelayer);
		}catch(...){
			if(a > 0) continue;
		}

		TexLayer *tl = &m_layer[a];

		if(tl->t != NULL && tl->tc != NULL){
			tl->t->Set();

			glTexCoordPointer(2,GL_FLOAT,0,tl->tc);
		}
	}
}