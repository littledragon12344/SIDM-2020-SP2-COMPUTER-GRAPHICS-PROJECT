#ifndef PATH_BUILDER_H
#define PATH_BUILDER_H
#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include <MyMath.h>
#include "LoadOBJ.h"
#include "Mesh.h"
class PathBuilder
{
public:
	static std::vector<Position> Point_position;
	static void GenerateOBJLine(const std::string& meshname, const std::string& file_path);
};

#endif