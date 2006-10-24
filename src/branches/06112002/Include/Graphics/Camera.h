#ifndef _CAMERA_H_
	#define _CAMERA_H_

#include <Maths/Quaternion.h>
#include <Maths/Matrix.h>
#include <Maths/Vector.h>

class Camera{
private:
	Maths::Vector			m_position;													//	position of camera within world
	Maths::Vector			m_angle;														//	angle of camera rotation
	Maths::Quaternion	m_quat;															//	Rotation quaternion
	Maths::Matrix			m_view;															//	The View Matrix
	char							*m_Name;														//	The name of the camera

public:
	
	enum{SIDE=0,UP,FORWARD};												//	Axis Identifiers for 6DOF

					Camera		();
					~Camera		();
	
	void		SetName		(char *n);										//	Sets the camera name
	char *	GetName		(void);												//	Retrieves the camera name
	
	void		Zoom			(float z);										//	Zooms the camera in closer(how close? Right down their throats!)

	void		Update		(void);												//	Updates the camera position
	
	void		Rotate		(float x, float y, float z);	//	Rotates the camera by 
	void		Rotate		(Maths::Vector &r);						//	Rotates the camera by vector
	void		Translate	(float dist, int direction);	//	Translates the camera position along a 6DOF vector
};

#endif // #ifndef _CAMERA_H_
