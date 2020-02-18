#include"CarAI.h"
#include <fstream>
#include <map>


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
	if (position == defaultPosition)
	{
		Vector3 Temp2 = Vector3(Point[start].x, Point[start].y, Point[start].z);
		target = Temp2;
		position = Temp;
	}
	if (start == Point.size())
	{
		start = 0;
		target = Vector3(Point[0].x, Point[0].y, Point[0].z);
	}
	Vector3 View = Temp - position;
	if (sqrt(dot(View, View)) < 0.5|| dot(View, View)<0.1)
	{
		start++;
		if (start == Point.size())
		{
			start = 0;
			target = Vector3(Point[0].x, Point[0].y, Point[0].z);
		}
		Vector3 Temp2 = Vector3(Point[start].x, Point[start].y, Point[start].z);
		position = Temp;
		target = Temp2;
	}
	Vector3 view = (position - target).Normalized();
	position -= view * (float)(10.f * dt);
}

void CarAi::GeneratePath(const std::string& file_path)
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
			Position vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
			Point.push_back(vertex);
		}
	}

}

float CarAi::dot(Vector3 Fstnum, Vector3 sndNum)
{
	float x = Fstnum.x * sndNum.x;
	float z = Fstnum.z * sndNum.z;
	float num =x+z;
	return num;
}

void CarAi::init(Vector3 pos,Vector3 Target,Vector3 up)
{
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
