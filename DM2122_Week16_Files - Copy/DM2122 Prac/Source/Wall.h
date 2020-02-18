/*#pragma once
#include <vector>
#include <Mtx44.h>

class Wall
{
private:
	Vector3 position;
	Vector3 normal;
	int length, height, depth;
public:
	Wall(Vector3 pos, Vector3 nml, int len, int hgt, int depth);
	Vector3 getNormal();
	Vector3 getPosition();
	int getLength();
	int getHeight();
	int getDepth();
	float getWallNormalRotation();
	// Gets the displacement of any object after it moves. 
	// If it hits any wall, the displacement will change.
	static Vector3 dispAftCollideWithWall(Vector3 position, Vector3 displacement, std::vector<Wall>& walls);
};
*/

#pragma once
#include <vector>
#include <Mtx44.h>

class Wall
{
private:
	Vector3 position;
	Vector3 normal;
	int length, height, depth;
	// Have this function so that only need include Wall.h in other files to access all the walls. (e.g. used in camera collision detection)
	static std::vector<Wall*> allWalls;
	Wall(); // Prevent any wall object from being created using constructor.
	Wall(Vector3 pos, Vector3 nml, int len, int hgt, int depth);
public:
	static void createWall(Vector3 pos, Vector3 nml, int len, int hgt, int dpt);
	Vector3 getNormal();
	Vector3 getPosition();
	int getLength();
	int getHeight();
	int getDepth();
	static int getNumOfWall();
	static Wall* getWall(int index); // Get the wall at the index position
	float getWallNormalRotation();
	// Gets the displacement of any object after it moves. 
	// If it hits any wall, the displacement will change.
	static Vector3 dispAftCollideWithWall(Vector3 position, Vector3 displacement);
};