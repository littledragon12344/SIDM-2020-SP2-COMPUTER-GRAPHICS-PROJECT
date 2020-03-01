#ifndef CAR_AI_H
#define CAR_AI_H

#include "Vector3.h"
#include <vector>
#include <MyMath.h>
#include "Path.h"
#include "Wall.h"
#include "CCar.h"

class CarAi
{
private:
	Vector3 position;
	Vector3 target;
	Vector3 up;
	Vector3 TargetFromPos;
	Vector3 xzTarget;
	Vector3 End;

	float angleFromx;
	float rotationy;
	int start;
	float Tempangle;
	float rotationSpeed;
public:

	Path *PathToGo;//Get Path information

	int round=0;
	int randCar;
	CCar AiCar;
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	//Collision Condition outcome
	float previousX;
	float previousZ;
	bool Collided;
	//std::vector<Vector3> Point;
	CarAi();
	~CarAi();
	void Updates(float dt);
	//Temporary
	float acceration;
	float speed;
	float maxspeed;
	//Temporary
	//void GeneratePath(const std::string& file_path, float scale, Vector3 Offset);
	float dot(Vector3 Fstnum, Vector3 sndNum);//dot product of 2 num
	float distance(Vector3 Num);// |[]| of a vertex to find distance
	void init(Vector3 position, Vector3 Target, Vector3 UP, float RotateSpeed,Path *paths);//Initializing
	Vector3 GetTargetpos();//Get Target position
	Vector3 GetPosition();//get Position
	float GetRotation();//Get Rotation
	Vector3 GetRandomizePoint();//Set random path point (short distance changes
	Vector3 Getforward();//Get targetfrompos/forward direction
	void Collidewithwall(std::vector<Wall*> wallcollide);//collision outcome
};

#endif