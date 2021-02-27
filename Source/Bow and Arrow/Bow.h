#pragma once
#include <Component\SimpleScene.h>
#include "Transform2D.h"
#include "Object2D.h"

class Bow {
	public:
		Mesh* halfCircle;
		Mesh* bowRectangle;

		float centerX;
		float centerY;
		float bowX;

		Bow();
		~Bow();
		Bow(std::string halfCircleName, glm::vec3 bowCenter, float radius, glm::vec3 color,
			std::string bowRectangleName, glm::vec3 cornerRect, float bowRectangleWidth, float bowRectangleHeight);
};