/*	ORIGINAL COPYRIGHT 
	Copyright (c) Mark J. Kilgard, 1997. 

	This program is freely distributable without licensing fees  and is
	provided without guarantee or warrantee expressed or  implied. This
	program is -not- in the public domain.
*/

/*	RELEASE NOTES
	This code was rewritten by Chris Thomas<chris_thomas@hotmail.com> 2001

	**********************VERY IMPORTANT RELEASE NOTES INFORMATION*****************************
	The original code was also mangled to become part of an object TextureFont, derived
	from IFont which is the base of my font library code.  This code now becomes part of that
	but since I can't GPL it without permission, the following files are exempt from 
	the GPL licence applied to this library

	TextureFont.cpp
	TextureFont.h

	These files contain the original source code from Mark Kilgard, 
	I dont have his permission to alter his licence to be compatible,
	hence this rather long and tedious header
	**********************VERY IMPORTANT RELEASE NOTES INFORMATION*****************************

	It was rewritten to take advantage of the new developments of OpenGL

	1.	Texture objects are now the only method of storing the textures, since they are now
		practially standard. Any hardware that doesnt support them, I neither care about
		nor give two shits about if you can't run this code because of this decision :) hehe (joking)
		BUY NEW HARDWARE

	2.	Vertex arrays are now the method of rendering the quads, they are much faster and I will be 
		able to integrate the vertex arrays nicely into the fusion engine this file should accompany.

	I've removed practically all the code I found to be useless, at least on this platform 
	(win32/nvidia gf2mx), however, if you	find that something quite reasonable has not been 
	done to support you, can you contact me and I'll help or code up that relevant support, I 
	dont have a problem with this, it's not exactly rocket science, so get in touch and I'll 
	see what I can do about supporting your hardware

*/

#include <font/TextureFont.h>
#include <fstream>
#include <Fusion.h>

static char		*lastError;
static int			format;
static std::ifstream	file;
static char		*font_filename;
static unsigned int	texture_offset;

/*
 *	The procedure that will create the font texture which each glyph
 *	will reside
 */
unsigned char * LoadTextureFont(unsigned char *buffer, int width, int height, int numcomp)
{
	int i,j,stride;
	unsigned char *bitmap,*tmp_src,*tmp_dst;
	
	file.open(font_filename,std::ios::binary);
	file.seekg(texture_offset,std::ios::beg);

	switch (format) {
		case TXF_FORMAT_BYTE:
			bitmap	=	new unsigned char[width * height];

			memset(bitmap,0,width*height);

			file.read((char *)bitmap,width*height);

			for (i = 0;i<height;i++){
				tmp_src		=	&bitmap[i * width];
				tmp_dst		=	&buffer[i * width * numcomp];
			
				for (j = 0;j<width;j++){
					if (tmp_src[j >> 3] & (1 << (j & 7))){
						for(int a=0;a<numcomp;a++)	tmp_dst[(j*numcomp)+a] = 255;
					}
				}
			}
			delete bitmap;
		break;

		case TXF_FORMAT_BITMAP:
			stride	=	(width + 7) >> 3;
			bitmap	=	new unsigned char[stride * height];
			
			memset(bitmap,0,stride*height);

			file.read((char *)bitmap,stride*height);

			for (i = 0;i<height;i++){
				tmp_src		=	&bitmap[i * stride];
				tmp_dst		=	&buffer[i * width * numcomp];
			
				for (j = 0;j<width;j++){
					if (tmp_src[j >> 3] & (1 << (j & 7))){
						for(int a=0;a<numcomp;a++)	tmp_dst[(j*numcomp)+a] = 255;
					}
				}
			}

			delete bitmap;
		break;
	}

	return buffer;
}

/*
 *	To support platforms whose endian are different
 */
inline void ReverseByteOrder(char *bp, int size, int count)
{
	int a,b,c,baseoffset;
	char temp;

	for(a=0,baseoffset=0;a<count;a++,baseoffset=a*size)	for(b=0,c=size-1;c>=size/2;b++,c--)
	{
		temp				= bp[baseoffset+b];
		bp[baseoffset+b]	= bp[baseoffset+c];
		bp[baseoffset+c]	= temp;
	}
}

char * TextureFont::ErrorString(void){  return lastError;	}

