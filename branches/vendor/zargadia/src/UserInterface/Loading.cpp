#include <Zargadia.h>

SceneGraph		*LoadScene;						//	Loading Scene
UserInterface	*LoadUI;						//	UserInterfaces
Textbox			*Loading,*Section,*PoweredBy;	//	Loading Screen output textboxes
IFont			*font;							//	Font to render text
Entity			*fusionlogo;					//	Fusion logo entity (for controlling it's animation)

void BuildLoadingUI(void)
{
	LoadScene = fusion->Scene->AddScene();
	fusion->Scene->ActivateScene(LoadScene);

	//	Activate Loading scene and Interface + Create a window for components
	LoadUI = fusion->Interface->AddUI(LoadScene);
	LoadUI->SetActive(true);
	
	Window *w =	LoadUI->AddWindow(fusion->Input);

	//	Load up a new font
	font = fusion->Font->AddFont("lucida.txf",TEXTUREFONT);

	//	Create all the Loading screen textboxes
	Loading		=	(Textbox *)w->AddComponent(new TextBoxSetup(20,550,1,font,"Loading..."));
	Section		=	(Textbox *)w->AddComponent(new TextBoxSetup(20,570,1,font,"UserInterface: Title"));
	PoweredBy	=	(Textbox *)w->AddComponent(new TextBoxSetup(SCREENWIDTH/3,100,1,font,"Powered By:"));

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
	e->SetTranslate(TRANSLATE_ABS,250,100,1);
	e->SetScale(500,500,1);
	LoadScene->AddEntity(e);

	fusionlogo = e;
}	
