#include "Wall.h"
#include <fstream>

std::vector<Wall*> Wall::allWalls;

Wall::Wall(Vector3 pos, Vector3 nml, float len, float hgt, float dpt)
	: position(pos), normal(nml), length(len), height(hgt), depth(dpt) {  }

// Create walls using code
void Wall::createWall(Vector3 pos, Vector3 nml, float len, float hgt, float dpt)
{
	allWalls.push_back(new Wall(pos, nml.Normalize(), len, hgt, dpt));
}

// Generates walls from an obj file.
// Only works if:
// - walls has no rotation in the x and z axis.
// - walls in the obj files have "wall" or "Wall" as the first part of the name
// - walls are cuboid
bool Wall::generateWalls(const char* file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Unable to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}

	std::vector<Vector3> temp_vertices;
	std::vector<Vector3> temp_normals;

	while (!fileStream.eof())
	{
		char buf[256];
		fileStream.getline(buf, 256);

		if (strncmp("v ", buf, 2) == 0)
		{
			Vector3 vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strncmp("vn ", buf, 3) == 0)
		{
			Vector3 normal;
			sscanf_s((buf + 2), "%f%f%f", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		// if it has "g Wall" or "g wall" in the buffer, it means that the vertices and normals in temp_vertices and temp_normals are for walls.
		else if (std::string(buf).find("Wall") != std::string::npos || std::string(buf).find("wall") != std::string::npos)
		{
			// Get the middle position of the wall
			Vector3 position;

			// Vectors to help find out the length, height, width
			// Note: it can be pointing up / down, right / left
			Vector3 up, front, right;

			float length = -1;
			float height = -1;
			float depth = -1;

			// Get up and right vector
			for (int i = 0; i < temp_normals.size(); ++i)
			{
				if (temp_normals[i].y != 0)
					up = temp_normals[i].Normalized();
				else
					front = temp_normals[i].Normalized();

				if (up != Vector3(0, 0, 0) && front != Vector3(0, 0, 0))
					break;
			}

			// Skip to the next object if up is not set in the loop above as it would mean that the shape is rotated in either x or z axis.
			if (up == Vector3(0, 0, 0))
			{
				std::cout << "Obj rotated in x or z axis";
				continue;
			}

			// Don't need to find front vector as it should be up cross product with right.
			right = up.Cross(front).Normalized();

			for (int i = 0; i < temp_vertices.size(); ++i)
			{
				// Add up all vertices and divide it by the size to find average.
				position += temp_vertices[i];

				// If either length, height or depth haven't been set,
				if (length < 0 || height < 0 || depth < 0)
				{
					Vector3 tmp;

					// Getting the length, height and depth
					for (int index = 0; index < temp_vertices.size(); ++index)
					{
						if (temp_vertices[i] == temp_vertices[index])
							continue;
						tmp = temp_vertices[i] - temp_vertices[index];
						// Round off to 5.d.p because it may have some accuracy errors like floating point error.
						float tmpLength = (roundf(tmp.Length() * 10000) / 10000);
						// If tmp is parallel to up, set height to tmp's length
						if (height < 0 && (roundf(abs(tmp.Dot(up)) * 10000) / 10000) == tmpLength)
							height = tmpLength;
						// If tmp is parallel to right, set length to tmp's length
						else if (length < 0 && (roundf(abs(tmp.Dot(right)) * 10000) / 10000) == tmpLength)
							length = tmpLength;
						// Else, it would be parallel to front
						// So set depth to tmp's length
						else if (depth < 0 && (roundf(abs(tmp.Dot(front)) * 10000) / 10000) == tmpLength)
							depth = tmpLength;
					}
				}
			}
			// Average position to get the center position of the wall
			position *= 1.f / temp_vertices.size();

			allWalls.push_back(new Wall(position, front, length, height, depth));
			temp_vertices.clear();
			temp_normals.clear();
		}
		// Make sure temp_vertices and temp_normals are empty
		else if (strncmp("g default", buf, 9) == 0)
		{
			temp_vertices.clear();
			temp_normals.clear();
		}
	}

	fileStream.close();
}

Vector3 Wall::getNormal() { return normal; }

Vector3 Wall::getPosition() { return position; }

float Wall::getLength() { return length; }

float Wall::getHeight() { return height; }

float Wall::getDepth() { return depth; }

void Wall::deleteWalls()
{
	for (int i = 0; i < allWalls.size(); ++i)
		delete allWalls[i];
	allWalls.clear();
}

int Wall::getNumOfWall() { return allWalls.size(); }

Wall* Wall::getWall(int index)
{
	if (index < allWalls.size()) // Check if it is out of range
		return allWalls[index];
	else
		return nullptr;
}

// Rotation of the wall normal in the xz plane
float Wall::getWallNormalRotation()
{
	if (normal.x < 0)
		return atan(normal.z / normal.x) * 180.0 / 3.1415926535f;
	else
		return 180 + atan(normal.z / normal.x) * 180.0 / 3.1415926535f;
}

// For player and wall collision detection in xz plane / top-down view
// Paramenters:
//		Position:		current position of the player
//		Displacement:	displacement of where the player will move if there's no wall
// Returns: 
//		Vector3, displacement of the point after it hits the wall
Vector3 Wall::playerWallCollision(Vector3 position, Vector3 displacement)
{
	int numWallsCollided = 0;
	Vector3 nextPos = position + displacement;
	Vector3 finalPosDisplacement = displacement; // If never collide with any walls.

	for (int wallIndex = 0; wallIndex < allWalls.size(); ++wallIndex)
	{
		Wall wall = *allWalls[wallIndex]; // Get the current wall in the loop for easier access.

		float maxWallPlayerDepthDist = wall.depth / 2.f + 2.f;
		float maxWallPlayerLengthDist = wall.length / 2.f + 2.f;

		// Distance of the nextPos to the wall's center in the wall's normal direction
		float perpendicularDist = nextPos.Dot(wall.normal) - wall.position.Dot(wall.normal);

		// Vector parallel to the wall / 90 degrees clockwise of the normal
		Vector3 wallPara;
		// Simple clockwise 90 degrees rotation of the normal / right vector of wall
		wallPara.x = wall.normal.z;
		wallPara.y = 0;
		wallPara.z = -wall.normal.x;

		float parallelDist = nextPos.Dot(wallPara) - wall.position.Dot(wallPara);

		if (
			((abs(perpendicularDist) < maxWallPlayerDepthDist) ||  // If next position is inside the wall in the wall normal direction
			((perpendicularDist * (position.Dot(wall.normal) - wall.position.Dot(wall.normal))) < 0)) // If the point is moving very fast and it passes than the wall's depth
			&&
			((abs(parallelDist) < maxWallPlayerLengthDist) || // If next position is inside the wall in the wall parallel direction
			((parallelDist * (position.Dot(wallPara) - wall.position.Dot(wallPara))) < 0)) // If the point is moving very fast and it passes than the wall's length
			)
		{
			// If collided with 2 walls, don't move
			if (numWallsCollided == 1)
				return Vector3(0, 0, 0);
			else
				++numWallsCollided;

			// If the perpendicular distance of the current position of the object is more than the depth,
			// It is on the length (x-axis) side of the object
			// Need to round the value to 4.d.p because for rotated walls, the value might have some accuracy error like floating point errors
			if (roundf(abs(position.Dot(wall.normal) - wall.position.Dot(wall.normal)) * 1000) / 1000 >= (roundf(maxWallPlayerDepthDist) * 1000) / 1000.f)
			{
				// Flip wallPara to the other direction if the displacement is the other direction of wallPara.
				// wallPara will be 90 degrees anti-clockwise to the normal
				if (wall.normal.z * displacement.x - wall.normal.x * displacement.z < 0)
					wallPara = -wallPara;
				finalPosDisplacement = (1 - abs(displacement.Normalize().Dot(wall.normal))) * wallPara * displacement.Length();

				if (displacement.Normalize().Dot(wall.normal) < 0)
					finalPosDisplacement = finalPosDisplacement - (abs((position + finalPosDisplacement).Dot(wall.normal) - wall.position.Dot(wall.normal)) - maxWallPlayerDepthDist) * wall.normal;
				else
					finalPosDisplacement = finalPosDisplacement + (abs((position + finalPosDisplacement).Dot(wall.normal) - wall.position.Dot(wall.normal)) - maxWallPlayerDepthDist) * wall.normal;
			}
			// Else, it is on the depth (z-axis) side of the object
			else
			{
				if (displacement.Normalize().Dot(wall.normal) > 0)
					finalPosDisplacement = (abs(displacement.Normalize().Dot(wall.normal))) * wall.normal * displacement.Length();
				else
					finalPosDisplacement = -(abs(displacement.Normalize().Dot(wall.normal))) * wall.normal * displacement.Length();

				if (wall.normal.z * displacement.x - wall.normal.x * displacement.z > 0)
					finalPosDisplacement = finalPosDisplacement + (abs((position + finalPosDisplacement).Dot(wallPara) - wall.position.Dot(wallPara)) - maxWallPlayerLengthDist) * wallPara;
				else
					finalPosDisplacement = finalPosDisplacement - (abs((position + finalPosDisplacement).Dot(wallPara) - wall.position.Dot(wallPara)) - maxWallPlayerLengthDist) * wallPara;
			}
		}
	}
	return finalPosDisplacement;
}

// For car and wall collision detection in xz plane / top-down view
// Parameters:
//		Position:	position of the car
//		Fwd:		foward vector of the car
//		len:		length of the car
//		width:		width of the car
// Returns:
//		bool,	returns true if collide with any walls,
//		returns	false if never collide with any wall
// Method used: separating axis theorem (from online)
Wall* Wall::carWallCollision(Vector3 pos, Vector3 fwd, float width, float len)
{
	fwd.Normalize();
	Vector3 rectRight;
	rectRight.x = fwd.z;
	rectRight.y = 0;
	rectRight.z = -fwd.x;

	bool collided = false;

	// 0 - top left corner
	// 1 - top right corner
	// 2 - bottom left corner
	// 3 - bottom right corner
	// Corner position of rectangle relative to the world axis / position looking top down
	// Will make it faster to calculate the rectCornerPos relative to the wall
	Vector3 rectCornerWorldPos[4];
	rectCornerWorldPos[0] = pos - (len / 2.f) * rectRight + (width / 2.f) * fwd;
	rectCornerWorldPos[1] = pos + (len / 2.f) * rectRight + (width / 2.f) * fwd;
	rectCornerWorldPos[2] = pos - (len / 2.f) * rectRight - (width / 2.f) * fwd;
	rectCornerWorldPos[3] = pos + (len / 2.f) * rectRight - (width / 2.f) * fwd;

	for (int wallIndex = 0; wallIndex < allWalls.size(); ++wallIndex)
	{
		Wall wall = *allWalls[wallIndex];

		Vector3 wallRight;
		// Simple clockwise 90 degrees rotation of the normal
		wallRight.x = wall.normal.z;
		wallRight.y = 0;
		wallRight.z = -wall.normal.x;

		// Getting corner positions of rectangle and wall

		// 0 - top left corner
		// 1 - top right corner
		// 2 - bottom left corner
		// 3 - bottom right corner
		// Corner position of rectangle relative to the wall's position looking top down
		Vector3 rectCornerPos[4];
		rectCornerPos[0] = rectCornerWorldPos[0] - wall.position;
		rectCornerPos[1] = rectCornerWorldPos[1] - wall.position;
		rectCornerPos[2] = rectCornerWorldPos[2] - wall.position;
		rectCornerPos[3] = rectCornerWorldPos[3] - wall.position;

		// 0 - top left corner
		// 1 - top right corner
		// 2 - bottom left corner
		// 3 - bottom right corner
		// Corner position of rectangle relative to the rectangle's position looking top down
		Vector3 wallCornerPos[4];
		wallCornerPos[0] = (wall.position - (wall.length / 2.f) * wallRight + (wall.depth / 2.f) * wall.normal - pos);
		wallCornerPos[1] = (wall.position + (wall.length / 2.f) * wallRight + (wall.depth / 2.f) * wall.normal - pos);
		wallCornerPos[2] = (wall.position - (wall.length / 2.f) * wallRight - (wall.depth / 2.f) * wall.normal - pos);
		wallCornerPos[3] = (wall.position + (wall.length / 2.f) * wallRight - (wall.depth / 2.f) * wall.normal - pos);



		// --------------------------------
		// Checking in the wall's axis
		// --------------------------------

		// Get the largest and smallest distance of the corners to the rectangle's center / wall's center depending on which is being calculated.
		float largest, smallest;
		float tmp;

		// Set both largest and smallest to distance of top right corner of the rectangle in wallRight axis.
		largest = smallest = rectCornerPos[0].Dot(wallRight);

		// Getting largest and smallest distance of rectangle's corner position in wall right axis.
		for (int i = 1; i < 4; ++i)
		{
			tmp = rectCornerPos[i].Dot(wallRight);

			// Getting largest and smallest
			if (tmp > largest)
				largest = tmp;
			else if (tmp < smallest)
				smallest = tmp;
		}

		// If either largest or smallest are outside, of the rect's min and max position (-len / 2 and len / 2), continue to next wall
		if (largest < -wall.length / 2.f || smallest > wall.length / 2.f)
			continue; // continue to next wall

		// Do the same as above (from calculating the wallCornerPos to if statement above) but do the calculations on the rect's foward vector instead
		// Set both largest and smallest to distance of top right corner of the rectangle in wall normal axis.
		largest = smallest = rectCornerPos[0].Dot(wall.normal);

		// Getting largest and smallest distance of rectangle's corner position in wall normal axis.
		for (int i = 1; i < 4; ++i)
		{
			tmp = rectCornerPos[i].Dot(wall.normal);

			// Getting largest and smallest
			if (tmp > largest)
				largest = tmp;
			else if (tmp < smallest)
				smallest = tmp;
		}

		// If either largest or smallest are outside, of the wall's min and max position (-depth / 2 and depth / 2),
		// it means it can't have collided so go to the next wall.
		if (largest < -wall.depth / 2.f || smallest > wall.depth / 2.f)
			continue; // continue to next wall



		// --------------------------------
		// Checking in the rectangle's axis
		// --------------------------------

		// Set both largest and smallest to distance of top right corner of the wall in rectRight axis.
		largest = smallest = wallCornerPos[0].Dot(rectRight);

		// Getting largest and smallest distance of wall's corner position in rectangle right axis.
		for (int i = 1; i < 4; ++i)
		{
			tmp = wallCornerPos[i].Dot(rectRight);

			// Getting largest and smallest
			if (tmp > largest)
				largest = tmp;
			else if (tmp < smallest)
				smallest = tmp;
		}

		// If either largest or smallest are outside, of the rect's min and max position (-len / 2 and len / 2), continue to next wall
		if (largest < -len / 2.f || smallest > len / 2.f)
			continue; // continue to next wall

		// Do the same as above (from calculating the wallCornerPos to if statement above) but do the calculations on the rect's foward vector instead
		// Set both largest and smallest to distance of top right corner of the rectangle in foward axis.
		largest = smallest = wallCornerPos[0].Dot(fwd);

		// Getting largest and smallest distance of rectangle's corner position in rectangle foward axis.
		for (int i = 1; i < 4; ++i)
		{
			tmp = wallCornerPos[i].Dot(fwd);

			// Getting largest and smallest
			if (tmp > largest)
				largest = tmp;
			else if (tmp < smallest)
				smallest = tmp;
		}
		// If either largest or smallest are outside, of the rect's min and max position (-width / 2 and width / 2), continue to next wall
		if (largest < -width / 2.f || smallest > width / 2.f)
			continue; // continue to next wall

		// If it runs here, it means that it has collided with the wall in wallIndex.
		return allWalls[wallIndex];
	}

	return NULL;
}
