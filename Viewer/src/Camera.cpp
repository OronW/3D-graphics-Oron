#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

glm::mat4x4 Camera::Perspective(const float fovy, const float aspect, const float zNear, const float zFar)
{
	return { fovy / aspect,		0,					 0,										0,
				0,				fovy,				 0,							  			0,	
			    0,				0,			-(zFar + zNear) / (zFar - zNear),		-2 * zFar*zNear / (zFar - zNear),
		        0,				0,					-1,										 0							 };
}