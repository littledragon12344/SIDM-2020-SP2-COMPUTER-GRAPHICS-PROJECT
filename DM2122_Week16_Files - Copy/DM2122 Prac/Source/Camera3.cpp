#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position =defaultPosition= pos;
	TargetFromPos = pos-target;
	defaultTarget = TargetFromPos;
	xzTarget = target-pos;
	Vector3 view = (position - this->target).Normalized();
	this->target = target;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(CAMERA_SPEED * dt * 2);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		xzTarget = rotation * xzTarget;
		TargetFromPos = rotation * TargetFromPos;
		position = TargetFromPos + target;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt * 2);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		xzTarget = rotation * xzTarget;
		TargetFromPos = rotation * TargetFromPos;
		position = TargetFromPos + target;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		Vector3 view = (position - target).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		TargetFromPos = rotation * TargetFromPos;
		position = TargetFromPos + target;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		Vector3 view = (position - target).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		TargetFromPos = rotation * TargetFromPos;
		position = TargetFromPos + target;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 direction = position - target;
		Vector3 view = xzTarget.Normalized();
		target -= view * (float)(10.f * dt);
		position -= view * (float)(10.f * dt);
	}
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = xzTarget.Normalized();
		target += view * (float)(10.f * dt);
		position += view * (float)(10.f * dt);

	}
	if (Application::IsKeyPressed('A'))
	{
		Vector3 view = xzTarget.Normalized();
		Vector3 viewLeft;
		viewLeft = Vector3(cos(Math::DegreeToRadian(-90)) * view.x - sin(Math::DegreeToRadian(-90)) * view.z, 0, cos(Math::DegreeToRadian(-90)) * view.z + sin(Math::DegreeToRadian(-90)) * view.x);
		target += viewLeft * (float)(10.f * dt);
		position += viewLeft * (float)(10.f * dt);

	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 view = xzTarget.Normalized();
		Vector3 viewRight;
		viewRight = Vector3(cos(Math::DegreeToRadian(90)) * view.x - sin(Math::DegreeToRadian(90)) * view.z, 0, cos(Math::DegreeToRadian(90)) * view.z + sin(Math::DegreeToRadian(90)) * view.x);
		target += viewRight * (float)(10.f * dt);
		position += viewRight * (float)(10.f * dt);
	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
	if (target.x > 90)
	{
		target.x = 90;
		position = TargetFromPos + target;
	}
	if (target.z > 90)
	{
		target.z = 90;
		position = TargetFromPos + target;
	}
	if (target.x < -90)
	{
		target.x = -90;
		position = TargetFromPos + target;
	}
	if (target.z < -90)
	{
		target.z = -90;
		position = TargetFromPos + target;
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	TargetFromPos = defaultTarget;
	target = TargetFromPos + position;
	xzTarget = defaultTarget;
	up = defaultUp;
}