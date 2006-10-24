#ifndef _IFONT_H_
	#define _IFONT_H_

#include <FusionSubsystem.h>
#include <vector>

// supported font types
#define BITMAPFONT		0	//	Unscaled, unmipmapped bitmap fonts
#define TEXTUREFONT		1	//	Uses OpenGL + Mark Kilgards Texture Fonts

class IFont{
protected:
	char	*m_filename;
	int		m_glyph_width,m_glyph_height;
public:
								IFont						(){}
	virtual				~IFont					(){}
	virtual bool	Initialise			(char *filename,Fusion *f)														=	0;
	virtual int		GetGlyphWidth		(void)																								=	0;
	virtual int		GetGlyphHeight	(void)																								=	0;
	virtual void	SetScale				(float x, float y)																		=	0;
	virtual void	SetColour				(float r, float g, float b, float a)									=	0;
	virtual void	RenderString		(float x, float y, float depth, char *text, int len)	=	0;
};

class FontDB: public FusionSubsystem{
protected:
	std::vector<IFont *>	m_fontlist;
public:
									FontDB			();
	virtual					~FontDB			();
	virtual bool		Initialise	(void);
	virtual IFont	*	AddFont			(char *font, int font_type);
	virtual void		RemoveFont	(IFont *font);
	virtual void		RemoveFont	(int font_id);
	virtual IFont *	operator[]	(unsigned int index);
};

#endif // #ifndef _IFONT_H_
