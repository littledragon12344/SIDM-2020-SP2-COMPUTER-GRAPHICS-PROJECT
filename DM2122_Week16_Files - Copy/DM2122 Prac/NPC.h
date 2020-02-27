#ifndef NPC_H
#define NPC_H
#include "Vertex.h"

class NPC
{
public:
	NPC();
	~NPC();

	bool IsNear(float pX, float pZ);
	void setCoords(float npcX, float npcZ);

	bool IsInteracting();
	void Interact(int a);
	void Update(double dt);
	float getX();
	float getZ();

	//for movement
	void setDirection(int i);
	float getDirection();
	void MoveToCar1(double dt, int index);
	float speed;
	float acceleration = 0;
	Vector3 newPosition;


private:
	float x, z;
	float direction;
	bool closeEnough, interacting;
};

#endif
