#ifndef PATH_H
#define PATH_H

//#include "Vector3.h"
//#include <vector>
//#include <MyMath.h>
#include "LoadOBJ.h"
#include <fstream>
#include <map>
#include "Mtx44.h"

class Path
{
private:
public:
	std::vector<Vector3> Point;
	void GeneratePath(const std::string& file_path, float scale, Vector3 Offset);
};

#endif