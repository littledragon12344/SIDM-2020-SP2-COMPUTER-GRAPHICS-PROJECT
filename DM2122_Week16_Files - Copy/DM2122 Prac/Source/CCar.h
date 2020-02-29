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
	Mesh* car_mesh;
	Mesh* wheel_mesh;
	Mesh* interior_mesh;

public: 
	CCar();
	~CCar();

	static std::vector<CCar*>AllCar;

	std::vector<CCar*>GetAllCar();

	static void CreateCar(Mesh* car_mesh, Mesh* wheel_mesh, Mesh* interior_mesh, float max_speed,float acceleration);

	Mesh* GetCarMesh();
	Mesh* GetWheelMesh();
	Mesh* GetInteriorMesh();

	void SetCarMesh(Mesh* mesh);
	void SetWheelMesh(Mesh* mesh);
	void SetInteriorMesh(Mesh* mesh);

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
	void CalculateSpeed(float a, float u, float t);
};

