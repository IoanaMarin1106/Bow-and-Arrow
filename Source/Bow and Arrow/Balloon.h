#pragma once
#include <Component\SimpleScene.h>

#include "Transform2D.h"
#include "Object2D.h"

class Balloon {
	public:
		Mesh* elipse;
		Mesh* triangle;
		Mesh* polyline;

		float centerX;
		float centerY;

		Balloon(std::string triangleName, glm::vec3 corner, float length, glm::vec3 color,
				std::string balloonName, glm::vec3 center, float radius,
				std::string polylineName, float width, float height);
		Balloon();
		~Balloon();

};