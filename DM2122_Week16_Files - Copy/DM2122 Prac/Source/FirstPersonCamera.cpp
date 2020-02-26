#include "FirstPersonCamera.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

FirstPersonCamera::FirstPersonCamera()
{
}

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	//TargetFromPos = defaultTarget = target;
	this->target = defaultTarget = target;
	xzTarget = this->up = defaultUp = up;
	Vector3 view = (position - this->target).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = up;
}

void FirstPersonCamera::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = target - position;
		Vector3 direction = position - target;
		//Vector3 view = xzTarget.Normalized();
		target -= view * (float)(10.f * dt);
		position -= view * (float)(10.f * dt);
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = target - position;
		//Vector3 view = xzTarget.Normalized();
		target += view * (float)(10.f * dt);
		position += view * (float)(10.f * dt);

	}
	if (Application::IsKeyPressed('A'))
	{
		float yaw = (float)(CAMERA_SPEED * dt * 2);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		TargetFromPos = rotation * TargetFromPos;
		target = TargetFromPos + position;
		//up = rotation * up;
	}
	if (Application::IsKeyPressed('D'))
	{
		float yaw = (float)(-CAMERA_SPEED * dt * 2);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0); 
		TargetFromPos = rotation * TargetFromPos;
		target = TargetFromPos + position;
		//up = rotation * up;
	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
	if (position.x > 90)
	{
		position.x = 90;
		target = TargetFromPos + position;
	}
	if (position.z > 90)
	{
		position.z = 90;
		target = TargetFromPos + position;
	}
	if (position.x < -90)
	{
		position.x = -90;
		target = TargetFromPos + position;
	}
	if (position.z < -90)
	{
		position.z = -90;
		target = TargetFromPos + position;
	}
}

void FirstPersonCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	xzTarget = defaultUp;
	up = defaultUp;
}