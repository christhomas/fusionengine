#include "aiwars.h"

SceneGraph		*LoadScene;		//	Loading scenegraph
SceneGraph		*TitleScene;	//	Titlescreen scenegraph
SceneGraph		*GameScene;		//	In game scenegraph

UserInterface	*LoadUI;			//	Loading interface
UserInterface	*GameUI;			//	Game interface
IFont					*font;				//	Font to render text

void UI_Loading(void)
{
	LoadScene = fusion->Scene->AddScene();
	fusion->Scene->ActivateScene(LoadScene);

	//	Activate Loading scene and Interface + Create a window for components
	LoadUI = fusion->Interface->AddUI(LoadScene);
	LoadUI->SetActive(true);
	
	Window *w =	LoadUI->AddWindow(fusion->Input);

	//	Load up a new font
	font = fusion->Font->AddFont("Lucida.txf",TEXTUREFONT);

	Textbox	*Loading;
	Textbox	*Section;
	Textbox	*PoweredBy;

	//	Create all the Loading screen textboxes
	Loading		=	reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(20,SCREENHEIGHT-80,1,font,"Loading...")));
	Section		=	reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(20,SCREENHEIGHT-60,1,font,"For What its worth")));
	PoweredBy	=	reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(SCREENWIDTH-550,SCREENHEIGHT-570,1,font,"Powered By:")));

	//	Scale down the text accordingly
	Loading->SetScale(0.5,0.5);
	Section->SetScale(0.5,0.5);
	PoweredBy->SetScale(0.5,0.5);

	ITexture	*t;
	Overlay		*o;
	Entity		*e;

	//	Create a texture, apply it to a new overlay
	t	=	fusion->Graphics->CreateTexture("file://sprites/Fusion.tga");
	o	=	fusion->Mesh->CreateOverlay(t);
	//	add a new animation frame
	o->AddFrame();
	//	create an entity and use the overlay as the mesh
	e	=	fusion->Mesh->CreateEntity(o);

	//	Scale the entity and set it's animation function
	e->SetTranslate(TRANSLATE_ABS,SCREENWIDTH-550,SCREENHEIGHT-550,1);
	e->SetScale(500,500,1);
	LoadScene->AddEntity(e);
}	

void UI_Titlescreen(void)
{
	TitleScene	= fusion->Scene->AddScene();

	ITexture	*t;
	Overlay		*o;
	Entity		*e;

	t = fusion->Graphics->CreateTexture("file://sprites/title.tga");
	o	=	fusion->Mesh->CreateOverlay(t);
	o->AddFrame();
	e	=	fusion->Mesh->CreateEntity(o);

	e->SetTranslate(TRANSLATE_ABS,100,50,1);
	e->SetScale(800,150,1);

	TitleScene->AddEntity(e);
}

void UI_Game(void)
{
	GameUI = fusion->Interface->AddUI(GameScene);
	GameUI->SetActive(true);

	Window *w = GameUI->AddWindow(fusion->Input);

	for(int a=0;a<serverbot.size();a++){
		Textbox *e = reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(5,a*20,1,font,"#ERR")));
		Textbox *n = reinterpret_cast<Textbox *>(w->AddComponent(new TextBoxSetup(20,a*20,1,font,"#ERR")));

		e->SetScale(0.3f,0.3f);
		e->SetColour(0,255,0,255);

		n->SetScale(0.3f,0.3f);
		n->SetColour(0,255,0,255);

		serverbot[a]->SetDisplay(e,n);
	}
}