#include <iostream>
#include "Balloon.h"
#include <Component/SimpleScene.h>

using namespace std;

Balloon::Balloon() {

};

Balloon::~Balloon() {

};

Balloon::Balloon(std::string triangleName, glm::vec3 corner, float length, glm::vec3 color, 
				std::string elipseName, glm::vec3 center, float radius,
				std::string polylineName, float width, float height) {

	this->centerX = center.x;
	this->centerY = center.y;

	this->elipse = Object2D::CreateBalloon(elipseName, center, radius, color, false);
	this->triangle = Object2D::CreateTriangle(triangleName, corner, length, color, true);
	this->polyline = Object2D::CreatePolyline(polylineName, corner, width, height, color);
};