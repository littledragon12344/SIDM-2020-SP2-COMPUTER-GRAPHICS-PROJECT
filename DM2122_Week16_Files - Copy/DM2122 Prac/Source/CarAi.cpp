#include"CarAI.h"
#include <fstream>
#include <map>
#include "Mtx44.h"


CarAi::CarAi() :rotationSpeed(0.f), PathToGo(NULL), Tempangle(0.f), rotationy(0.f), start(1), angleFromx(0)
{
}

CarAi::~CarAi()
{
}

void CarAi::Updates(float dt)
{
	Vector3 Temp = PathToGo->Point[start];//Get Start of each point
	if (position == defaultPosition)//Starting angle			//Initialize Car ai Target pos and rotation
	{
		Vector3 Temp2 = (*PathToGo).Point[1];
		End = Temp2;//Initialize End
		TargetFromPos = (Temp2 - position).Normalized();
		//position = Temp;
		Vector3 view = (End - position).Normalized();
		//float num = dot(view, Vector3(1, 0, 0)) / (distance(view) * distance(Vector3(1, 0, 0)));//Check distance of start to next End
		rotationy = Math::RadianToDegree(atan2(1 * view.z - 0 * view.x, 1 * view.x + 0));
		rotationy = -rotationy;
	}
	Vector3 Distance = Temp - position;		//Check Distance of the car to the end
	if (sqrt(dot(Distance, Distance)) < 10|| dot(Distance, Distance)<0.1)
	{
		Tempangle = 0;
		//rotationy = angleFromx;
		start++;		//Variable to say move to next start// start = next end , next end = next next end 
		if (start == PathToGo->Point.size())	//Check when start reach the end of line
		{
			start = 0; //start = 1st start variable
			//End = Vector3(Point[0].x, Point[0].y, Point[0].z);	//End = first start vector
		}
		Vector3 Temp2 = PathToGo->Point[start];//get next End
		End = Temp2;//end = next end
		//target = Temp*2;
	}
	Vector3 view = (End -position).Normalized();
	//float num = dot(view, Vector3(1, 0, 0)) / (distance(view) * distance(Vector3(1, 0, 0)));
	angleFromx = Math::RadianToDegree(atan2(1*view.z-0*view.x,1*view.x+0));//Get angel accordin to x 180< angleFromx <-180
	angleFromx = -angleFromx;//for convinent use
	if (Tempangle == 0)
	{
		Vector3 view = (End - position).Normalized();
		Vector3 view2 = (position - target).Normalize();
		float num = dot(view, view2) / (distance(view) * distance(view2));
		//Tempangle = Math::RadianToDegree(acos(num));
		Tempangle = Math::RadianToDegree(atan2(view2.x * view.z - view2.z * view.x, view2.z * view.x + view2.x * view.z));
		if (Tempangle < 0)
			Tempangle = -Tempangle;
	}
	if ((rotationy > 0 && angleFromx > 0)||(rotationy<0&&angleFromx<0))
	{
		if (rotationy < angleFromx)
		{
			rotationy += rotationSpeed * dt;
			if (rotationy > angleFromx)
				rotationy = angleFromx;
		}
		if (rotationy > angleFromx)
		{
			rotationy -= rotationSpeed * dt;
			if (rotationy < angleFromx)
				rotationy = angleFromx;
		}
	}
	else  
	{
		if (rotationy - angleFromx > 180 || rotationy - angleFromx < -180) 
		{
			if (rotationy > 90 && angleFromx < -90)//Check for acute
			{
				Tempangle -= rotationSpeed * dt;
				rotationy += rotationSpeed * dt;
				if (rotationy > (2 * 180 + angleFromx))
				{
					//rotationy = angleFromx;
					Tempangle = 0;
				}
			}
			else if (rotationy < -90 && angleFromx > 90)//Check for acute
			{
				Tempangle -= rotationSpeed * dt;
				rotationy -= rotationSpeed * dt;
				if (rotationy < (-2 * 180 + angleFromx))
				{
				//	rotationy = angleFromx;
					Tempangle = 0;
				}
			}
			else//happen when angleFromx is facing oppsite x directions of rotationy
			{
				if (rotationy < (-angleFromx + rotationy) && Tempangle != 0)
				{
					Tempangle -= rotationSpeed * dt;
					rotationy += rotationSpeed * dt;
					if (rotationy > (-angleFromx + rotationy))
					{
						//rotationy = angleFromx;
						Tempangle = 0;
					}
				}
				if (rotationy > (-angleFromx + rotationy) && Tempangle != 0)
				{
					Tempangle -= rotationSpeed * dt;
					rotationy -= rotationSpeed * dt;
					if (rotationy < (-angleFromx + rotationy))
					{
					//	rotationy = angleFromx;
						Tempangle = 0;
					}
				}
			}
		}
		else
		{
			if (rotationy < angleFromx && Tempangle != 0)
			{
				Tempangle -= rotationSpeed * dt;
				rotationy += rotationSpeed * dt;
				if (rotationy >angleFromx)
				{
					//rotationy = angleFromx;
					Tempangle = 0;
				}
			}
			if (rotationy > angleFromx && Tempangle != 0)
			{
				Tempangle -= rotationSpeed * dt;
				rotationy -= rotationSpeed * dt;
				if (rotationy <angleFromx)
				{
				//	rotationy = angleFromx;
					Tempangle = 0;
				}
			}
		}/*
		if (Tempangle < 0)
		{
			Tempangle = 0;
			rotationy = angleFromx;
		}*/
	}
	if (rotationy > 180)
	{
		rotationy = rotationy-360;
	}
	if (rotationy < -180)
	{
		rotationy = rotationy + 360;
	}
	//if (start == 0)
	//{
	//	//std::cout << angleFromx << std::endl;
	//}
	if (rotationy != angleFromx)
	{
		if (Tempangle > 90)//if angel is big slow down
		{
			acceration = -rotationSpeed / maxspeed;
		}
		else//else coutinue allerate
		{
			acceration = 10.f;
		}
	}
	speed += acceration * dt;
	if (speed > maxspeed)
	{
		speed = maxspeed;
	}
	target = GetTargetpos();
	Vector3 TargetView = (target - position).Normalize();
	position += TargetView * (float)(speed * dt);
//	target = GetTargetpos();

}

