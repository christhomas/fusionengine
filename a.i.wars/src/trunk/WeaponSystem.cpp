#include "WeaponSystem.h"
#include "aiwars.h"

WeaponSystem::WeaponSystem()
{

}

WeaponSystem::~WeaponSystem()
{

}

void WeaponSystem::Initialise(char *texture)
{
	int width,height;

	ITexture *t = fusion->Graphics->CreateTexture(texture);
	t->GetDimensions(width,height);

	Overlay *o = fusion->Mesh->CreateOverlay(t);
	o->AddFrame(new Rect(0,0,width,height));

	m_weapon = fusion->Mesh->CreateEntity(o);
	m_weapon->SetScale(8,8,1);
	m_weapon->SetAxis(4,4,1);
	m_weapon->SetActive(false);

	GameScene->CollisionSystem->AddEntity(1,m_weapon);
}

Entity * WeaponSystem::GetEntity(void)
{
	return m_weapon;
}

bool WeaponSystem::WeaponAvailable(int type)
{
	if(m_weapon->GetActive() == false){
		return true;
	}

	return false;
}

bool WeaponSystem::Fire(int type,int power, Maths::Vector &pos, Maths::Vector &vector)
{
	if(WeaponAvailable(type) == true){
		m_weapon->SetTranslate(TRANSLATE_ABS,pos);
		m_weapon->SetActive(true);
		m_vector = vector*(power*8);
		return true;
	}

	return false;
}

void WeaponSystem::Update(void)
{
	if(m_weapon->GetActive() == true){
		m_weapon->SetTranslate(TRANSLATE_REL,m_vector);

		Maths::Vector *pos = m_weapon->GetPosition();

		if(pos->x < 0 || pos->x > SCREENWIDTH)	m_weapon->SetActive(false);
		if(pos->y < 0 || pos->y > SCREENHEIGHT)	m_weapon->SetActive(false);
	}
}