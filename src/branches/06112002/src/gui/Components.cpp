#include <gui/GUIComponents.h>

/*
 *	Functions to initialise the library database with the objects compiled into the dll
 */
IWindowComponent * PadComponentFunction()	{	return NULL;							}
IWindowComponent * CreateButton()					{	return new Button();			}
IWindowComponent * CreateTextBox()				{	return new Textbox();			}
IWindowComponent * CreateProgressBar()		{	return new ProgressBar();	}

IWindowComponent * Destroy(IWindowComponent *wc)
{
	delete wc;
	
	return NULL;
}

void update(LIBRARY *library)
{
// get the number of components in the library, then push back the difference between that and the maximum
	int difference = MAXIDCODE - library->size();
	struct WindowComponentFunctionPtrs *wc;

// push back that many pad components
	for(int a=0;a<difference;a++){
		wc = new WindowComponentFunctionPtrs;
		if(a == BUTTON)						wc->m_init	=	CreateButton;
		else if(a == TEXTBOX)			wc->m_init	=	CreateTextBox;
		else if(a == PROGRESSBAR)	wc->m_init	=	CreateProgressBar;
		else											wc->m_init	=	PadComponentFunction;

		wc->m_destroy	= Destroy;

		library->push_back(wc);
	}
}