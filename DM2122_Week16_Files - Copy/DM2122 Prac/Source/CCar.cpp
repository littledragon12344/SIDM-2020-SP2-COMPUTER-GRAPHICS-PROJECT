#include "CCar.h"
#include <fstream>

std::vector<CCar*>CCar:: AllCar;

std::vector<CCar*>CCar::GetAllCar()
{
	return AllCar;
}

CCar::CCar()
	: current_speed(-1), max_speed(-1), acceleration(-1), distance(-1)
{

}

CCar::~CCar()
{
	
}

void CCar::CreateCar(Mesh* car_mesh, Mesh* wheel_mesh, float max_speed, float acceleration)
{
	CCar* temp = new CCar;
	temp->SetMaxSpeed(max_speed);
	temp->SetAcceleration(acceleration);
	AllCar.push_back(temp);
}

//===================Setters & Getters===================
float CCar::GetCurrentSpeed()
{
	return current_speed;
}

float CCar::GetMaxSpeed()
{
	return max_speed;
}

float CCar::GetAcceleration()
{
	return acceleration;
}
float CCar::GetDist()
{
	return distance;
}

void CCar::SetCurrentSpeed(float speed)
{
	this->current_speed = speed;
}

void CCar::SetMaxSpeed(float speed)
{
	this->max_speed = speed;
}

void CCar::SetDist(float dist)
{
	this->distance = dist;
}

void CCar::SetAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}
//=======================================================

//===================Kinematic formula===================
void CCar::CalculateSpeed(float a, float u, float t)
{
	SetCurrentSpeed((u + (a * t)));
}
//=======================================================

