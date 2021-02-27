#include "Object2D.h"

#include <Core/Engine.h>
#include <iostream>

Mesh* Object2D::CreatePolygon(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(2 * length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color)
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2};

	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateTriangleBow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, 2 * length, 0), color)
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };

	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateShuriken(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color), //0
		VertexFormat(corner + glm::vec3(length / 2, length / 2, 0), color), //1
		VertexFormat(corner + glm::vec3(0, length / 2, 0), color), //2
		VertexFormat(corner + glm::vec3(length / 2, 0, 0), color), //3
		VertexFormat(corner + glm::vec3(length, 0, 0), color), //4
		VertexFormat(corner + glm::vec3(length, length / 2, 0), color), //5
		VertexFormat(corner + glm::vec3(length, length, 0), color), //6
		VertexFormat(corner + glm::vec3(length/2, length, 0), color), //7
		VertexFormat(corner + glm::vec3(0, length, 0), color) //8
	};

	Mesh* shuriken = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2,
											3, 4, 1,
											5, 6, 1,
											7, 8, 1};

	if (!fill) {
		shuriken->SetDrawMode(GL_TRIANGLES);
	}
	
	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

Mesh* Object2D::CreateBow(std::string name, glm::vec3 bowCenter, float radius, glm::vec3 color, bool fill) {
	glm::vec3 center = bowCenter;
	int points = 150;

	std::vector<VertexFormat> vertices = {};
	//vertices.push_back(VertexFormat(center, color));

	float x = 0, y = 0;
	float x2, y2;

	for (int i = 0; i < points; i++) {
		x = radius * cos(i * M_PI / points);
		y = radius * sin(i * M_PI / points);

		x2 = 11 * cos(i * M_PI / points);
		y2 = 11 * sin(i * M_PI / points);

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(x2, y2, 0), color));
	}

	Mesh* bow = new Mesh(name);

	std::vector<unsigned short> indices = {};
	for (int i = 0; i <= 2*points + 1; i++) {
		indices.push_back(i);
	}

	indices.push_back(1);

	if (!fill) {
		bow->SetDrawMode(GL_TRIANGLE_STRIP);
	}

	bow->InitFromData(vertices, indices);
	return bow;
}

Mesh* Object2D::CreateBalloon(std::string name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;
	int points = 100;

	std::vector<VertexFormat> vertices = {};

	float x = 0, y = 0;

	for (float i = -points ; i < points; i++) {
		x = radius * cos(i * M_PI / points);
		y = radius * sin(i * M_PI / points);

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
	}

	Mesh* bow = new Mesh(name);

	std::vector<unsigned short> indices = {};
	for (int i = 0; i < 2*points; i++) {
		indices.push_back(i);
	}

	indices.push_back(1);

	if (!fill) {
		bow->SetDrawMode(GL_TRIANGLE_FAN);
	}
	
	bow->InitFromData(vertices, indices);
	return bow;
}

Mesh* Object2D::CreatePolyline(std::string name, glm::vec3 bottomPoint, float width, float height, glm::vec3 color) {
	glm::vec3 bottom = bottomPoint;
	
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(bottom, color),
		VertexFormat(bottom + glm::vec3(width, height, 0), color),
		VertexFormat(bottom + glm::vec3(0, 2 * height, 0), color),
		VertexFormat(bottom + glm::vec3(width, 3 * height, 0), color),
		VertexFormat(bottom + glm::vec3(0, 4 * height, 0), color),
		VertexFormat(bottom + glm::vec3(width, 5 * height, 0), color)
	};

	Mesh* polyline = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 5 };

	polyline->SetDrawMode(GL_LINE_STRIP);

	polyline->InitFromData(vertices, indices);
	return polyline;
}


Mesh* Object2D::CreateArrow(std::string name, glm::vec3 leftBottomCorner, float width, float height, float length, glm::vec3 color, bool fill) {
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color), //0
		VertexFormat(corner + glm::vec3(width, 0, 0), color),//1
		VertexFormat(glm::vec3(width, -length, 0), color), //2
		VertexFormat(glm::vec3(width + length, 0, 0), color),//3
		VertexFormat(glm::vec3(width, length, 0), color), //4
		VertexFormat(corner + glm::vec3(width, height, 0), color),//5
		VertexFormat(corner + glm::vec3(0, height, 0), color)//6
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 6,
											6, 1, 5, 
											2, 3, 4 };

	if (!fill) {
		triangle->SetDrawMode(GL_TRIANGLES);
	}
	else {
		indices.push_back(0);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}