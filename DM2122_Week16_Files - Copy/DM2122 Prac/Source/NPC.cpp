#include "NPC.h"

NPC::NPC()
{
	x = 0.0f, z = 0.0f, interacting = false, closeEnough = false; direction = 0;
}

NPC::~NPC()
{

}

bool NPC::IsNear(float pX, float pZ)
{
	if (pX >= x * 5.0f + 50.0f && pX <= x * 5.0f - 50.0f)
	{
		if (pZ >= z * 5.0f + 50.0f && pZ <= z * 5.0f - 50.0f)
		{
			closeEnough = true;
			return true;
		}
	}

	closeEnough = false;
	return false;
}

bool NPC::IsInteracting()
{
	return interacting;
}

void NPC::Interact(int a)
{
	switch (a)
	{
	case 0: interacting = false;
		break;
	case 1: interacting = true;
		break;
	}
}

void NPC::setCoords(float npcX, float npcZ)
{

	x = npcX; z = npcZ;

}

void NPC::setDirection(int i)
{
	direction = i;
}

float NPC::getX()

{
	return x;
}

float NPC::getZ()

{
	return z;
}

float NPC::getDirection()
{
	return direction;
}

void NPC::MoveToCar1(double dt, int index)
{
	switch(index)
	{
	case 1:
		x -= (float)(dt);
		z -= (float)(dt);
	}
}