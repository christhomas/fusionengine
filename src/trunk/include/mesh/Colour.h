#ifndef _COLOUR_H_
	#define _COLOUR_H_

struct Colour3f{
	float r,g,b;
};

struct Colour4f: public Colour3f{
	float a;
};

struct Colour3i{
	int r,g,b;
};

struct Colour4i: public Colour3i{
	int a;
};

struct Colour3b{
	char r,g,b;
};

struct Colour4b: public Colour3b{
	char a;
};

#endif // #ifndef _COLOUR_H_
