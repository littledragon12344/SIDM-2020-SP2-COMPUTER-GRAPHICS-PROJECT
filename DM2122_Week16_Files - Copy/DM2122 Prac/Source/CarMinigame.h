#ifndef CAR_MINI_H
#define CAR_MINI_H

#include "Vector3.h"
#include <vector>
#include <MyMath.h>
#include "LoadOBJ.h"
#include <iostream>


class CarMinigame
{
private:
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 TargetFromPos;
	Vector3 xzTarget;
	float rotationy;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	CarMinigame();
	~CarMinigame();
	void Updates(float dt);
	//void GeneratePath(const std::string& file_path, float scale, Vector3 Offset);
	void GenerateCarMesh();
	void init(Vector3 position, Vector3 Target, Vector3 UP, float RotateSpeed);
};

#endif