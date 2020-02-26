#ifndef CAR_MINI_H
#define CAR_MINI_H

#include "Vector3.h"
#include <vector>
#include <MyMath.h>
#include "LoadOBJ.h"
#include <iostream>
#include "Mesh.h"
#include "CCar.h"

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
	float Speed;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	CarMinigame();
	~CarMinigame();
	void init(Vector3 position, Vector3 Target, Vector3 UP, float RotateSpeed);
	void GetNewSpeed(float dt, float Acceration);
	void Updates(float dt);
	Vector3 GetTargetpos();
};

#endif