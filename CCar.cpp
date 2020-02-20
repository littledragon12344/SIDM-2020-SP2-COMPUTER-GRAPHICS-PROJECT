#include "CCar.h"
#include "Utility.h"

CCar::CCar()
	: speed(-1), acceleration(-1)
{

}

CCar::~CCar()
{

}

float CCar::GetSpeed()
{
	return speed;
}

void CCar::SetSpeed(float speed)
{
	this->speed = speed;
}

float CCar::GetAcceleration()
{
	return acceleration;
}

void CCar::SetAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}

float CCar::Kinematic1(float v, float u, float a, float t)
{
	if (v == GetDefaultValue())		//finding final speed
	{
		v = u + (a * t);
		return v;
	}
	if (u == GetDefaultValue())		//finding initial speed
	{
		u = v - (a * t);
		return u;
	}
	if (a == GetDefaultValue())		//finding acceleration
	{
		a = (v - u) / t;
		return a;
	}
	if (t == GetDefaultValue())		//finding time
	{
		t = (v - u) / a;
		return t;
	}
}

float CCar::Kinematic2(float s, float u, float v, float t)
{
	if (s == GetDefaultValue())		//finding displacment/distance
	{
		s = (u + v) / 2 * t;
		return s;
	}
	if (u == GetDefaultValue())		//finding initial speed
	{
		u = (s * t * 2) - v;
		return u;
	}
	if (v == GetDefaultValue())		//finding final speed
	{
		v = (s * t * 2) - u;
		return v;
	}
	if (t == GetDefaultValue())		//finding time
	{
		t = (s * 2) / (u + v);
		return t;
	}
}

float CCar::Kinematic3(float s, float u, float t, float a)
{
	if (s == GetDefaultValue())		//finding displpacement/distance
	{
		s = (u * t) + (a * (t * t)) / 2;
		return s;
	}
	if (u == GetDefaultValue())		//finding initial speed
	{
		u = (s - (a * (t * t) / 2)) / t;
		return u;
	}
	if (t == GetDefaultValue())		//finding time
	{
		t = sqrt((s - (u * t)) * 2 / a);
		return t;
	}
	if (a == GetDefaultValue())		//finding acceleration
	{
		a = (s - (u * t)) * 2 / (t * t);
		return a;
	}

}

float CCar::Kinematic4(float v, float u, float a, float s)
{
	if (v == GetDefaultValue())		//finding final speed 
	{
		v = sqrt((u * u) + (2 * a * s));
		return v;
	}
	if (u == GetDefaultValue())		//finding initial speed
	{
		u = sqrt((v * v) - (2 * a * s));
		return u;
	}
	if (a == GetDefaultValue())		//finding acceleration
	{
		a = ((v * v) - (u * u)) / 2 / s;
		return a;
	}
	if (s == GetDefaultValue())		//finding displacement/distance
	{
		s = ((v * v) - (u * u)) / 2 / a;
		return s;
	}

}
