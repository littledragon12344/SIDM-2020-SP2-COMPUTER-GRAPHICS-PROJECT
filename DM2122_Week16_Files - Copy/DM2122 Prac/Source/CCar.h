#pragma once
#include "Vertex.h"
#include "Mesh.h"

#include <vector>

class CCar
{
	float current_speed;
	float max_speed;
	float acceleration;
	float distance;
	Position carPos;


public: 
	CCar();
	~CCar();

	static std::vector<CCar*>AllCar;

	std::vector<CCar*>GetAllCar();

	static void CreateCar(float max_speed,float acceleration);

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

	//kinematic formulas
	float Kinematic1(char mode, float v, float u, float a, float t);
	float Kinematic2(char mode, float s, float u, float v, float t);
	float Kinematic3(char mode, float s, float u, float t, float a);
	float Kinematic4(char mode, float v, float u, float a, float s);

};

