#include <Win32/DirectX/DG8DynamicVB.h>
#include <Win32/DirectX/DG8Graphics.h>

DG8DynamicVB::DG8DynamicVB()
{
	m_vertexbuffer		=	NULL;
	m_numvertex				=	0;
	m_name						=	NULL;
	m_smoothingangle	=	0;

	memset(&m_material,0,sizeof(D3DMATERIAL8));
}

DG8DynamicVB::~DG8DynamicVB()
{
	ReleaseAll();
}

bool DG8DynamicVB::Initialise(int nv, int ni, int nc_p, int nc_t)
{
	int result = 0;
	
	m_numvertex	= nv;
	m_numindex	= ni;

	IDirect3DDevice8 *dev = ((DG8Graphics *)fusion->Graphics)->m_RenderDevice;

	result = dev->CreateVertexBuffer(	nv*sizeof(CUSTOMVERTEX),
										D3DUSAGE_DYNAMIC,
										D3DFVF_CUSTOMVERTEX,
										D3DPOOL_DEFAULT,
										&m_vertexbuffer);

	if(result != D3D_OK) return false;

	result = dev->CreateIndexBuffer(	ni*sizeof(int),
										D3DUSAGE_DYNAMIC,
										D3DFMT_INDEX32,
										D3DPOOL_DEFAULT,
										&m_indexbuffer);

	CUSTOMVERTEX *ptr = NULL;

	result = m_vertexbuffer->Lock(0,0,(unsigned char **)&ptr,D3DLOCK_DISCARD);

	if(result == D3D_OK){
		for(int a=0;a<nv;a++){
			ptr[a].colour = DG8COLOUR(	(char)m_material.Diffuse.r*256,
																	(char)m_material.Diffuse.g*256,
																	(char)m_material.Diffuse.b*256,
																	(char)m_material.Diffuse.a*256);
		}
	}

	m_vertexbuffer->Unlock();

	if(result != D3D_OK) return false;

	return true;
}

void DG8DynamicVB::ReleaseAll(void)
{
	m_vertexbuffer->Release();

	delete[] m_name;
}

void DG8DynamicVB::SetComponents(int p, int t)
{

}

void DG8DynamicVB::SetName(char *n)
{
	delete[] m_name;

	m_name = new char[strlen(n)+1];

	strcpy(m_name,n);
}

void DG8DynamicVB::SetPosition(float *p)
{
	CUSTOMVERTEX *ptr = NULL;
	int result = m_vertexbuffer->Lock(0,0,(unsigned char **)&ptr,D3DLOCK_DISCARD);

	if(result == D3D_OK){
		for(int a=0;a<m_numvertex;a++){
			ptr[a].px	=	*p++;
			ptr[a].py	=	*p++;
			ptr[a].pz	=	*p++;
		}

		m_vertexbuffer->Unlock();
	}
}

void DG8DynamicVB::SetNormal(float *n)
{
	int a;

	CUSTOMVERTEX *ptr = NULL;

	int result = m_vertexbuffer->Lock(0,0,(unsigned char **)&ptr,D3DLOCK_DISCARD);

	if(result == D3D_OK){
		for(a=0;a<m_numvertex;a++){
			ptr[a].nx	=	*n++;
			ptr[a].ny	=	*n++;
			ptr[a].nz	=	*n++;
		}

		m_vertexbuffer->Unlock();
	}
}

void DG8DynamicVB::SetTexcoord(float *t)
{

}

void DG8DynamicVB::SetIndex(int *i)
{
	int *ptr = NULL;

	int result = m_indexbuffer->Lock(0,0,(unsigned char **)&ptr,D3DLOCK_DISCARD);

	if(result == D3D_OK){
		memcpy(ptr,i,m_numindex*sizeof(int));	
	}
}

void DG8DynamicVB::SetTexture(ITexture *t)
{

}

void DG8DynamicVB::SetColour(Colour4f *c)
{
	SetColour(c->r,c->g,c->b,c->a);
}

void DG8DynamicVB::SetColour(float r, float g, float b, float a)
{
	m_material.Diffuse.r = r/256;
	m_material.Diffuse.g = g/256;
	m_material.Diffuse.b = b/256;
	m_material.Diffuse.a = a/256;
}

void DG8DynamicVB::SetMaterial(Material *m)
{

}

void DG8DynamicVB::SetSmoothingAngle(float angle)
{
	m_smoothingangle = angle;
}

char * DG8DynamicVB::GetName(void)
{
	return m_name;
}

float * DG8DynamicVB::GetPosition(void)
{
	return NULL;
}

float *	DG8DynamicVB::GetNormal(void)
{
	return NULL;
}

float * DG8DynamicVB::GetTexcoord(void)
{
	return NULL;
}

int * DG8DynamicVB::GetIndex(void)
{
	return NULL;
}

ITexture * DG8DynamicVB::GetTexture(void)
{
	return NULL;
}

Colour4f * DG8DynamicVB::GetColour(void)
{
	return NULL;
}

Material * DG8DynamicVB::GetMaterial(void)
{
	return NULL;
}

float DG8DynamicVB::GetSmoothingAngle(void)
{
	return m_smoothingangle;
}

int DG8DynamicVB::GetNumIndex(void)
{
	return m_numindex;
}

void DG8DynamicVB::Render(void)
{
	DG8Graphics *g = reinterpret_cast<DG8Graphics *>(fusion->Graphics);
	IDirect3DDevice8 *dev = g->m_RenderDevice;

	if(dev != NULL && m_vertexbuffer != NULL){
		dev->SetMaterial(&m_material);
		dev->SetIndices(m_indexbuffer,0);
		dev->SetStreamSource( 0, m_vertexbuffer, sizeof(CUSTOMVERTEX) );
		dev->SetVertexShader( D3DFVF_CUSTOMVERTEX );
		dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,m_numvertex,0,m_numvertex/3);
	}
}
