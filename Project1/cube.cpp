#include "cube.h"




vector<glm::vec3> Create_Cube_Vertices(float width, float height, float depth){
	width /= 2;
	height /= 2;
	depth /= 2;

	vector<glm::vec3> vertices{
	// front
	glm::vec3(-width, -height, depth),
	glm::vec3(width, -height, depth),
	glm::vec3(width, height, depth),
	glm::vec3(-width, height, depth),
	//back
	glm::vec3(-width, -height, -depth),
	glm::vec3(width, -height, -depth),
	glm::vec3(width, height, -depth),
	glm::vec3(-width, height, -depth)
	};

	return vertices;

}