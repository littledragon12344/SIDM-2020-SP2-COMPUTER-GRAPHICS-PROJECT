#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"
#include "Wall.h"

MouseCamera::MouseCamera()
{
}

MouseCamera::~MouseCamera()
{
}

void MouseCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	TargetFromPos = defaultTarget = target;
	this->target = TargetFromPos + position;
	xzTarget = defaultTarget;
	Vector3 view = (position - this->target).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void MouseCamera::Update(double dt)
{

	static const float CAMERA_SPEED = 15.f;
	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(CAMERA_SPEED * dt * 5);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		xzTarget = rotation * xzTarget;
		TargetFromPos = rotation * TargetFromPos;
		target = TargetFromPos + position;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt * 5);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		xzTarget = rotation * xzTarget;
		TargetFromPos = rotation * TargetFromPos;
		target = TargetFromPos + position;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		Vector3 Temp = position;
		Temp.y = 2;
		Vector3 view = (position - target).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		TargetFromPos = rotation * TargetFromPos;
		target = TargetFromPos + Temp;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		Vector3 Temp = position;
		Temp.y = 2;
		Vector3 view = (position - target).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		TargetFromPos = rotation * TargetFromPos;
		target = TargetFromPos + Temp;
	}

	/*if(Application::IsKeyPressed('S'))
	{
		Vector3 direction = position - target;
			Vector3 view = xzTarget.Normalized();
			target -= view * (float)(10.f * dt);
			position -= view * (float)(10.f * dt);
	}
	if( Application::IsKeyPressed('W'))
	{
		Vector3 view = xzTarget.Normalized();
		target += view * (float)(10.f * dt);
		position += view * (float)(10.f * dt);

	}
	if (Application::IsKeyPressed('A'))
	{
		Vector3 view = xzTarget.Normalized();
		Vector3 viewLeft;
		viewLeft = Vector3(cos(Math::DegreeToRadian(-90))*view.x-sin(Math::DegreeToRadian(-90))*view.z, 0, cos(Math::DegreeToRadian(-90)) * view.z + sin(Math::DegreeToRadian(-90)) * view.x);
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
	}*/

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

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up).Normalized();
	Vector3 posDisplacement;

	if (Application::IsKeyPressed('W'))
		posDisplacement += view;
	if (Application::IsKeyPressed('S'))
		posDisplacement -= view;
	if (Application::IsKeyPressed('A'))
		posDisplacement -= right;
	if (Application::IsKeyPressed('D'))
		posDisplacement += right;

	if (posDisplacement != Vector3(0, 0, 0))
	{
		posDisplacement.y = 0;

		posDisplacement = posDisplacement.Normalized() * (float)(CAMERA_SPEED * dt);
		Vector3 displacementAftCollide = Wall::playerWallCollision(position, posDisplacement);

		position += displacementAftCollide;
		target = position + view;
	}
}

void MouseCamera::Reset()
{
	position = defaultPosition;
	TargetFromPos = defaultTarget;
	target = TargetFromPos + position;
	xzTarget = defaultTarget;
	up = defaultUp;
}