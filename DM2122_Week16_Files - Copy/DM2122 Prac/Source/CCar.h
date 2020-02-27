#pragma once
#include "Vertex.h"
#include "Mesh.h"

class CCar
{
	float current_speed;
	float max_speed;
	float acceleration;
	float distance;
	Position carPos;
	static CCar* s_instance;

public: 
	CCar();
	~CCar();

	static CCar* instance();

	//getting the values
	float GetCurrentSpeed();
	float GetMaxSpeed();
	float GetAcceleration();
	float GetDist();
	//setting values
	void SetCurrentSpeed(float speed);
	void SetMaxSpeed(float speed);
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

