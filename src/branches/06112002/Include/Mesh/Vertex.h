#ifndef _VERTEX_H_
	#define _VERTEX_H_

//	2D vertex for texture coordinates
struct Vertex2f{
	float x,y;
};

//	3D Vertex for general vertices
struct Vertex3f{
	float x,y,z;
};

struct Vertex3s{
	short x,y,z;
};

typedef Vertex3f Vector3f;
typedef Vertex3s Vector3s;

#endif // #ifndef _VERTEX_H_
