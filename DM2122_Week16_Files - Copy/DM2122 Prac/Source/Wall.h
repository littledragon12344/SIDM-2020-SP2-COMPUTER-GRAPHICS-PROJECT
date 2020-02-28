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
	static void deleteWalls(); // Deletes all walls
	static int getNumOfWall();	
	static Wall* getWall(int index); // Get the wall at the index position
	float getWallNormalRotation(); // Rotation of the wall normal in the xz plane


	// Collison functions:

	// For player and wall collision detection in xz plane / top-down view
	// Paramenters:
	//		Position:		current position of the player
	//		Displacement:	displacement of where the player will move if there's no wall
	// Returns: 
	//		Vector3, displacement of the point after it hits the wall
	static Vector3 playerWallCollision(Vector3 position, Vector3 displacement);
	// For car and wall collision detection in xz plane / top-down view
	// Parameters:
	//		Position:	position of the car
	//		Fwd:		foward vector of the car
	//		len:		length of the car
	//		width:		width of the car
	// Returns:
	//		bool,	returns true if collide with any walls,
	//		returns	false if never collide with any wall
	static Wall* carWallCollision(Vector3 pos, Vector3 fwd, float len, float width);
};
