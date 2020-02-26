#include "CCar.h"
#include "Utility.h"

CCar::CCar()
	: current_speed(-1), max_speed(-1), acceleration(-1), distance(-1)
{

}

CCar::~CCar()
{
	
}

//setting the car position
void CCar::SetCarPos(float carPosx, float carPosy, float carPosz)
{
	carPos.x = carPosx;
	carPos.y = carPosy;
	carPos.z = carPosz;
}

float CCar::GetCarPos(char pos)		//pass in the x, y, or z character to get the position
{
	switch (pos)
	{
	case 'x':
		return carPos.x;
	case 'y':
		return carPos.y;
	case 'z':
		return carPos.z;
	default:
		break;
	}
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

//===================Kinematic formulas===================
float CCar::Kinematic1(char mode, float v, float u, float a, float t)
{
	switch (mode)
	{
	case 'v':
		v = u + (a * t);		//finding final speed
		SetMaxSpeed(v);
		return v;
	case 'u':
		u = v - (a * t);		//finding initial speed
		SetCurrentSpeed(u);
		return u;
	case 'a':
		a = (v - u) / t;		//finding acceleration
		SetAcceleration(a);
		return a;
	default:
		break;
	}
}

float CCar::Kinematic2(char mode, float s, float u, float v, float t)
{
	switch (mode)
	{
	case 's':
		s = (u + v) / 2 * t;		//finding displacment/distance
		SetDist(s);
		return s;
	case 'u':
		u = (s * t * 2) - v;		//finding initial speed
		SetCurrentSpeed(u);
		return u;
	case 'v':
		v = (s * t * 2) - u;		//finding final speed
		SetMaxSpeed(v);
		return v;
	default:
		break;
	}
}

float CCar::Kinematic3(char mode, float s, float u, float t, float a)
{
	switch (mode)
	{
	case 's':
		s = (u * t) + (a * (t * t)) / 2;		//finding displpacement/distance
		SetDist(s);
		return s;
	case 'u':
		u = (s - (a * (t * t) / 2)) / t;		//finding initial speed
		SetCurrentSpeed(u);
		return u;
	case 'a':
		a = (s - (u * t)) * 2 / (t * t);		//finding acceleration
		SetAcceleration(a);
		return a;
	default:
		break;
	}
}

float CCar::Kinematic4(char mode, float v, float u, float a, float s)
{
	switch (mode)
	{
	case 'v':
		v = sqrt((u * u) + (2 * a * s));		//finding final speed 
		SetMaxSpeed(v);
		return v;
	case 'u':
		u = sqrt((v * v) - (2 * a * s));		//finding initial speed
		SetCurrentSpeed(u);
		return u;
	case 'a':
		a = ((v * v) - (u * u)) / 2 / s;		//finding acceleration
		SetAcceleration(a);
		return a;
	case 's':
		s = ((v * v) - (u * u)) / 2 / a;		//finding displacement/distance
		SetDist(s);
		return s;
	default:
		break;
	}
}
//========================================================

