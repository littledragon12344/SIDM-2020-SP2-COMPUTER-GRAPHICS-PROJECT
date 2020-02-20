#pragma once
class CCar
{
	//float u;	//initial speed 
	//float v;	//final speed
	//float a;	//accelation
	//float s;	//displacement
	//float t;	//time
	float speed;
	float acceleration;

public: 
	CCar();
	~CCar();

	float GetSpeed();
	void SetSpeed(float speed);
	float GetAcceleration();
	void SetAcceleration(float acceleration);

	float Kinematic1(float v, float u, float a, float t);
	float Kinematic2(float s, float u, float v, float t);
	float Kinematic3(float s, float u, float t, float a);
	float Kinematic4(float v, float u, float a, float s);

};

