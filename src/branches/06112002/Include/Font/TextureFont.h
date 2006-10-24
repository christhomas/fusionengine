/*	ORIGINAL COPYRIGHT 
	Copyright (c) Mark J. Kilgard, 1997. 

	This program is freely distributable without licensing fees  and is
	provided without guarantee or warrantee expressed or  implied. This
	program is -not- in the public domain.
*/

/*	RELEASE NOTES
	This program was rewritten by Chris Thomas<chris_thomas@hotmail.com> 2001

	**********************VERY IMPORTANT LICENCE INFORMATION*****************************
	The original code was also mangled to become part of an object TextureFont, derived
	from IFont which is the base of my font library code.  This code now becomes part of that
	but since I can't GPL it without permission, the following files are exempt from 
	the GPL licence applied to this library

	TextureFont.cpp
	TextureFont.h

	These files contain the original source code from Mark Kilgard, 
	I dont have his permission to alter his licence to be compatible,
	hence this rather long and tedious header
	**********************VERY IMPORTANT LICENCE INFORMATION*****************************

	It was rewritten to take advantage of the new developments of OpenGL

	1.	Texture objects are now the only method of storing the textures, since they are now
		practially standard. Any hardware that doesnt support them, I neither care about
		now give two shits if you can't run this code because of this decision :) hehe (joking)
		BUY NEW HARDWARE

	2.	Vertex arrays are now the method of rendering the quads, they are much faster and I was 
		able to integrate the vertex arrays nicely into the fusion engine this file should accompany.

	I've removed practically all the code I found to be useless, at least on this platform 
	(win32/nvidia gf2mx), however, if you	find that something quite reasonable has not been 
	done to support you, can you contact me and I'll help or code up that relevant support, I 
	dont have a problem with this, it's not exactly rocket science, so get in touch and I'll 
	see what I can do about supporting your hardware

*/

#ifndef _TEXTUREFONT_H_
	#define _TEXTUREFONT_H_

#include <Font/IFont.h>
#include <Mesh/Vertex.h>
#include <Mesh/Colour.h>

//	Predeclared classes (prevents circular #include'ing)
class	ITexture;
class	IVertexBuffer;

#define TXF_FORMAT_BYTE		0
#define TXF_FORMAT_BITMAP	1

struct FontData{
  unsigned short	c;			//	Potentially support 16-bit glyphs.
  unsigned char		width;
  unsigned char		height;
  signed char			xoffset;
  signed char			yoffset;
  signed char			advance;
  char						dummy;		//	Space holder for alignment reasons.
  short						x;
  short						y;
};

struct VertexData{
	Vertex2f	texture[4];
	Vertex2f	vertex[4];
	float			advance;
};

class TextureFont: public IFont{
protected:
	FontData			*m_font_data;
	VertexData		*m_vertex_data;
	VertexData		**m_lut;
	ITexture			*m_texture;
	IVertexBuffer	*m_vertexbuffer;

	int						m_tex_width, m_tex_height;
	int						m_max_ascent,m_max_descent;
	int						m_num_glyphs,m_min_glyph,m_range;
	unsigned char	*m_teximage;
	Colour4f			m_rgba;
	float					m_scale[2];
	Vertex2f			m_normal[4];
	int						m_index[6];

	virtual VertexData *	GetGlyph				(int c);
public:
												TextureFont			();
	virtual								~TextureFont		();
	virtual bool					Initialise			(char *filename,Fusion *f);
	virtual char *				ErrorString			(void);
	virtual int						GetGlyphWidth		(void);
	virtual int						GetGlyphHeight	(void);
	virtual void					SetScale				(float x, float y);
	virtual void					SetColour				(float r, float g, float b, float a);
	virtual void					RenderGlyph			(int c);
	virtual void					RenderString		(float x, float y, float depth, char *string,int len);
};

#endif // #ifndef _TEXTUREFONT_H_