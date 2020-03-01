#include "CarMinigame.h"
#include "Application.h"
#include "Mtx44.h"

CarMinigame::CarMinigame()
{
}

CarMinigame::~CarMinigame()
{
}

void CarMinigame::init(Vector3 pos, Vector3 Tar, Vector3 up, float RotateSpeed,Vector3 StartLine)
{
	position = pos;
	TargetFromPos = Tar;
	target = TargetFromPos + position;
	rotateSpeed = RotateSpeed;
	CCar::AllCar[CCar::CarSwitch]->SetCurrentSpeed(0);
	this->StartLine = StartLine;
	Acceleration = CCar::AllCar[CCar::CarSwitch]->GetAcceleration();
	CCar::AllCar[CCar::CarSwitch]->SetCurrentSpeed(0);
}

void CarMinigame::Updates(float dt)
{
	Vector3 view = (target -position).Normalized();
	static const float Rotation = 50.f;
	target = GetTargetpos();
	if (Application::IsKeyPressed('S'))//Brake
	{
		Acceleration= CCar::AllCar[CCar::CarSwitch]->GetAcceleration()*-2;//Decellarate
	}
	else if (Application::IsKeyPressed('W'))//speed up
	{
		Acceleration = CCar::AllCar[CCar::CarSwitch]->GetAcceleration();
	}
	else//Fake Friciton
	{
		Acceleration = CCar::AllCar[CCar::CarSwitch]->GetAcceleration()/-2;
	}
	if (Application::IsKeyPressed('A'))//Rotation Left
	{
		rotationy += 300.f * dt;
		Acceleration = CCar::AllCar[CCar::CarSwitch]->GetAcceleration() / -2;//Fake decelleration due to Rotation
	}
	if (Application::IsKeyPressed('D'))//Rotate Right
	{
		rotationy -= 300.f * dt;
		Acceleration = CCar::AllCar[CCar::CarSwitch]->GetAcceleration() / -2;//Fake decelleration due to Rotation
	}
	Vector3 Temp =(target - position).Normalized();
		if (Wall::carWallCollision(position + view * (float)(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() * dt)*2, target, 7, 2.5).size() != 0 )
		{
			std::vector<Wall*> Temp = Wall::carWallCollision(position + view * (float)(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() * dt) * 2, GetTargetpos(), 7, 2.5);
			for (int i = 0; i < Temp.size(); i++)
			{
				if (Temp[i]->getPosition() == StartLine&&!Collided)
				{
					round++;
				}
			}
			previousX = position.x;
			previousZ = position.z;
			Collided = true;
		}
		else if (Wall::carWallCollision(position + view * (float)(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() * dt)*2, target, 7, 2.5).size() == 0)
		{
			Collided = false;
		}
		target += view * (float)(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() * dt);
		position += view * (float)(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() * dt);
		if (Collided == true)
		{
			Collidewithwall(Wall::carWallCollision(position + view * (float)(CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() * dt)*2, target, 7, 2.5));
		}
		CCar::AllCar[CCar::CarSwitch]->CalculateSpeed(Acceleration, CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed(), dt);
		if (CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() < 0)
		{
			CCar::AllCar[CCar::CarSwitch]->SetCurrentSpeed(0);
		}
		if (CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed() > CCar::AllCar[CCar::CarSwitch]->GetMaxSpeed())
		{
			CCar::AllCar[CCar::CarSwitch]->SetCurrentSpeed(CCar::AllCar[CCar::CarSwitch]->GetMaxSpeed());
			Acceleration = 1;
		}
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

void CarMinigame::Collidewithwall(std::vector<Wall*> wallcollide)
{
	for (int i = 0; i < wallcollide.size(); i++)
	{
		if (wallcollide[i]->getPosition() == StartLine)
		{
		}
		else
		{
			Acceleration = -CCar::AllCar[CCar::CarSwitch]->GetCurrentSpeed();
			if (wallcollide[i]->getLength() > wallcollide[i]->getDepth())
			{
				position.z = previousZ;
				/*if (position.x > wallcollide[i]->getPosition().x + wallcollide[i]->getLength() && position.x < wallcollide[i]->getPosition().x - wallcollide[i]->getLength())
					position.x = previousX;*/
			}
			if (wallcollide[i]->getLength() < wallcollide[i]->getDepth())
			{
				position.x = previousX;
			/*	if (position.z > wallcollide[i]->getPosition().z + wallcollide[i]->getDepth() && position.z < wallcollide[i]->getPosition().z - wallcollide[i]->getDepth())
					position.z = previousZ;*/
			}
		}
	}
}
