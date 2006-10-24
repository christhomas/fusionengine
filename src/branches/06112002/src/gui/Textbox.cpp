#include <gui/GUIComponents.h>
#include <Font/IFont.h>

Textbox::Textbox()
{
	m_component_type	=	TEXTBOX;
	m_caps						=	WNDCOMP_NOCAPS;
	m_string					=	NULL;
	m_entity					=	NULL;
	m_active					=	true;
	m_flash						=	true;
	m_flashms					=	-1;
	
	m_scale[0]				=	1;
	m_scale[1]				=	1;

	m_rgba[0]					=	1;
	m_rgba[1]					=	1;
	m_rgba[2]					=	1;
	m_rgba[3]					=	1;
}

Textbox::~Textbox()
{
	/*
		TODO:	Add all the necessary functionality to
				cleanup this object
	*/

	delete m_string;
}

void Textbox::SetCaps(int caps, union CapabilityData *cd)
{
	m_caps |= caps;

	if(m_caps & WNDCOMP_FLASHING){
		m_flash		=	true;
		m_flashms	=	cd->FlashingMilliseconds;
	}else if(m_caps & WNDCOMP_MARQUEE){
		cd->marquee.MarqueeMilliseconds;
		cd->marquee.speed;
	}
}

void Textbox::SetActive(bool active)
{
	m_active = active;
	
	if(m_entity != NULL)m_entity->SetActive(m_active);
	
	if(m_active == true){
		m_flashtimer.Start();
	}else{
		m_flashtimer.Stop();
	}
}

bool Textbox::Update(void)
{
	if(m_active == true){
		if(m_caps & WNDCOMP_FLASHING){
			m_flashtimer.Tick();

			unsigned int ms = m_flashtimer.GetMillisecondsAbs();
			if(ms > m_flashms){
				if(m_flash == true){
					m_flash = false;
				}else{
					m_flash = true; 
				}

				m_flashtimer.Reset();
			}

			if(m_flash == false) return true;
		}

		m_font->SetScale(m_scale[0],m_scale[1]);
		m_font->SetColour(m_rgba[0],m_rgba[1],m_rgba[2],m_rgba[3]);		
		m_font->RenderString(m_x,m_y,m_depth,m_string,strlen(m_string));
	}
	return true;
}

void Textbox::SetPosition(float x, float y, float depth)
{
	//	Repositions the textbox
	m_x		= x;
	m_y		= y;
	m_depth = depth;
}

void Textbox::SetScale(float x, float y)
{
	//	Set the scale of the box
	m_scale[0] = x;
	m_scale[1] = y;
}

void Textbox::SetColour(float r, float g, float b, float a)
{
	m_rgba[0]	=	r;
	m_rgba[1]	=	g;
	m_rgba[2]	=	b;
	m_rgba[3]	=	a;
}

void Textbox::Initialise(WndComponentSetup *e, SceneGraph *scenegraph)
{
	TextBoxSetup *tb	=	reinterpret_cast<TextBoxSetup *>(e);

	//	Update the string before setting any paramaters
	UpdateString(tb->m_text);

	m_font				=	tb->m_font;
	m_width				=	tb->m_width;
	m_x						=	tb->m_x;
	m_y						=	tb->m_y;
	m_depth				=	tb->m_depth;
	m_caps				=	tb->m_caps;

	//	Calculate the width of the textbox
	//	is this necessary?
	if(m_string != NULL){
		if(m_width & WNDCOMP_AUTOWIDTH){
			m_width = strlen(m_string);
		}else if(m_width > 0){
			m_width = tb->m_width;
		}else{

		}
	}
}

void Textbox::UpdateString(char *string)
{
	if(m_caps & WNDCOMP_CONTENTSLOCKED){
		//	The contents are locked, you cannot update the string
		return;
	}else if(m_caps & WNDCOMP_AUTOWIDTH){
		//	You are able to resize the 
		//	length of the box as you wish
	}

	if(string!=NULL){
		delete m_string;
		m_string = new char[strlen(string)+1];
		strcpy(m_string,string);
	}
}

char * Textbox::GetString(void)
{
	return m_string;	
}