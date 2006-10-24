#include <Win32/DirectX/DG8Graphics.h>
#include <Win32/Win32Device.h>

DG8Graphics::DG8Graphics()
{
	object_name			=	"DirectGraphics Version 1.0 <chris_thomas@hotmail.com>";
	object_filename	= "libDirectGraphics8.dll";

	m_active				=	false;
	Enable3DMode		=	true;

	m_Direct3D			=	NULL;
	m_RenderDevice	=	NULL;

	m_width					=	0;
	m_height				=	0;
	m_bpp						=	32;
	m_fullscreen		=	false;

	memset(&m_clear,0,sizeof(Colour3f));
}

DG8Graphics::~DG8Graphics()
{
	for(int a=0;a<Textures.size();a++) delete Textures[a];
	Textures.clear();

	m_RenderDevice->Release();
	m_Direct3D->Release();
}

bool DG8Graphics::Initialise(void)
{
	//	Initialise all the platform data
	m_platform = reinterpret_cast<Win32PlatformData *>(fusion->GetPlatformData());

	m_platform->m_hwnd	=	NULL;
	m_platform->m_hinst	=	NULL;

	Window = new Win32Device();

	return true;
}

bool DG8Graphics::SetupGraphics(void)
{
	if((m_Direct3D = Direct3DCreate8(D3D_SDK_VERSION)) != NULL){
		if(m_Direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&m_DisplayMode) == D3D_OK){
			// Set up the structure used to create the D3DDevice
			D3DPRESENT_PARAMETERS d3dpp;
			memset(&d3dpp,0,sizeof(d3dpp) );
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat = m_DisplayMode.Format;

			if(m_Direct3D->CreateDevice(	D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_HAL, 
											m_platform->m_hwnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp,
											&m_RenderDevice) == D3D_OK){
				
				m_RenderDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
				m_RenderDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

				Disable(IGraphics::LIGHTING);
				SetClearColour(0,0,0);

				return true;
			}
		}
	}


	return false;
}

void DG8Graphics::SetClearColour(float r, float g, float b)
{
	m_clear.r = (char)(r*255);
	m_clear.g = (char)(g*255);
	m_clear.b = (char)(b*255);
}

// Resize And Initialize The Window
void DG8Graphics::ResizeWindowDisplay(int x, int y)
{
	D3DXMatrixPerspectiveFovRH(&m_tempmatrix, 45.0f, (float)x/(float)y, 1.0f, 5000.0f);

	if(m_RenderDevice != NULL){
		m_RenderDevice->MultiplyTransform(D3DTS_PROJECTION,&m_tempmatrix);
	
		D3DXMatrixIdentity(&m_tempmatrix);
		m_RenderDevice->MultiplyTransform(D3DTS_VIEW,&m_tempmatrix);
	}
}

void DG8Graphics::ClearScreen(void)
{
    // Clear the backbuffer to a blue color
    m_RenderDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(m_clear.r,m_clear.g,m_clear.b), 1.0f, 0 );
    
    // Begin the scene
    m_RenderDevice->BeginScene();
}

void DG8Graphics::Update(void)
{
	// End the scene
	m_RenderDevice->EndScene();
    
	// Present the backbuffer contents to the display
	m_RenderDevice->Present( NULL, NULL, NULL, NULL );

	Window->Update();
}

bool DG8Graphics::SetMode(int x, int y, bool f)
{
	m_width				= x;
	m_height			= y;
	m_fullscreen	= f;

	if(Window->Open(x,y,f) == true)
	{
		if(SetupGraphics() == true){
			ResizeWindowDisplay(m_width,m_height);
			return true;
		}
	}

	return false;
}

bool DG8Graphics::Enable(int flag, int options)
{
	switch(flag){
		case IGraphics::LIGHTING:{
			m_RenderDevice->SetRenderState(D3DRS_LIGHTING,true);
			m_RenderDevice->SetRenderState(D3DRS_COLORVERTEX,true);
		}break;

		case IGraphics::LIGHT:{
			options &= 7;

			m_RenderDevice->LightEnable(options,true);

			memset(&lightdata[options],0,sizeof(D3DLIGHT8));

			lightdata[options].Type = D3DLIGHT_POINT;

			m_RenderDevice->SetLight(options,&lightdata[options]);
		}break;

		case IGraphics::MODE3D:{
			
		}break;

		case IGraphics::MODE2D:{
			
		}break;
	};

	return true;
}

bool DG8Graphics::Disable(int flag, int options)
{
	switch(flag){
		case IGraphics::LIGHTING:{
			m_RenderDevice->SetRenderState(D3DRS_LIGHTING,false);
		}break;

		case IGraphics::LIGHT:{
			options&=7;
			m_RenderDevice->LightEnable(options,false);
		}break;
	};

	return true;
}

int DG8Graphics::GetRenderMode(void)
{
	return 0;
}

//=================================
//	Get	Video mode information
//=================================
void DG8Graphics::GetScreenResolution(int &x, int &y, int &bpp)
{
	x		= m_width;
	y		= m_height;
	bpp	= m_bpp;
}

int DG8Graphics::GetScreenWidth(void)
{
	return m_width;
}

