#ifndef CAR_MINI_H
#define CAR_MINI_H

#include "Vector3.h"
#include <vector>
#include <MyMath.h>
#include "LoadOBJ.h"

class CarMinigame
{
private:
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 TargetFromPos;
	Vector3 xzTarget;

	float angleFromx;
	float rotationy;
	int start;
	float Tempangle;
	float rotationSpeed;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	CarMinigame();
	~CarMinigame();
	void Updates(float dt);
	void GeneratePath(const std::string& file_path, float scale, Vector3 Offset);
	float dot(Vector3 Fstnum, Vector3 sndNum);
	float distance(Vector3 Num);
	void init(Vector3 position, Vector3 Target, Vector3 UP, float RotateSpeed);
};

#endif