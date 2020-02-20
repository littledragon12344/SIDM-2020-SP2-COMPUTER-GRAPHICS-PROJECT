#ifndef TOP_DOWN_CAMERA_H
#define TOP_DOWN_CAMERA_H

#include "Camera.h"

class TopDownCamera : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	TopDownCamera();
	~TopDownCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif