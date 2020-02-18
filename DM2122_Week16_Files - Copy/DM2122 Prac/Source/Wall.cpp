
#include "Wall.h"

std::vector<Wall*> Wall::allWalls;

Wall::Wall(Vector3 pos, Vector3 nml, int len, int hgt, int dpt) : position(pos), normal(nml), length(len), height(hgt), depth(dpt) {  }

void Wall::createWall(Vector3 pos, Vector3 nml, int len, int hgt, int dpt)
{
	allWalls.push_back(new Wall(pos, nml.Normalize(), len, hgt, dpt));
}

Vector3 Wall::getNormal() { return normal; }

Vector3 Wall::getPosition() { return position; }

int Wall::getLength() { return length; }

int Wall::getHeight() { return height; }

int Wall::getDepth() { return depth; }

int Wall::getNumOfWall() { return allWalls.size(); }

Wall* Wall::getWall(int index)
{
	if (index < allWalls.size()) // Check if it is out of range
		return allWalls[index];
	else
		return nullptr;
}

// Rotation of the wall in the xz plane
float Wall::getWallNormalRotation()
{
	if (normal.x < 0)
		return atan(normal.z / normal.x) * 180.0 / 3.1415926535f;
	else
		return 180 + atan(normal.z / normal.x) * 180.0 / 3.1415926535f;
}

// For collision detection
// Gets the displacement of an object after it collides with a wall
// Position: position of the object
// Displacement: displacement of where the object will move
Vector3 Wall::dispAftCollideWithWall(Vector3 position, Vector3 displacement)
{
	int numWallsCollided = 0;
	Vector3 tmp;
	Vector3 nextPos = position + displacement;
	Vector3 finalPosDisplacement = displacement; // If never collide with any walls.
	for (int wallIndex = 0; wallIndex < allWalls.size(); ++wallIndex)
	{
		Wall wall = *allWalls[wallIndex]; // Get the current wall in the loop for easier access.

		// Perpendicular distance of position to wall
		// Finding distance of a point (nextPos) to a plane(wall).
		float perpendicularDist = abs(nextPos.Dot(wall.getNormal()) - wall.getPosition().Dot(wall.getNormal()));
		tmp = wall.getPosition();
		tmp.y = nextPos.y; // have the wall.y position be the same as nextPos.y
		// squared distance of the point from the center of the wall.
		// if never square, need sqrt which is expensive
		float parallelDistSquared = pow((tmp - nextPos).Length(), 2) - pow(perpendicularDist, 2);
		if (perpendicularDist < wall.depth / 2.f && parallelDistSquared < pow((wall.getLength() / 2.0f), 2))
		{
			// If collided with 2 walls, don't move
			if (numWallsCollided == 1)
				return Vector3(0, 0, 0);
			else
				++numWallsCollided;

			// Vector parallel to the wall / 90 degrees clockwise of the normal
			Vector3 wallPara;
			// Simple clockwise 90 degrees rotation of the normal
			wallPara.x = wall.normal.z;
			wallPara.y = 0;
			wallPara.z = wall.normal.x * -1;
			// If the perpendicular distance of the current position of the object is more than the depth,
			// It is on the length (x-axis) side of the object
			// Need to round the value to 5.d.p because for walls that are not 90 degrees to the x- or z-axis, the value is not accurate.
			if (roundf(abs(position.Dot(wall.normal) - wall.position.Dot(wall.normal)) * 10000) / 10000 >= wall.depth / 2.f)
			{
				// Flip wallPara to the other direction if the displacement is the other direction of wallPara.
				// wallPara will be 90 degrees anti-clockwise to the normal
				if (wall.normal.z * displacement.x - wall.normal.x * displacement.z < 0)
					wallPara *= -1;
				finalPosDisplacement = (1 - abs(displacement.Normalize().Dot(wall.normal))) * wallPara * displacement.Length();

				if (displacement.Normalize().Dot(wall.normal) < 0)
					finalPosDisplacement = finalPosDisplacement - (abs((position + finalPosDisplacement).Dot(wall.normal) - wall.position.Dot(wall.normal)) - wall.depth / 2.f) * wall.normal;
				else
					finalPosDisplacement = finalPosDisplacement + (abs((position + finalPosDisplacement).Dot(wall.normal) - wall.position.Dot(wall.normal)) - wall.depth / 2.0) * wall.normal;
			}
			// Else, it is on the depth (z-axis) side of the object
			else
			{
				if (displacement.Normalize().Dot(wall.normal) > 0)
					finalPosDisplacement = (abs(displacement.Normalize().Dot(wall.normal))) * wall.normal * displacement.Length();
				else
					finalPosDisplacement = -(abs(displacement.Normalize().Dot(wall.normal))) * wall.normal * displacement.Length();

				if (wall.normal.z * displacement.x - wall.normal.x * displacement.z > 0)
					finalPosDisplacement = finalPosDisplacement + (abs((position + finalPosDisplacement).Dot(wallPara) - wall.position.Dot(wallPara)) - wall.length / 2.0) * wallPara;
				else
					finalPosDisplacement = finalPosDisplacement - (abs((position + finalPosDisplacement).Dot(wallPara) - wall.position.Dot(wallPara)) - wall.length / 2.0) * wallPara;
			}
		}
	}

	return finalPosDisplacement;
}
