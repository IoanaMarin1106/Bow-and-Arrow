#include <iostream>
#include "Shuriken.h"
#include <Component/SimpleScene.h>

Shuriken::Shuriken() {

};

Shuriken::~Shuriken() {

};

Shuriken::Shuriken(std::string shurikenName, glm::vec3 corner, float shurikenSide, glm::vec3 color) {
	this->centerX = corner.x + shurikenSide;
	this->centerY = corner.y + shurikenSide;

	this->shuriken = Object2D::CreateShuriken(shurikenName, corner, shurikenSide, color, true);
};

