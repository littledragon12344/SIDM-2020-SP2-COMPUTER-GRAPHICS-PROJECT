#pragma once
#include "Vertex.h"
class CCar
{
	float speed;
	float acceleration;
	float distance;
	Position carPos;

public: 
	CCar();
	~CCar();

	//getting the values
	float GetSpeed();
	float GetAcceleration();
	float GetDist();
	//setting values
	void SetSpeed(float speed);
	void SetAcceleration(float acceleration);
	void SetDist(float dist);

	//car position management
	void SetCarPos(float carPosx, float carPosy, float carPosz);
	float GetCarPos(char pos);

	//kinematic formulas
	float Kinematic1(char mode, float v, float u, float a, float t);
	float Kinematic2(char mode, float s, float u, float v, float t);
	float Kinematic3(char mode, float s, float u, float t, float a);
	float Kinematic4(char mode, float v, float u, float a, float s);

};

