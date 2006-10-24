#include <font/IFont.h>
#include <font/BitmapFont.h>
#include <font/TextureFont.h>
#include <Fusion.h>

FontDB::FontDB()
{
	object_name		= "FontDB Version 1.0 <chris_thomas@hotmail.com>";
	object_filename	= "libFontDB.dll";
}

FontDB::~FontDB()
{
	for(unsigned int a=0;a<m_fontlist.size();a++) delete m_fontlist[a];

	m_fontlist.clear();
}

bool FontDB::Initialise(void)
{
	return true;
}

IFont * FontDB::AddFont(std::string font, int font_type)
{
	IFont *fnt = NULL;

	switch(font_type){
		case BITMAPFONT:	{	fnt = new BitmapFont();		}break;
		case TEXTUREFONT:	{	fnt = new TextureFont();	}break;
	};

	if(fnt->Initialise(font) == false){
		delete fnt;
	}else{
		m_fontlist.push_back(fnt);
	}

	return fnt;
}

void FontDB::RemoveFont(IFont *font)
{
	for(unsigned int a=0;a<m_fontlist.size();a++){
		if(m_fontlist[a] == font){
			delete m_fontlist[a];
			m_fontlist.erase(m_fontlist.begin() + a);
		}
	}
}

void FontDB::RemoveFont(unsigned int font_id)
{
	if(font_id >= 0 && font_id < m_fontlist.size()){
		delete m_fontlist[font_id];
		m_fontlist.erase(m_fontlist.begin() + font_id);
	}
}

IFont * FontDB::operator[] (unsigned int index)
{
	IFont *f = NULL;

	if(index < m_fontlist.size()) f = m_fontlist[index];

	return f;
}
