#include <gui/GUIComponents.h>
#include <Rect.h>

/*
 *	Button Window Component
 */

#define NORMAL		0
#define HIGHLIGHTED	1
#define CLICKED		2

Button::Button()
{
	m_component_type	= BUTTON;
	m_active			= true;
}

Button::~Button(){}//	TODO: do I have anything to do in here?

bool Button::Update(void)
{
	return true;
}

void Button::SetActive(bool active)
{
	m_active = active;
	m_entity->SetActive(m_active);
}

void Button::Initialise(WndComponentSetup *e, SceneGraph *scenegraph)
{
	ButtonSetup *bs		=	reinterpret_cast<ButtonSetup *>(e);
	m_click_func		=	bs->m_clickfunc;
	m_highlight_func	=	bs->m_highlightfunc;
	m_entity			=	bs->m_button;
	m_button			=	reinterpret_cast<Overlay *>(m_entity->GetMesh());

	scenegraph->AddEntity(m_entity);
}

bool Button::Click(void)
{
	if(m_click_func != NULL) if(m_click_func() == false) return false;

	m_button->SetFrame(CLICKED);
	return true;
}

bool Button::Highlight(int hx, int hy)
{
	if(m_highlight_func != NULL) if(m_highlight_func() == false) return false;

	Maths::Vector	*position	=	m_entity->GetPosition();
	Maths::Vector	*scale		=	m_entity->GetScale();
	Rect r((int)position->x,(int)position->y, (int)((scale->x*2)+position->x),(int)((scale->y*2)+position->y));

	if(r.Contains(hx,hy) == true){
		m_button->SetFrame(HIGHLIGHTED);
		return true;
	}

	m_button->SetFrame(NORMAL);
	return false;
}

void Button::SetPosition(float x, float y, float depth)
{
	m_entity->SetTranslate(TRANSLATE_ABS,x,y,depth);
}

void Button::SetScale(float x, float y)
{
	m_entity->SetScale(x,y,1);
}

