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
	Position carPos;

public: 
	CCar();
	~CCar();

	float GetSpeed();
	void SetSpeed(float speed);
	float GetAcceleration();
	void SetAcceleration(float acceleration);

	void SetCarPos(float carPosx, float carPosy, float carPosz);
	float GetCarPos(char pos);

	float Kinematic1(float v, float u, float a, float t);
	float Kinematic2(float s, float u, float v, float t);
	float Kinematic3(float s, float u, float t, float a);
	float Kinematic4(float v, float u, float a, float s);

};

