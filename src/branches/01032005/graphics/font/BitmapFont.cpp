#include <font/BitmapFont.h>
#include <mesh/Overlay.h>
#include <mesh/Entity.h>
#include <Fusion.h>
#include <fstream>

bool BitmapFont::Initialise(std::string fontfile)
{
	m_glyph_width	=	0;
	m_glyph_height	=	0;

	//=======================================
	// load the file, read it, grab the data
	//=======================================
	std::ifstream file(fontfile.c_str());

	std::string line;

// get the filename
	file >> line;
	m_filename = line.substr(0,line.find("=")+1);

// get the font width
	file >> line;
	line = line.substr(0,line.find("=")+1);
	m_glyph_width = atoi(line.c_str());

// get the font height
	file >> line;
	line = line.substr(0,line.find("=")+1);
	m_glyph_height = atoi(line.c_str());
	
	//=======================================
	//	Build the list of texture coordinates
	//	for the font to render from
	//=======================================
	m_characters = fusion->Mesh->CreateOverlay(fusion->Graphics->CreateTexture(m_filename));

	int num_keys_section[4] = {
		NUMBERNUMERICALKEYS,
		NUMBERUPPERCASELETTERS,
		NUMBERLOWERCASELETTERS,
		NUMBERSPECIALKEYS 
	};

	for(int a=0;a<4;a++) for(int b=0;b<num_keys_section[a];b++){
		m_characters->AddFrame(new Rect(b*m_glyph_width,a*m_glyph_height,(b+1)*m_glyph_width,(a+1)*m_glyph_height));
	}

	m_entity = fusion->Mesh->CreateEntity((Mesh *)m_characters);
	m_entity->SetActive(true);
	m_entity->SetScale(m_glyph_width,m_glyph_height,1);

	return true;
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

void BitmapFont::RenderString(float x, float y, float depth,std::string text, int len)
{
	for(int a=0;a<len;a++){
		m_characters->SetFrame(AsciiMapping[text[a]]);
		m_entity->SetTranslate(TRANSLATE_ABS,x+(a*m_glyph_width),y,depth);
		m_entity->Render();
	}
}
