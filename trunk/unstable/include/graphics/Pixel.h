// Pixel.h: interface for the Pixel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIXEL_H__8610A637_AFCF_4415_8BBC_3DEF80C54C83__INCLUDED_)
#define AFX_PIXEL_H__8610A637_AFCF_4415_8BBC_3DEF80C54C83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cstdlib>
#include <cmath>

struct RGB{
	unsigned char r, g, b, a;
};

//=====================
//	Base Pixel Class
//=====================
class Pixel  
{
protected:
	RGB *m_pixel_in;
	RGB m_pixel_out;

	unsigned char *buffer;
public:
	Pixel(unsigned char *buf)
	{
		buffer			=	buf;
		m_pixel_in	= NULL;
	};

	virtual	~Pixel(){};

	virtual	RGB *	ReadPixel	(unsigned int offset)	=	0;
	virtual	void	WritePixel	(unsigned int offset)	=	0;
	virtual	RGB *	GetOutput	(void)					=	0;
	virtual	void	SetInput	(RGB *pixel_in)			=	0;
};

//=====================
//	8-Bit derived class
//=====================
class Pixel8Bit: public Pixel
{
protected:
	unsigned char	*palette;
public:
	inline 	Pixel8Bit(unsigned char *buf, unsigned char *pal): Pixel(buf)
	{
		palette	= pal;
	}

	inline virtual ~Pixel8Bit(){}

	inline virtual RGB * ReadPixel(unsigned int offset)
	{
		unsigned char *ptr = &palette[(buffer[offset] * 3)];

		m_pixel_out.r	= *ptr++;
		m_pixel_out.g	= *ptr++;
		m_pixel_out.b	= *ptr++;
		m_pixel_out.a	= 0;

		return &m_pixel_out;
	}

	inline virtual void WritePixel(unsigned int offset){}	// writing to 8bit surfaces is not allowed, use 32bit instead

	inline virtual RGB * GetOutput(void)
	{
		return	&m_pixel_out;
	}

	inline virtual void SetInput(RGB *pixel_in){}	// writing to 8bit surfaces is not allowed, use 32bit instead
};

//======================
//	16-Bit derived class
//======================
class Pixel16Bit: public Pixel
{
private:
	char	m_NumBit_r, m_NumBit_g, m_NumBit_b;
	char	shift_r, shift_g, shift_b;
	char	pixel_mask_r,pixel_mask_g,pixel_mask_b;

public:
	inline Pixel16Bit(unsigned char *buf, int num_r, int num_g, int num_b): Pixel(buf)
	{
		m_NumBit_r	=	num_r;
		m_NumBit_g	=	num_g;
		m_NumBit_b	=	num_b;

		shift_r	=	16		-	m_NumBit_r;
		shift_g	=	shift_r	-	m_NumBit_g;
		shift_b	=	shift_g	-	m_NumBit_b;

		pixel_mask_r	=	((int)(pow(2,m_NumBit_r))-1) << shift_r;
		pixel_mask_g	=	((int)(pow(2,m_NumBit_g))-1) << shift_g;
		pixel_mask_b	=	((int)(pow(2,m_NumBit_b))-1) << shift_b;
	}

	inline virtual ~Pixel16Bit(){}

	inline virtual RGB *	ReadPixel(unsigned int offset)
	{
		short pixelvalue = (unsigned short)buffer[offset*2];

		m_pixel_out.r = (pixelvalue >>	shift_r)	&	pixel_mask_r;
		m_pixel_out.g = (pixelvalue >>	shift_g)	&	pixel_mask_g;
		m_pixel_out.b = (pixelvalue >>	shift_b)	&	pixel_mask_b;
		m_pixel_out.a = 0;

		return &m_pixel_out;
	}

	inline virtual void	WritePixel(unsigned int offset){}	// writing to 16bit surfaces is not allowed, use 32bit instead

	inline virtual RGB * GetOutput(void){
		return &m_pixel_out;
	}

	inline virtual void SetInput(RGB *pixel_in){}	// writing to 16bit surfaces is not allowed, use 32bit instead
};

//======================
//	24-Bit derived class
//======================
class Pixel24Bit: public Pixel
{
public:
	Pixel24Bit(unsigned char *buf): Pixel(buf){}

	inline virtual ~Pixel24Bit(){}

	inline virtual RGB * ReadPixel(unsigned int offset)
	{
		unsigned char *ptr = &buffer[offset*3];

		m_pixel_out.r = *ptr++;
		m_pixel_out.g = *ptr++;
		m_pixel_out.b = *ptr++;
		m_pixel_out.a = 0;

		return &m_pixel_out;
	}

	inline virtual void	WritePixel(unsigned int offset)
	{
		unsigned char *ptr = &buffer[offset*3];

		ptr[0] = m_pixel_in->r;
		ptr[1] = m_pixel_in->g;
		ptr[2] = m_pixel_in->b;
	}

	inline virtual RGB * GetOutput(void)
	{
		return &m_pixel_out;
	}

	inline virtual void SetInput(RGB *pixel_in)
	{
		m_pixel_in = pixel_in;
	}
};

class Pixel32Bit: public Pixel
{
public:
	Pixel32Bit(unsigned char *buf): Pixel(buf){}

	inline virtual ~Pixel32Bit(){}

	inline virtual RGB * ReadPixel(unsigned int offset)
	{
		unsigned char *ptr = &buffer[offset*4];

		m_pixel_out.r = *ptr++;
		m_pixel_out.g = *ptr++;
		m_pixel_out.b = *ptr++;
		m_pixel_out.a = *ptr++;

		return &m_pixel_out;
	}

	inline virtual void	WritePixel(unsigned int offset)
	{
		unsigned char *ptr = &buffer[offset*4];

		ptr[0]	=	m_pixel_in->r;
		ptr[1]	=	m_pixel_in->g;
		ptr[2]	=	m_pixel_in->b;
		ptr[3]	=	m_pixel_in->a;
	}

	inline virtual RGB * GetOutput(void)
	{
		return &m_pixel_out;
	}

	inline virtual void SetInput(RGB *pixel_in)
	{
		m_pixel_in = pixel_in;
	}
};

#endif // !defined(AFX_PIXEL_H__8610A637_AFCF_4415_8BBC_3DEF80C54C83__INCLUDED_)