//PAth only works idthe vertices is in the rite order best to start with 2d triangle and increase the amount of vertices on it using multicutTool
//void CarAi::GeneratePath(const std::string& file_path,float scale,Vector3 Offset)//get points on the obj
//{
//	std::ifstream fileStream(file_path, std::ios::binary);
//	if (!fileStream.is_open())
//	{
//		std::cout << "Unable to open " << file_path << ". Are you in the right directory ?\n";
//		return;
//	}
//	while (!fileStream.eof())
//	{
//		char buf[256];
//		fileStream.getline(buf, 256);
//		if (strncmp("v ", buf, 2) == 0)
//		{
//			Vector3 vertex;
//			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
//			vertex = vertex * scale;
//			vertex += Offset;
//			Point.push_back(vertex);
//		}
//	}
//
//}

float CarAi::dot(Vector3 Fstnum, Vector3 sndNum)//Dot product in Vector Math
{
	float x = Fstnum.x * sndNum.x;
	float y = Fstnum.y * sndNum.y;
	float z = Fstnum.z * sndNum.z;
	float num =x+z+y;
	return num;
}
float CarAi::distance(Vector3 Num)// vector math to find distance ||
{
	float x = Num.x * Num.x;
	float y = Num.y * Num.y;
	float z = Num.z * Num.z;
	float num = x + z+y;
	num = sqrt(num);
	return num;
}

void CarAi::init(Vector3 pos,Vector3 Target,Vector3 up,float RotateSpeed, Path *paths)//Initiallizing
{
	acceration = 10.f;
	speed = 0.f;
	maxspeed = 60.f;
	PathToGo = paths;
	start = 0;
	rotationSpeed = RotateSpeed;
	this->position = defaultPosition = pos;
	TargetFromPos = defaultTarget = Target;
	this->target = TargetFromPos + position;
	xzTarget = defaultTarget;
	Vector3 view = (position - this->target).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

Vector3 CarAi::GetTargetpos()//Get Target Posiiton using the angles
{
	Vector3 Temp;
	Vector3 xDir = Vector3(1, 0, 0);
	//Since we only rotaing x and y
	Temp.x = cos(Math::DegreeToRadian(-rotationy)) * xDir.x - sin(Math::DegreeToRadian(-rotationy)) * xDir.z;
	Temp.z = sin(Math::DegreeToRadian(-rotationy)) * xDir.x + cos(Math::DegreeToRadian(-rotationy)) * xDir.z;
	Temp.y = 0;
	Temp = position + Temp;
	return Temp;
}

Vector3 CarAi::GetPosition()
{
	return position;
}

float CarAi::GetRotation()
{
	return rotationy;
}
