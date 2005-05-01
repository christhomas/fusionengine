#include <Zargadia.h>

UserInterface	*TitleUI;								//	UserInterfaces
SceneGraph		*TitleScene;						//	Scenes for the Title and Game
Entity				*TitlescreenEntity[7];	//	List of entities on the titlescreen

void BuildTitleUI(void)
{
	TitleScene	=	fusion->Scene->AddScene();
	TitleUI		=	fusion->Interface->AddUI(TitleScene);

	Window *w	=	TitleUI->AddWindow(fusion->Input);
	w->SetTitle("Title");

	ITexture	*tb;
	Overlay		*o;
	Entity		*e;

//=====================================
//	Load the texture startbutton.tga, 
//	add an overlay, create an entity 
//	from that overlay
//=====================================
	tb	=	fusion->Graphics->CreateTexture("file://sprites/startbutton.tga");
	o	=	fusion->Mesh->CreateOverlay(tb);

	o->AddFrame(new Rect(0,0,128,32));
	o->AddFrame(new Rect(0,32,128,64));
	o->AddFrame(new Rect(0,0,128,32));
	
//	Create an entity for the button, scale it and add a Mesh animation for it
//	Then create a Button component and add it to the window
	e	=	fusion->Mesh->CreateEntity(o);
	e->SetTranslate(TRANSLATE_ABS,-200,450,0.8f);
	e->SetScale(110,30,1);
	e->SetMeshAnimation(TS_ButtonAnimation);
	TitlescreenEntity[0] = e;
	w->AddComponent(new ButtonSetup(e,StartButtonClick, NULL));
	e->SetActive(false);

//=====================================
//	Load the texture exitbutton.tga, 
//	add an overlay, create an entity 
//	from that overlay
//=====================================
	tb	=	fusion->Graphics->CreateTexture("file://sprites/exitbutton.tga");
	o	=	fusion->Mesh->CreateOverlay(tb);

	o->AddFrame(new Rect(0,0,128,32));
	o->AddFrame(new Rect(0,32,128,64));
	o->AddFrame(new Rect(0,0,128,32));

//	Create an entity for the button, scale it and add a Mesh animation for it
//	Then create a Button component and add it to the window
	e	=	fusion->Mesh->CreateEntity(o);
	e->SetTranslate(TRANSLATE_ABS,-300,500,0.8f);
	e->SetScale(80,30,1);
	e->SetMeshAnimation(TS_ButtonAnimation);
	TitlescreenEntity[1] = e;
	w->AddComponent(new ButtonSetup(e,ExitButtonClick, NULL));
	e->SetActive(false);

//=====================================
//	Load the texture CreditsButton.tga, 
//	add an overlay, create an entity 
//	from that overlay
//=====================================
	tb	=	fusion->Graphics->CreateTexture("file://sprites/CreditsButton.tga");
	o	=	fusion->Mesh->CreateOverlay(tb);

	o->AddFrame(new Rect(0,0,128,32));
	o->AddFrame(new Rect(0,32,128,64));
	o->AddFrame(new Rect(0,0,128,32));

//	Create an entity for the button, scale it and add a Mesh animation for it
//	Then create a Button component and add it to the window
	e	=	fusion->Mesh->CreateEntity(o);
	e->SetTranslate(TRANSLATE_ABS,-300,550,0.8f);
	e->SetScale(164,30,1);
	e->SetMeshAnimation(TS_ButtonAnimation);
	TitlescreenEntity[2] = e;
	w->AddComponent(new ButtonSetup(e,ShowCredits, NULL));
	e->SetActive(false);

//========================================================================
//	Load each texture, add an overlay, create an entity from that overlay, 
//	set it's initial state to inactive
//========================================================================

//======================
//	PlayerShip-Title.tga
//======================
	tb	=	fusion->Graphics->CreateTexture("file://sprites/PlayerShip-Title.tga");
	o	=	fusion->Mesh->CreateOverlay(tb);
	o->AddFrame();
	e	=	fusion->Mesh->CreateEntity(o);

	//	Scale the entity and set it's animation function
	e->SetTranslate(TRANSLATE_ABS,800,100,0.4f);
	e->SetScale(512,512,1);
	e->SetMeshAnimation(TS_ShipAnimation);
	e->SetActive(false);
	TitleScene->AddEntity(e);
	TitlescreenEntity[3] = e;

//======================
//	Zargadia-Title.tga
//======================
	tb	=	fusion->Graphics->CreateTexture("file://sprites/Zargadia-Title.tga");
	o	=	fusion->Mesh->CreateOverlay(tb);
	o->AddFrame();
	e	=	fusion->Mesh->CreateEntity(o);

	//	Translate/scale the entity and set it's animation function
	e->SetTranslate(TRANSLATE_ABS,10,-120,0.5f);
	e->SetScale(600,120,1);
	e->SetMeshAnimation(TS_ZargadiaTitleAnimation);
	e->SetActive(false);
	TitleScene->AddEntity(e);
	TitlescreenEntity[4] = e;

//======================
//	Mouse Cursor
//======================
	e = Input->InitCursor();
	e->SetActive(false);
	TitleScene->AddEntity(e);
	TitlescreenEntity[5] = e;

//==================================
//	Powered By DodgyVision(tm) logo
//==================================
	Textbox *t;
	t = reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(10,SCREENHEIGHT-15,1,font,"Powered by DodgyVision (tm)")));
	t->SetColour(255,255,255,255);
	t->SetScale(0.5,0.5);
	t->SetActive(false);

//======================
//	Animated Fusion Logo
//======================
	TitleScene->AddEntity(fusionlogo);
}

void ActivateTitleScreen(void)
{
	fusionlogo->SetMeshAnimation(TS_FusionLogoAnimation);

	TitlescreenEntity[0]->SetActive(true);
	TitlescreenEntity[1]->SetActive(true);
	TitlescreenEntity[2]->SetActive(true);
	TitlescreenEntity[3]->SetActive(true);
	TitlescreenEntity[4]->SetActive(true);
	TitlescreenEntity[5]->SetActive(true);
}

//	Titlescreen motions
void TS_ShipAnimation(Entity *entity)
{
	Maths::Vector *position = entity->GetPosition();
	if(position->x>50) position->x+=-50;

	entity->Translate();
	entity->Scale();
}

void TS_ZargadiaTitleAnimation(Entity *entity)
{
	Maths::Vector *position = entity->GetPosition();
	if(position->y<0) position->y+=10;

	entity->Translate();
	entity->Scale();
}

//	Titlescreen Button Motion
void TS_ButtonAnimation(Entity *entity)
{
	Maths::Vector *position = entity->GetPosition();

	if(position->x<600) position->x+=50;

	entity->Translate();
	entity->Scale();
}

void TS_FusionLogoAnimation(Entity *entity)
{
	Maths::Vector *v;
	
	v = entity->GetPosition();

	if(v->x>10)		v->x-=10;
	if(v->y<550)	v->y+=18.75;

	v = entity->GetScale();

	if(v->x>50)	v->x-=18.75;
	if(v->y>50)	v->y-=18.75;

	entity->Translate();
	entity->Scale();
}

//	Titlescreen interface actions
bool StartButtonClick(void)
{
	Zargadia = GameStart;
	return true;
}

bool ExitButtonClick(void)
{
	System = SystemQuit;
	return true;
}
