#ifndef CAR_AI_H
#define CAR_AI_H

#include "Vector3.h"
#include <vector>
#include <MyMath.h>
#include "LoadOBJ.h"

class CarAi
{
private:
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 TargetFromPos;
	Vector3 xzTarget;

	int start;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	std::vector<Position> Point;
	CarAi();
	~CarAi();
	void Updates(float dt);
	void GeneratePath(const std::string& file_path);
	float dot(Vector3 Fstnum, Vector3 sndNum);
	void init(Vector3 position, Vector3 Target, Vector3 UP);
};

#endif