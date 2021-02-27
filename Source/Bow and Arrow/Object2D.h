#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreatePolygon(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);

	// Create a triangle
	Mesh* CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	// Create the triangle for bow
	Mesh* CreateTriangleBow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);

	// Create a shuriken
	Mesh* CreateShuriken(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	// Create a bow
	Mesh* CreateBow(std::string name, glm::vec3 bowCenter, float radius, glm::vec3 color, bool fill = false);

	// Create a balloon
	Mesh* CreateBalloon(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill);

	//Create a polyline
	Mesh* CreatePolyline(std::string name, glm::vec3 bottomPoint, float width, float height, glm::vec3 color);

	//Create an arrow
	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, float width, float height, float length, glm::vec3 color, bool fill);
}

