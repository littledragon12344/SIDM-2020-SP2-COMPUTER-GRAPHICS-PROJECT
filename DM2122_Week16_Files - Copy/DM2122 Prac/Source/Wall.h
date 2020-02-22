#pragma once
#include <vector>
#include <Mtx44.h>

class Wall
{
private:
	Vector3 position;
	Vector3 normal;
	float length, height, depth;
	// Have this function so that only need include Wall.h in other files to access all the walls. (e.g. used in camera collision detection)
	static std::vector<Wall*> allWalls;
	Wall(); // Prevent any wall object from being created using constructor.
	Wall(Vector3 pos, Vector3 nml, float len, float hgt, float depth);
public:
	static void createWall(Vector3 pos, Vector3 nml, float len, float hgt, float dpt);

	// Generates walls from an obj file.
	// Only works if:
	// - walls has no rotation in the x and z axis.
	// - walls in the obj files have "wall" or "Wall" as the first part of the name
	// - walls are cuboid
	static bool generateWalls(const char* file_path);
	Vector3 getNormal();
	Vector3 getPosition();
	float getLength();
	float getHeight();
	float getDepth();
	static int getNumOfWall();
	static Wall* getWall(int index); // Get the wall at the index position
	float getWallNormalRotation(); // Rotation of the wall normal in the xz plane


	// Collison functions:

	// For point - rectangle collision detection in xz plane
	// Position: position of the object
	// Displacement: displacement of where the object will move
	// Returns: Vector3, displacement of the point after it hits the wall
	static Vector3 dispAftCollideWithWall(Vector3 position, Vector3 displacement);
	// For rectangle - wall collision detection in xz plane
	// Position: center position of the rectangle
	// Fwd: foward direction of the rectangle
	// len: length of the rectangle
	// width: width of the rectangle
	static bool rectWallCollision(Vector3 pos, Vector3 fwd, float len, float width);
};