int DG8Graphics::GetScreenHeight(void)
{
	return m_height;
}

int DG8Graphics::GetScreenDepth(void)
{
	return m_bpp;
}

//==============================
//	Texture Management
//==============================
ITexture * DG8Graphics::CreateTexture(int x, int y)
{
	return NULL;
}

ITexture * DG8Graphics::CreateTexture(char *image)
{
	ITexture *texture = NULL;

	return texture;
}

ITexture * DG8Graphics::CreateTexture(int x, int y, ITexture::textureproc_t proc)
{
	ITexture *texture = NULL;
	
	return texture;
}

void DG8Graphics::DeleteTexture(ITexture *texture)
{

}

void DG8Graphics::DeleteTexture(unsigned int index)
{

}

ITexture * DG8Graphics::GetTexture(unsigned int index)
{
	ITexture *t = NULL;

	return t;
}

void DG8Graphics::ReloadTextures(void)
{

}

//=====================
//	Lighting Methods
//=====================

void DG8Graphics::LightPosition(unsigned int light, Maths::Vector &position)
{
	light &= 7;

	memset(&lightdata[light],0,sizeof(D3DLIGHT8));

	lightdata[light].Type = D3DLIGHT_POINT;
	lightdata[light].Position.x = position.x;
	lightdata[light].Position.y = position.y;
	lightdata[light].Position.z = position.z;

	m_RenderDevice->SetLight(light,&lightdata[light]);
}

void DG8Graphics::LightDiffuse(unsigned int light, Colour4f *diffuse)
{
	light &= 7;

	lightdata[light].Diffuse.r = diffuse->r;
	lightdata[light].Diffuse.g = diffuse->g;
	lightdata[light].Diffuse.b = diffuse->b;
	lightdata[light].Diffuse.a = diffuse->a;

	m_RenderDevice->SetLight(light,&lightdata[light]);
}

void DG8Graphics::LightAmbient(unsigned int light, Colour4f *ambient)
{
	light &= 7;

	lightdata[light].Ambient.r = ambient->r;
	lightdata[light].Ambient.g = ambient->g;
	lightdata[light].Ambient.b = ambient->b;
	lightdata[light].Ambient.a = ambient->a;

	m_RenderDevice->SetLight(light,&lightdata[light]);	
}

//=======================================
//	Rendering Methods
//=======================================
IVertexBuffer * DG8Graphics::CreateVertexBuffer(int mode)
{
	if(mode & IVertexBuffer::STATIC)	return new DG8StaticVB();
	if(mode & IVertexBuffer::DYNAMIC)	return new DG8DynamicVB();

	return NULL;
}

void DG8Graphics::SetTexture(ITexture *texobj)
{
	texobj->Set();
}

void DG8Graphics::SetColour(float *rgba)
{

}

void DG8Graphics::SetColour(float r, float g, float b, float a)
{

}

void DG8Graphics::LoadIdentity()
{
	D3DXMatrixIdentity(&m_tempmatrix);

	m_RenderDevice->SetTransform(D3DTS_VIEW,&m_tempmatrix);
}

void DG8Graphics::Translate(Maths::Vector &position)
{
	D3DXMatrixTranslation(&m_tempmatrix,position.x,position.y,position.z);

	m_RenderDevice->MultiplyTransform(D3DTS_VIEW,&m_tempmatrix);
}

void DG8Graphics::Translate(float x, float y, float z)
{
	D3DXMatrixTranslation(&m_tempmatrix,x,y,z);

	m_RenderDevice->MultiplyTransform(D3DTS_VIEW,&m_tempmatrix);
}

void DG8Graphics::Scale(Maths::Vector &scale)
{
	D3DXMatrixScaling(&m_tempmatrix,scale.x,scale.y,scale.z);

	m_RenderDevice->MultiplyTransform(D3DTS_VIEW,&m_tempmatrix);
}

void DG8Graphics::Scale(float x, float y, float z)
{
	D3DXMatrixScaling(&m_tempmatrix,x,y,z);

	m_RenderDevice->MultiplyTransform(D3DTS_VIEW,&m_tempmatrix);
}

void DG8Graphics::Rotate(float *rotate)
{
	Rotate(rotate[0],rotate[1],rotate[2],rotate[3]);
}

void DG8Graphics::Rotate(float angle, float x, float y, float z)
{
	m_quat.LoadIdentity();
	m_quat.Rotate(angle,x,y,z);
	
	m_RenderDevice->MultiplyTransform(D3DTS_VIEW,(D3DMATRIX*)m_quat.GetMatrix());
}

void DG8Graphics::Rotate(float r, float p, float y)
{
	m_quat.LoadIdentity();
	m_quat.Rotate(r,p,y);

	m_RenderDevice->MultiplyTransform(D3DTS_VIEW,(D3DMATRIX*)m_quat.GetMatrix());
}

void DG8Graphics::MultiplyMatrix(float *matrix)
{
	m_RenderDevice->MultiplyTransform(D3DTS_VIEW,(D3DMATRIX*)matrix);
}

//	Restore the default win32 display mode
bool DG8Graphics::RestoreMode(void)
{
	return false;
}
