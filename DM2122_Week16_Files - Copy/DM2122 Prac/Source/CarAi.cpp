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
		End = Temp2+GetRandomizePoint()*2;//Initialize End
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
		End = Temp2 + GetRandomizePoint() * 2;//end = next end
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
					Tempangle = 0;
				}
			}
			else if (rotationy < -90 && angleFromx > 90)//Check for acute
			{
				Tempangle -= rotationSpeed * dt;
				rotationy -= rotationSpeed * dt;
				if (rotationy < (-2 * 180 + angleFromx))
				{
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
						Tempangle = 0;
					}
				}
				if (rotationy > (-angleFromx + rotationy) && Tempangle != 0)
				{
					Tempangle -= rotationSpeed * dt;
					rotationy -= rotationSpeed * dt;
					if (rotationy < (-angleFromx + rotationy))
					{
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
					Tempangle = 0;
				}
			}
			if (rotationy > angleFromx && Tempangle != 0)
			{
				Tempangle -= rotationSpeed * dt;
				rotationy -= rotationSpeed * dt;
				if (rotationy <angleFromx)
				{
					Tempangle = 0;
				}
			}
		}
	}
	if (rotationy > 180)//Make rotationy to a easier balue for the code to run and for my self to know
	{
		rotationy = rotationy-360;
	}
	if (rotationy < -180)
	{
		rotationy = rotationy + 360;
	}
	if (rotationy != angleFromx)
	{
			acceration = -rotationSpeed / AiCar.GetMaxSpeed();
	}
	else
	{
		acceration = AiCar.GetAcceleration();
	}

	target = GetTargetpos();
	
	//std::cout << "\n";
	Vector3 TargetView = (target - position).Normalize();
	position += TargetView * (float)(AiCar.GetCurrentSpeed() * dt);
	if (Collided == true)
	{
		Collidewithwall(Wall::carWallCollision(position + TargetView * (float)(AiCar.GetCurrentSpeed() * dt)*2, GetTargetpos(), 7, 2.5));
	}
	//Check Collision
	if (Wall::carWallCollision(position + TargetView * (float)(AiCar.GetCurrentSpeed() * dt)*2, GetTargetpos(), 7, 2.5).size() != 0)
	{
		std::vector<Wall*> Temp = Wall::carWallCollision(position + TargetView * (float)(AiCar.GetCurrentSpeed() * dt) * 2, GetTargetpos(), 7, 2.5);
		for (int i = 0; i < Temp.size(); i++)
		{
			if (Temp[i]->getPosition() == PathToGo->Point[1]&&!Collided)
			{
				round++;
			}
		}
		previousX = position.x;
		previousZ = position.z;
		Collided = true;
	}
	else if(Wall::carWallCollision(position + TargetView * (float)(AiCar.GetCurrentSpeed() * dt)*2, GetTargetpos(), 7, 2.5).size() == 0)//Check nolonger collide with object
	{
		Collided = false;
	}
	AiCar.CalculateSpeed(acceration, AiCar.GetCurrentSpeed(), dt);//Find new speed Using CCar class
	if (AiCar.GetCurrentSpeed() > AiCar.GetMaxSpeed())
	{
		AiCar.SetCurrentSpeed(AiCar.GetMaxSpeed());
	}
	if (AiCar.GetCurrentSpeed() <0 )
	{
		AiCar.SetCurrentSpeed(0);
		acceration = 1;
	}
//	target = GetTargetpos();
}
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

void CarAi::init(Vector3 pos,Vector3 Target,Vector3 up,float RotateSpeed, Path *paths)//Initiallizing default variable
{
	PathToGo = paths;
	start = 0;
	round = 0;
	rotationy = 0;
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
	randCar = rand() % CCar::AllCar.size();
	AiCar = *CCar::AllCar[randCar];
	acceration = AiCar.GetAcceleration();
}

Vector3 CarAi::GetTargetpos()//Get Target Posiiton using the angles
{
	Vector3 Temp;
	Vector3 xDir = Vector3(1, 0, 0);
	//Since we only rotaing x and y
	Temp.x = cos(Math::DegreeToRadian(-rotationy)) * xDir.x - sin(Math::DegreeToRadian(-rotationy)) * xDir.z;
	Temp.z = sin(Math::DegreeToRadian(-rotationy)) * xDir.x + cos(Math::DegreeToRadian(-rotationy)) * xDir.z;
	Temp.y = 0;
	TargetFromPos = Temp;
	Temp = position + Temp;
	return Temp;
}

Vector3 CarAi::GetPosition()//get private variable
{
	return position;
}

float CarAi::GetRotation()//get private rotation
{
	return rotationy;
}

Vector3 CarAi::GetRandomizePoint()//Randomize the path vertices so that thr will be a slight difference int path each time the carai go around
{
	Vector3 Temp;
	Vector3 xDir = Vector3(1, 0, 0);
	int postochange = rand() % 360;
	if (postochange > 180)
	{
		postochange = postochange-180;
	}
	else
	{
		postochange = -postochange;
	}
	Temp.x = cos(Math::DegreeToRadian(-postochange)) * xDir.x - sin(Math::DegreeToRadian(-postochange)) * xDir.z;
	Temp.z = sin(Math::DegreeToRadian(-postochange)) * xDir.x + cos(Math::DegreeToRadian(-postochange)) * xDir.z;
	Temp.y = 0;
	return Temp;
}

Vector3 CarAi::Getforward()//Get front direction
{
	return TargetFromPos;
}

void CarAi::Collidewithwall(std::vector<Wall*> wallcollide)//Collision out come
{
	for (int i = 0; i < wallcollide.size(); i++)
	{
		if (wallcollide[i]->getPosition() == PathToGo->Point[1])
		{
		}
		else//Collision out come with the longest side of the wall
		{
			acceration =-AiCar.GetCurrentSpeed();
			if (wallcollide[i]->getLength() > wallcollide[i]->getDepth())
			{
				position.z = previousZ;
			}
			if (wallcollide[i]->getLength() < wallcollide[i]->getDepth())
			{
				position.x = previousX;
			}
		}
	}
}
