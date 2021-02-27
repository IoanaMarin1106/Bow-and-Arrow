#pragma once
#include <Component\SimpleScene.h>
#include "Transform2D.h"
#include "Object2D.h"

class Shuriken {
	public:
		Mesh* shuriken;

		float centerX;
		float centerY;


		Shuriken();
		~Shuriken();
		Shuriken(std::string shurikenName, glm::vec3 corner, float shurikenSide, glm::vec3 color);
};
