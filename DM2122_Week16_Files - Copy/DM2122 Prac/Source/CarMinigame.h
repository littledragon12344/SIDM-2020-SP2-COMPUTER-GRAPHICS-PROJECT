#ifndef CAR_MINI_H
#define CAR_MINI_H

#include "Vector3.h"
#include <vector>
#include <MyMath.h>
#include "LoadOBJ.h"
#include <iostream>
#include "Mesh.h"
#include "CCar.h"
#include "Wall.h"

class CarMinigame
{
private:
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 TargetFromPos;
	float rotationy;
	float rotateSpeed;
	float Acceleration;
	int round=0;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 StartLine;

	float previousX;
	float previousZ;
	bool Collided;

	CarMinigame();
	~CarMinigame();
	void init(Vector3 position, Vector3 Target, Vector3 UP, float RotateSpeed, Vector3 StartLine);
	void GetNewSpeed(float dt, float Acceration);
	void Updates(float dt);
	Vector3 GetTargetpos();
	void Collidewithwall(std::vector<Wall*> wallcollide);
};

#endif