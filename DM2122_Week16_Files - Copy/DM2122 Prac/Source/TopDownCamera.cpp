#include "TopDownCamera.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

TopDownCamera::TopDownCamera()
{
}

TopDownCamera::~TopDownCamera()
{
}

void TopDownCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	//TargetFromPos = defaultTarget = target;
	this->target =defaultTarget= target;
	xzTarget =this->up=defaultUp= up;
	Vector3 view = (position - this->target).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
}

void TopDownCamera::Update(double dt)
{

	static const float CAMERA_SPEED = 50.f;
	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(CAMERA_SPEED * dt * 2);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		xzTarget = rotation * xzTarget;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt * 2);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		xzTarget = rotation * xzTarget;
		up = rotation * up;
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

void TopDownCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	xzTarget = defaultUp;
	up = defaultUp;
}