TextureFont::TextureFont()
{
	/* For easy cleanup in error case. */
	m_font_data		=	NULL;
	m_vertex_data	=	NULL;
	m_lut			=	NULL;
	m_teximage		=	NULL;
	m_glyph_width	=	0;
	m_glyph_height	=	0;
	
	//	NOTE:	Every character rendered will have the same 
	//			normals/index, so just create one pair and 
	//			use on each character rendered

	//	Set all the normals
	m_normal[0].x = 0;	m_normal[0].y = 1;
	m_normal[1].x = 0;	m_normal[1].y = 1;
	m_normal[2].x = 0;	m_normal[2].y = 1;
	m_normal[3].x = 0;	m_normal[3].y = 1;
	
	//	Set all the Indices
	m_index[0]	=	0;	m_index[1]	=	1;	m_index[2]	=	2;
	m_index[3]	=	0;	m_index[4]	=	2;	m_index[5]	=	3;

	//	Create + assign all vertexpool data
	m_vertexbuffer	=	fusion->Graphics->CreateVertexBuffer(IVertexBuffer::DYNAMIC);
	m_vertexbuffer->Initialise(4,6,2,2);
	m_vertexbuffer->SetNormal((float *)m_normal);
	m_vertexbuffer->SetIndex(m_index);
	
	//	NOTE:	Dont assign the colour, since you might 
	//			want multicoloured text, or one line of 
	//			text in red, another in blue, so you'll 
	//			want this set PER sentence, rather than per font

	//	NOTE:	Dont assign the texture until it's been created
}

TextureFont::~TextureFont()
{
	delete m_font_data;
	delete m_vertex_data;
	delete m_lut;
	delete m_teximage;
	delete m_vertexbuffer;

	fusion->Graphics->DeleteTexture(m_texture);

	file.close();
}

bool TextureFont::Initialise(char *filename,Fusion *fusion)
{
	float w, h, xstep, ystep;
	char fileid[4];
	int max_glyph;
	int endianness, swap,i;

	//	Store the filename in the global variable and the class,
	//	for the procedural function to read and if the font needs to be re-read
	font_filename = m_filename = filename;

	file.open(m_filename,std::ios::binary);
	file.read((char *)&fileid,4);

	if(strncmp(fileid,"\377txf",4) != 0){
		lastError = "Not a texture font file.";
		file.close();
		return false;
	}

	file.read((char *)&endianness,sizeof(endianness));
	if(endianness = 0x12345678){
		swap = 0;
	}else if(endianness == 0x78563412){
		swap = 1;
	}else{
		lastError = "not a texture font file.";
		file.close();
		return false;
	}

	file.read((char *)&format,sizeof(format));
	file.read((char *)&m_tex_width,		sizeof(m_tex_width));
	file.read((char *)&m_tex_height,	sizeof(m_tex_height));
	file.read((char *)&m_max_ascent,	sizeof(m_max_ascent));
	file.read((char *)&m_max_descent,	sizeof(m_max_descent));
	file.read((char *)&m_num_glyphs,	sizeof(m_num_glyphs));

	if (swap) {
		ReverseByteOrder((char *)&format,		sizeof(format),			1);
		ReverseByteOrder((char *)&m_tex_width,	sizeof(m_tex_width),	1);
		ReverseByteOrder((char *)&m_tex_height,	sizeof(m_tex_height),	1);
		ReverseByteOrder((char *)&m_max_ascent,	sizeof(m_max_ascent),	1);
		ReverseByteOrder((char *)&m_max_descent,sizeof(m_max_descent),	1);
		ReverseByteOrder((char *)&m_num_glyphs,	sizeof(m_num_glyphs),	1);
	}

	m_font_data = new FontData[m_num_glyphs];

	file.read((char *)m_font_data,sizeof(FontData)*m_num_glyphs);

	if(swap){
		for(i = 0; i < m_num_glyphs; i++){
			ReverseByteOrder((char *)&m_font_data[i].c,sizeof(m_font_data[i].c),1);
			ReverseByteOrder((char *)&m_font_data[i].x,sizeof(m_font_data[i].x),1);
			ReverseByteOrder((char *)&m_font_data[i].y,sizeof(m_font_data[i].y),1);
		}
	}
	m_vertex_data = new VertexData[m_num_glyphs];

	w = m_tex_width;
	h = m_tex_height;
	xstep = 0.5 / w;
	ystep = 0.5 / h;
	for (i = 0; i < m_num_glyphs; i++) {
		FontData *tgi = &m_font_data[i];

		m_vertex_data[i].vertex[0].x	=	tgi->xoffset;
		m_vertex_data[i].vertex[0].y	=	-(tgi->height + tgi->yoffset);
		m_vertex_data[i].texture[0].x	=	tgi->x / w + xstep;
		m_vertex_data[i].texture[0].y	=	(tgi->y + tgi->height) / h + ystep;
		
		m_vertex_data[i].vertex[1].x	=	tgi->xoffset + tgi->width;
		m_vertex_data[i].vertex[1].y	=	-(tgi->height + tgi->yoffset);
		m_vertex_data[i].texture[1].x	=	(tgi->x + tgi->width) / w + xstep;
		m_vertex_data[i].texture[1].y	=	(tgi->y + tgi->height) / h + ystep;
		
		m_vertex_data[i].vertex[2].x	=	tgi->xoffset + tgi->width;
		m_vertex_data[i].vertex[2].y	=	-tgi->yoffset;
		m_vertex_data[i].texture[2].x	=	(tgi->x + tgi->width) / w + xstep;
		m_vertex_data[i].texture[2].y	=	tgi->y / h + ystep;

		m_vertex_data[i].vertex[3].x	=	tgi->xoffset;
		m_vertex_data[i].vertex[3].y	=	-tgi->yoffset;
		m_vertex_data[i].texture[3].x	=	tgi->x / w + xstep;
		m_vertex_data[i].texture[3].y	=	tgi->y / h + ystep;

		m_vertex_data[i].advance = tgi->advance;
	}

	m_min_glyph = m_font_data[0].c;
	max_glyph = m_font_data[0].c;
	for (i = 1; i < m_num_glyphs; i++) {
		if (m_font_data[i].c < m_min_glyph)	m_min_glyph = m_font_data[i].c;
		if (m_font_data[i].c > max_glyph)	max_glyph = m_font_data[i].c;
	}
	m_min_glyph = m_min_glyph;
	m_range = max_glyph - m_min_glyph + 1;

	m_lut = new VertexData *[m_range];

	for (i = 0; i < m_num_glyphs; i++) m_lut[m_font_data[i].c - m_min_glyph] = &m_vertex_data[i];

	texture_offset = file.tellg();

	file.close();

	m_texture = fusion->Graphics->CreateTexture(m_tex_width,m_tex_height,3,LoadTextureFont);

	return true;
}

