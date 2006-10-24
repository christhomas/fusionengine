#include <gui/GUIComponents.h>

ProgressBar::ProgressBar()
{
	m_component_type	= PROGRESSBAR;
	m_active			= true;
	m_Finalise		= false;
}

ProgressBar::~ProgressBar(){}

void ProgressBar::Initialise(WndComponentSetup *e, SceneGraph *scenegraph)
{
	ProgressBarSetup *p	=	reinterpret_cast<ProgressBarSetup *>(e);
	m_entity				=	p->m_entity;
	m_width				=	p->m_width;

	
	scenegraph->AddEntity(m_entity);
}

void ProgressBar::SetCaps(int caps, union CapabilityData *pd){}

void ProgressBar::SetActive(bool active)
{
	m_active = active;
	m_entity->SetActive(m_active);
}

bool ProgressBar::Update(void)
{
	static float old_pc = 0;
	float percentage = 0;

	if(m_active == true && m_Finalise==true){
		for(unsigned int a=0;a<m_meter_list.size();a++)	percentage+=*m_meter_list[a];

		Overlay *overlay = reinterpret_cast<Overlay *>(m_entity->GetMesh());

		float stretch = (percentage-old_pc)*m_onepercent;
		overlay->Stretch(STRETCH_RIGHT,stretch);
		old_pc = percentage;
	}
	return true;
}

bool ProgressBar::Highlight(int hx, int hy)	{	return false;	}
bool ProgressBar::Click(void)			{	return false;	}

void ProgressBar::AddPercentageMeter(int *meter)
{
	if((m_Finalise == false) && (meter!=NULL))	m_meter_list.push_back(meter);
}

void ProgressBar::Finalise(void)
{
	Maths::Vector *position = m_entity->GetPosition();

	if(m_meter_list.size() > 0){
		m_onepercent = ((m_width - position->x)/m_meter_list.size()) / 100;
	}
	m_Finalise = true;
}

void ProgressBar::SetPosition(float x, float y, float depth)
{
	m_entity->SetTranslate(TRANSLATE_ABS,x,y,depth);
}

void ProgressBar::SetScale(float x, float y)
{
	m_entity->SetScale(x,y,1);
}
