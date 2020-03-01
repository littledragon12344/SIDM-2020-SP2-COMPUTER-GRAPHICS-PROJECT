#ifndef MOUSECAM_H
#define MOUSECAM_H

#include "Camera.h"

class MouseCamera : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	MouseCamera();
	~MouseCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif