#include"CarAI.h"
#include <fstream>
#include <map>
#include "Mtx44.h"


CarAi::CarAi()
{
}

CarAi::~CarAi()
{
}

void CarAi::Updates(float dt)
{
	/*Vector3 closest;
	float shortest=1000;
	Vector3 scdClose;
	for (int i = 0; i < Point.size(); i++)
	{
		Vector3 Temp = Vector3(Point[i].x, Point[i].y, Point[i].z);
		Vector3 View =Temp - target;
		float Distance = sqrtf(dot(View, View));
		if (position== Temp)
		{
		}
		if (Distance < shortest)
		{
			scdClose = closest;
			closest = Temp;
			shortest = Distance;
		}
	}*/
	/*if (Start == NULL)
	{
		Start = closest;
		position = closest;
	}
	else
	{
		target = scdClose;
	}*/
	Vector3 Temp = Vector3(Point[start].x, Point[start].y, Point[start].z);
	if (position == defaultPosition)//Starting angle
	{
		Vector3 Temp2 = Vector3(Point[1].x, Point[1].y, Point[1].z);
		End = Temp2;
		TargetFromPos = (Temp2 - Temp).Normalized();
		position = Temp;
		Vector3 view = (End - position).Normalized();
		float num = dot(view, Vector3(1, 0, 0)) / (distance(view) * distance(Vector3(1, 0, 0)));
		rotationy = Math::RadianToDegree(atan2(1 * view.z - 0 * view.x, 1 * view.x + 0));
		rotationy = -rotationy;
	}
	/*if (start == Point.size())
	{
		start = 0;
		End = Vector3(Point[1].x, Point[1].y, Point[1].z);
	}*/
	Vector3 View = Temp - position;
	if (sqrt(dot(View, View)) < 0.5|| dot(View, View)<0.1)
	{
		start++;
		if (start == Point.size())
		{
			start = 0;
			End = Vector3(Point[0].x, Point[0].y, Point[0].z);
		}
		Vector3 Temp2 = Vector3(Point[start].x, Point[start].y, Point[start].z);
		End = Temp2;
		//target = Temp*2;
	}
	Vector3 view = (End -position).Normalized();
	Vector3 TargetView = (target - position).Normalize();
	//float num = dot(view, Vector3(1, 0, 0)) / (distance(view) * distance(Vector3(1, 0, 0)));
	angleFromx = Math::RadianToDegree(atan2(1*view.z-0*view.x,1*view.x+0));
	angleFromx = -angleFromx;
	if ((rotationy > 0 && angleFromx > 0)||(rotationy<0&&angleFromx<0))
	{
		if (rotationy < angleFromx)
		{
			/*float yaw = (float)(250.f * dt * 2);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			xzTarget = rotation * xzTarget;
			TargetFromPos = rotation * TargetFromPos;
			target = TargetFromPos + position;
			up = rotation * up;*/
			rotationy += 250.f * dt;
			if (rotationy > angleFromx)
				rotationy = angleFromx;
		}
		if (rotationy > angleFromx)
		{
		/*	float yaw = (float)(-250.f * dt * 2);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			xzTarget = rotation * xzTarget;
			TargetFromPos = rotation * TargetFromPos;
			target = TargetFromPos + position;
			up = rotation * up;*/
			rotationy -= 250.f * dt;
			if (rotationy < angleFromx)
				rotationy = angleFromx;
		}
	}
	else
	{
		if (Tempangle == 0)
		{
			Vector3 view = (End - position).Normalized();
			Vector3 view2 = (position - target).Normalize();
			float num = dot(view, view2) / (distance(view) * distance(view2));
			//Tempangle = Math::RadianToDegree(acos(num));
			Tempangle = Math::RadianToDegree(atan2(view2.x * view.z - view2.z * view.x, view2.z * view.x + view2.x*view.z));
			if(Tempangle<0)
				Tempangle = -Tempangle;
		}
		if (rotationy + -angleFromx > 180|| rotationy + -angleFromx<-180)
		{
			if (rotationy < (-angleFromx + rotationy) && Tempangle != 0)
			{
			
				Tempangle -= 250.f * dt;
				rotationy += 250.f * dt;
			}
			if (rotationy > (-angleFromx + rotationy) && Tempangle != 0)
			{
		
				Tempangle -= 250.f * dt;
				rotationy -= 250.f * dt;
			}
		}
		else
		{
			if (rotationy < angleFromx && Tempangle != 0)
			{
				Tempangle -= 250.f * dt;
				rotationy += 250.f * dt;
			}
			if (rotationy > angleFromx && Tempangle != 0)
			{
				Tempangle -= 250.f * dt;
				rotationy -= 250.f * dt;
			}
		}
		if (Tempangle < 0)
		{
			Tempangle = 0;
			rotationy = angleFromx;
		}
	}
	if (start == 0)
	{
		std::cout << angleFromx << std::endl;
	}
	position += view * (float)(10.f * dt);
	target += view * (float(10.f * dt));
}

//PAth only works idthe vertices is in the rite order best to start with 2d triangle and increase the amount of vertices on it using multicutTool
void CarAi::GeneratePath(const std::string& file_path,float scale,Vector3 Offset)//get points on the obj
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Unable to open " << file_path << ". Are you in the right directory ?\n";
		return;
	}
	while (!fileStream.eof())
	{
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0)
		{
			Vector3 vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
			vertex = vertex * scale;
			vertex += Offset;
			Point.push_back(vertex);
		}
	}

}

float CarAi::dot(Vector3 Fstnum, Vector3 sndNum)
{
	float x = Fstnum.x * sndNum.x;
	float y = Fstnum.y * sndNum.y;
	float z = Fstnum.z * sndNum.z;
	float num =x+z+y;
	return num;
}
float CarAi::distance(Vector3 Num)
{
	float x = Num.x * Num.x;
	float y = Num.y * Num.y;
	float z = Num.z * Num.z;
	float num = x + z+y;
	num = sqrt(num);
	return num;
}

void CarAi::init(Vector3 pos,Vector3 Target,Vector3 up)
{
	start = 0;
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
