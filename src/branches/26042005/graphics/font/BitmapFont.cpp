#include <font/BitmapFont.h>
#include <mesh/Overlay.h>
#include <mesh/Entity.h>
#include <Fusion.h>
#include <fstream>

bool BitmapFont::Initialise(char *fontfile,Fusion *f)
{
	m_glyph_width		=	0;
	m_glyph_height	=	0;
	m_fusion			=	f;

	//=======================================
	// load the file, read it, grab the data
	//=======================================
	std::ifstream fontresource_file(fontfile);

	char line[2048];

// get the filename
	fontresource_file.getline(line,2048);
	m_filename = strdup((char *)&(strchr(line,'='))[1]);

// get the font width
	fontresource_file.getline(line,2048);
	m_glyph_width = atoi(strdup((char *)&(strchr(line,'='))[1]));

// get the font height
	fontresource_file.getline(line,2048);
	m_glyph_height = atoi(strdup((char *)&(strchr(line,'='))[1]));
	
	//=======================================
	//	Build the list of texture coordinates
	//	for the font to render from
	//=======================================
	if(m_fusion != NULL){
		m_characters	=	m_fusion->Mesh->CreateOverlay(m_fusion->Graphics->CreateTexture(m_filename));

		int num_keys_section[4] =	{	NUMBERNUMERICALKEYS,NUMBERUPPERCASELETTERS,NUMBERLOWERCASELETTERS,NUMBERSPECIALKEYS };

		for(int a=0;a<4;a++) for(int b=0;b<num_keys_section[a];b++){
			m_characters->AddFrame(new Rect(b*m_glyph_width,a*m_glyph_height,(b+1)*m_glyph_width,(a+1)*m_glyph_height));
		}

		m_entity = m_fusion->Mesh->CreateEntity((Mesh *)m_characters);
		m_entity->SetActive(true);
		m_entity->SetScale(m_glyph_width,m_glyph_height,1);

		return true;
	}

	return false;
}

int BitmapFont::GetGlyphWidth(void)
{
	return m_glyph_width;
}

int BitmapFont::GetGlyphHeight(void)
{
	return m_glyph_height;
}

void BitmapFont::SetScale(float x, float y)
{
	m_entity->SetScale(x,y,1);
}

void BitmapFont::SetColour(float r, float g, float b, float a)
{
	m_entity->SetColour(a,r,g,b);
}

void BitmapFont::RenderString(float x, float y, float depth,char *text, int len)
{
	for(int a=0;a<len;a++){
		m_characters->SetFrame(AsciiMapping[text[a]]);
		m_entity->SetTranslate(TRANSLATE_ABS,x+(a*m_glyph_width),y,depth);
		m_entity->Render();
	}
}
