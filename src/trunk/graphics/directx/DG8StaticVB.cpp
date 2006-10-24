#include <Win32/DirectX/DG8StaticVB.h>

DG8StaticVB::DG8StaticVB()
{
}

DG8StaticVB::~DG8StaticVB()
{
}

bool DG8StaticVB::Initialise(int nv, int ni, int nc_p, int nc_t)
{
	return false;
}

void DG8StaticVB::ReleaseAll(void)
{
}

void DG8StaticVB::SetComponents(int p, int t)
{
}

void DG8StaticVB::SetName(char *n)
{
}

void DG8StaticVB::SetPosition(float *p)
{
}

void DG8StaticVB::SetNormal(float *n)
{
}

void DG8StaticVB::SetTexcoord(float *t)
{
}

void DG8StaticVB::SetIndex(int *i)
{
}

void DG8StaticVB::SetTexture(ITexture *t)
{
}

void DG8StaticVB::SetColour(Colour4f *c)
{
}

void DG8StaticVB::SetColour(float r, float g, float b, float a)
{
}

void DG8StaticVB::SetMaterial(Material *m)
{
}

void DG8StaticVB::SetSmoothingAngle(float angle)
{
	m_smoothingangle = angle;
}

char * DG8StaticVB::GetName(void)
{
	return m_name;
}

float * DG8StaticVB::GetPosition(void)
{
	return NULL;
}

float *	DG8StaticVB::GetNormal(void)
{
	return NULL;
}

float * DG8StaticVB::GetTexcoord(void)
{
	return NULL;
}

int * DG8StaticVB::GetIndex(void)
{
	return NULL;
}

ITexture * DG8StaticVB::GetTexture(void)
{
	return NULL;
}

Colour4f * DG8StaticVB::GetColour(void)
{
	return NULL;
}

Material * DG8StaticVB::GetMaterial(void)
{
	return NULL;
}

float DG8StaticVB::GetSmoothingAngle(void)
{
	return m_smoothingangle;
}

int DG8StaticVB::GetNumIndex(void)
{
	return 0;
}

void DG8StaticVB::Render(void)
{
}
