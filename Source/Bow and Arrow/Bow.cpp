#include <iostream>
#include "Bow.h"

using namespace std;

Bow::Bow() {
};

Bow::~Bow() {
};

Bow::Bow(std::string halfCircleName, glm::vec3 bowCenter, float radius, glm::vec3 color,
		std::string bowRectangleName, glm::vec3 cornerRect, float bowRectangleWidth, float bowRectangleHeight) {

	centerX = bowCenter.x;
	centerY = bowCenter.y;

	this->halfCircle = Object2D::CreateBow(halfCircleName, bowCenter, radius, color, false);
	this->bowRectangle = Object2D::CreatePolygon(bowRectangleName, cornerRect, bowRectangleWidth, bowRectangleHeight, color, true);
};


