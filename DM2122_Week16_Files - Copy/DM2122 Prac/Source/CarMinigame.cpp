#include "CarMinigame.h"
#include "Application.h"
#include "Mtx44.h"

CarMinigame::CarMinigame()
{
}

CarMinigame::~CarMinigame()
{
}

void CarMinigame::init(Vector3 pos, Vector3 Tar, Vector3 up, float RotateSpeed)
{
	position = pos;
	TargetFromPos = Tar;
	target = TargetFromPos + position;
	rotateSpeed = RotateSpeed;
}

void CarMinigame::Updates(float dt)
{
	Vector3 view = (target -position).Normalized();
	static const float Rotation = 50.f;
	target = GetTargetpos();
	if (Application::IsKeyPressed('S'))
	{
		Speed -= 25.f*dt;
		
	}
	else if (Application::IsKeyPressed('W'))
	{
		Speed += 10.f*dt;
	}
	else
	{
		Speed -= 3 * dt;
	}
	if (Application::IsKeyPressed('A'))
	{
		rotationy += 300.f * dt;
		Speed -= 5 * dt;
	}
	if (Application::IsKeyPressed('D'))
	{
		rotationy -= 300.f * dt;
		Speed -= 5 * dt;
	}
	if (Speed <= 0)
	{
		Speed = 0;
	}
	Vector3 Temp =(target - position).Normalized();
	if (position.x > 90)
	{
		position.x = 90;
		target = Temp + position;
	}
	 if (position.z > 90)
	{
		position.z = 90;
		target = Temp + position;
	}
	 if (position.x < -90)
	{
		position.x = -90;
		target = Temp + position;
	}
	 if (position.z < -90)
	{
		position.z = -90;
		target = Temp + position;
	}
		target += view * (float)(Speed * dt);
		position += view * (float)(Speed * dt);
}
Vector3 CarMinigame::GetTargetpos()
{
	Vector3 Temp;
	Vector3 xDir = Vector3(1, 0, 0);
	//std::cout << "\a";
	//Since we only rotaing x and y
	Temp.x = cos(Math::DegreeToRadian(-rotationy)) * xDir.x - sin(Math::DegreeToRadian(-rotationy)) * xDir.z;
	Temp.z = sin(Math::DegreeToRadian(-rotationy)) * xDir.x + cos(Math::DegreeToRadian(-rotationy)) * xDir.z;
	Temp.y = 0;
	Temp = position + Temp;
	return Temp;
}