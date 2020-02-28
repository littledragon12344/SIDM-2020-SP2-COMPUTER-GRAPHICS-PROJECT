#include "Path.h"
//PAth only works if the vertices is in the right order best to start with 2d triangle and increase the amount of vertices on it using multicutTool
void Path::GeneratePath(const std::string& file_path, float scale, Vector3 Offset)//get points on the obj
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Unable to open " << file_path << ". Are you in the right directory ?\n";
		return;
	}
	while (!fileStream.eof())
	{
		char buf[256];
		fileStream.getline(buf, 256);
		if (strncmp("v ", buf, 2) == 0)
		{
			Vector3 vertex;
			sscanf_s((buf + 2), "%f%f%f", &vertex.x, &vertex.y, &vertex.z);
			vertex = vertex * scale;
			vertex += Offset;
			Point.push_back(vertex);
		}
	}
	fileStream.close();
}
