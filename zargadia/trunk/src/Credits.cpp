#include <Zargadia.h>

//	Globals
UserInterface	*CreditUI;
SceneGraph		*CreditScene;

#define SETUP		0
#define EXECUTE		1
#define COMPLETE	2
static int state = SETUP;

void BuildCreditUI(void)
{
	CreditScene	= fusion->Scene->AddScene();
	CreditUI	= fusion->Interface->AddUI(CreditScene);

	Window *w = CreditUI->AddWindow(fusion->Input);

	w->AddComponent(new TextBoxSetup(50,250,1,font,"Code:  "));
	w->AddComponent(new TextBoxSetup(50,300,1,font,"Gfx:   "));
	w->AddComponent(new TextBoxSetup(50,350,1,font,"Music: "));

	Textbox *t;

	t = reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(200,250,1,font,"Chris Thomas")));
	t->SetColour(255,0,0,255);
	t = reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(200,300,1,font,"Chris Thomas")));
	t->SetColour(255,0,0,255);
	t = reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(200,350,1,font,"Elwood")));
	t->SetColour(255,0,0,255);

	ITexture	*tb;
	Overlay	*o;
	Entity		*e;

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
	e->SetTranslate(TRANSLATE_ABS,700,550,0.8f);
	e->SetScale(80,30,1);
	w->AddComponent(new ButtonSetup(e,HideCredits, NULL));

	CreditScene->AddEntity(Input->InitCursor());
}

bool ShowCredits(void)
{
	TitleUI->SetActive(false);
	Sound->TitleMusic->Stop();

	CreditUI->SetActive(true);
	Sound->CreditsMusic->Play();

	fusion->Scene->ActivateScene(CreditScene);

	return true;
}

bool HideCredits(void)
{
	CreditUI->SetActive(false);
	Sound->CreditsMusic->Stop();

	TitleUI->SetActive(true);
	Sound->TitleMusic->Play();

	fusion->Scene->ActivateScene(TitleScene);

	return true;
}