void TextureFont::SetScale(float x, float y)
{
	m_scale[0]	=	x;
	m_scale[1]	=	y;
}

void TextureFont::SetColour(float r, float g, float b, float a)
{
	m_rgba.r	=	r;
	m_rgba.g	=	g;
	m_rgba.b	=	b;
	m_rgba.a	=	a;
}

VertexData * TextureFont::GetGlyph(int c)
{
	VertexData *gd;

	// Automatically substitute uppercase letters with lowercase if not	uppercase available (and vice versa).
	if ((c >= m_min_glyph) && (c < m_min_glyph + m_range)) {
		gd = m_lut[c - m_min_glyph];
		
		if(gd)	return gd;

		if(islower(c))	c = toupper(c);
		if(isupper(c))	c = tolower(c);
		
		if((c >= m_min_glyph) && (c < m_min_glyph + m_range))	return m_lut[c - m_min_glyph];
	}
	
	return NULL;
}

int TextureFont::GetGlyphWidth(void)
{
	return m_glyph_width;
}

int TextureFont::GetGlyphHeight(void)
{
	return m_glyph_height;
}

void TextureFont::RenderGlyph(int c)
{
	VertexData *vd;

	vd = GetGlyph(c);

	if(vd!=NULL){
		m_vertexbuffer->SetPosition((float *)vd->vertex);
		//m_vertexbuffer->SetTexcoord((float *)vd->texture);
		m_vertexbuffer->SetTextureLayer(0,(float *)vd->texture,m_texture);
		m_vertexbuffer->Render();

		fusion->Graphics->Translate(vd->advance*1.1f,0,0);
	}
}

void TextureFont::RenderString(float x, float y,float depth, char *string,int len)
{
	//	Reset the current rendering position
	fusion->Graphics->LoadIdentity();
	
	//	Translate to a new rendering position
	fusion->Graphics->Translate(x,y,depth);
	
	//	Set the scale of the text
	fusion->Graphics->Scale(m_scale[0],m_scale[1],1);
	
	//	Set the colour of the text
	m_vertexbuffer->SetColour(&m_rgba);
	
	//	Render each glyph in the sentence
	for (int i = 0; i < len; i++)	RenderGlyph(string[i]);
}
