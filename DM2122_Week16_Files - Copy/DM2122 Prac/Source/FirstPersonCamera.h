#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "Vector3.h"

class FirstPersonCamera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 TargetFromPos;
	Vector3 xzTarget;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	FirstPersonCamera();
	~FirstPersonCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
};

#endif

