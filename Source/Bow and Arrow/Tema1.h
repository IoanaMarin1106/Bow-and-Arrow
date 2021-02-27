#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include "Bow.h"
#include "Balloon.h"
#include "Shuriken.h"

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		glm::mat3 modelMatrix;
		float translateX, translateY;
		float scaleX, scaleY;
		float angularStep;

		Bow* bow;
		Mesh* powerBar;
		Mesh* life;
		Mesh* bottom;
		Balloon* redBalloon;
		Balloon* yellowBalloon;
		Shuriken* shuriken;

		// Miscare shuriken
		float shurikenSpeed = 100;
		float rotateShuriken;
		float shurikenHalfSide = 25;
		float intervalShuriken;
		std::vector<glm::vec3> coordShuriken;

		// Miscare baloane
		std::vector<glm::vec3> coordYellowBalloons;
		std::vector<glm::vec3> coordRedBalloons;
		float balloonsSpeed = 100;
		float intervalYellowBalloons;
		float intervalRedBalloons;

		// Vectori de coordonate pentru baloane si pentru shurikene.
		std::vector<Balloon*> ballons[500];
		std::vector<Shuriken*> shurikens[500];

		// ====================  Miscare arc =========================

		//Coordonate initiale arc (x va ramane neschimbat)
		float bowY = 400;
		float bowX = 50;

		//coordonate in timp ce zboara
		float arrowMoveX;
		float arrowMoveY;

		float bowSpeed = 100;
		float bowRotation;
		float bowTranslateY;
		float bowTranslateX = 140;

		// Power bar
		float buttonMouseHold = 0;

		//Mouse move
		int coordMouseX;
		int coordMouseY;

		// Scor + Viata
		int score = 0;
		int lifes = 3;

		//Moduri ale jocului
		bool isEndGame = false;
		bool isPaused = false;

		float radians = M_PI / 2;

		float bowRectWidth = 8;
		float bowRectHeight = 100;

		float polygonWidth = 100;
		float polygonHeight = 8;

		float triangleSide = 8 + (polygonHeight / 2);

		float bowPolygonWidth = 100;

		float powerBarWidth = 80;
		float powerBarHeight = 20;

		float coordRectX = 0;
		float coordRectY = (-polygonHeight / 2);

		bool isMouseButtonRelease = false;
		float directionAngle;

		float xMoveArrow = 0;
		float yMoveArrow = 0;

		float bottomHeight = 40;

		//Pentru zborul sagetii

		bool isFlyingArrow = false;
		bool isOutOfGame = false;
		float arrowPower = 0;

		//Pentru vieti
		float lifeWidth = 12;
		float lifeHeight = 30;

		// initializam raza arcului
		float radius = 10;

		// Latura shurikenului / 2
		float squareSide = 100;

		//coordonate varf sageata
		float coordArrowX;
		float coordArrowY;

		glm::vec3 cornerRect;

		// Distanta la care vom randa fiecare viata pe ecran
		float distanceLifes;
	
};
