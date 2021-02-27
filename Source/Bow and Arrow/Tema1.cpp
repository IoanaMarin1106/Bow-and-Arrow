#include "Tema1.h"
#include "Math.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}


void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);

	// Dimensiune polilinie
	float width = 20;
	float height = 15;

	// compute coordinates of circle center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;

	//=================================Initializare inceput joc==============================
	cout << "============================= >>>> BOW AND ARROW GAME <<<====================" << endl;
	cout << "BALOANE ROSII: +15p / balon" << endl;
	cout << "BALOANE GALBENE: -10p / balon" << endl;
	cout << "SHURIKEN: +20p / shuriken" << endl << endl;
	cout << "Coliziunea arcului cu un shuriken produce scaderea unei vieti." << endl << endl;
	cout << "Scor initial: " << score << endl;
	cout << "Vieti ramase: " << lifes << endl;

	// =================== CREARE ARC ===================================================================

	// Centrul cercului pentru arc
	glm::vec3 bowCenter = glm::vec3(0, 0, 0);
	cornerRect = glm::vec3(0, -polygonHeight/2, 0);
	glm::vec3 cornerTriangle = glm::vec3(polygonWidth, -triangleSide, 0);

	// ==================================== CREARE ARC ============================================================
	bow = new Bow("halfCircle", bowCenter, radius, glm::vec3(0.3f, 0.75f, 0.9f),
				"bowRectangle", bowCenter, bowRectWidth, bowRectHeight);
	AddMeshToList(bow->halfCircle);
	AddMeshToList(bow->bowRectangle);
	bow->bowX = 0;

	Mesh* arrow = Object2D::CreateArrow("arrow", cornerRect, polygonWidth, polygonHeight, triangleSide, glm::vec3(1, 0, 1), true);
	AddMeshToList(arrow);

	//======================================CREARE SHURIKEN =======================================================

	shuriken = new Shuriken("shuriken", corner, squareSide / 2, glm::vec3(0.4f, 0.5f, 1));
	AddMeshToList(shuriken->shuriken);

	// =========================================== CREARE BALOANE ==================================================

	// Galbene
	yellowBalloon = new Balloon("yellowTriangle", corner, triangleSide, glm::vec3(1, 1, 0),
										"yellowBalloon", bowCenter, radius,
										"yellowPolyline", width, height);
	AddMeshToList(yellowBalloon->elipse);
	AddMeshToList(yellowBalloon->polyline);
	AddMeshToList(yellowBalloon->triangle);

	// Rosii
	redBalloon = new Balloon("redTriangle", corner, triangleSide, glm::vec3(1, 0, 0),
										"redBalloon", bowCenter, radius,
										"redPolyline", width, height);
	AddMeshToList(redBalloon->elipse);
	AddMeshToList(redBalloon->triangle);
	AddMeshToList(redBalloon->polyline);

	//=========================Initializare Power Bar ============================
	powerBar = Object2D::CreatePolygon("powerBar", corner, powerBarWidth, powerBarHeight, glm::vec3(0.6f, 0.5f, 1), true);
	AddMeshToList(powerBar);

	//========================Initializare margine jos ==================================
	bottom = Object2D::CreatePolygon("bottom", corner, window->GetResolution().x, bottomHeight, glm::vec3(1, 0.8f, 0.7f), true);
	AddMeshToList(bottom);

	//======================= Initializare viata=====================================
	life = Object2D::CreatePolygon("life", corner, lifeWidth, lifeHeight, glm::vec3(0, 1, 0), true);
	AddMeshToList(life);

	//======================== Initializare vector shuriken =======================
	coordShuriken = {};

	//======================== Initializare vector baloane =======================
	coordYellowBalloons = {};
	coordRedBalloons = {};
	
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(1, 0.7f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

// Functie pentru a determina coliziunea dintre doua cercuri.
// Functia este folosita pentru toate tipurile de coliziuni, deoarece putem considera un punct
// ca fiind un cerc de raza zero.
bool isCollision(float radiusCircle1, float radiusCircle2, glm::vec3 centerCircle1, glm::vec3 centerCircle2) {

	float dx = centerCircle1.x - centerCircle2.x;
	float dy = centerCircle1.y - centerCircle2.y;

	// Determinam distanta dintre centrele celor doua cercuri.
	float distance = sqrt(dx * dx + dy * dy);

	// Daca distanta determinata anterior este mai mica decat suma razelor atunci
	// inseamna ca avem o coliziune.
	if (distance < radiusCircle1 + radiusCircle2) {
		return true;
	}
	return false;

}

void Tema1::Update(float deltaTimeSeconds)
{

	// Ne asiguram ca nu este jocul in modul "pauza" adica daca nu am apasat tasta P.
	if (isPaused == false) {
		//=============================== Sageata ==========================================

		// Unghiul sub care se misca sageata urmarind cursorul
		bowRotation = atan2(coordMouseY - bowY, coordMouseX - bowX);

		// Se misca sageata pe arc
		if (isMouseButtonRelease == false && isFlyingArrow == false) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(bowX, bowY);
		}
		else if (isMouseButtonRelease == true) {

			isMouseButtonRelease = false;
			isFlyingArrow = true;

			//Unghiul sub care se va misca sageata este fix, ramane acelasi sub care a plecat
			directionAngle = bowRotation;

			//Calculam cu cat se va translata sageata pe traiectoria respectiva
			xMoveArrow = cos(directionAngle) * deltaTimeSeconds * 100;
			yMoveArrow = sin(directionAngle) * deltaTimeSeconds * 100;

			// Retinem coordonatele initiale ale sagetii, de unde a plecat de pe arc.
			arrowMoveX = bowX;
			arrowMoveY = bowY;
		}

		// Daca sageata este lansata
		if (isFlyingArrow == true) {
			arrowMoveX += (xMoveArrow * arrowPower);
			arrowMoveY += (yMoveArrow * arrowPower);

			// Se mentine unghiul sub care a fost lansata
			bowRotation = directionAngle;

			//Daca cu noile coordonate, sageata iese din ecran si va reveni pe arc
			if (arrowMoveX > window->GetResolution().x || arrowMoveX < 0 ||
				arrowMoveY > window->GetResolution().y || arrowMoveY < 0) {

				isFlyingArrow = false;

				// Resetam coordonatele sagetii care va reveni pe arc
				arrowMoveX = 0;
				arrowMoveY = 0;

				// Resetam puterea arcului la zero.
				arrowPower = 0;
			}
		}
		else {
			xMoveArrow = 0;
			yMoveArrow = 0;
		}

		//Coordonatele varfului sagetii
		coordArrowX = (polygonWidth + triangleSide) * cos(bowRotation) + arrowMoveX;
		coordArrowY = (polygonWidth + triangleSide) * sin(bowRotation) + arrowMoveY;
		glm::vec3 coordTipArrow = glm::vec3(coordArrowX, coordArrowY, 0);

		modelMatrix *= Transform2D::Translate(arrowMoveX, arrowMoveY);
		modelMatrix *= Transform2D::Rotate(bowRotation);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);
		//=============================== Arc ===============================================

		radians = -M_PI / 2;

		modelMatrix = glm::mat3(1);
		bow->centerX = bowX;
		bow->centerY = bowY;

		// Randare semicerc din care este format arcul
		modelMatrix *= Transform2D::Translate(bowX, bowY);
		modelMatrix *= Transform2D::Rotate(bowRotation);
		modelMatrix *= Transform2D::Rotate(radians);
		modelMatrix *= Transform2D::Scale(5, 5);
		RenderMesh2D(meshes["halfCircle"], shaders["VertexColor"], modelMatrix);

		//Randare dreptunghiul din care este format arcul
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bowX, bowY - 50);
		modelMatrix *= Transform2D::Translate(0, bowRectHeight / 2);
		modelMatrix *= Transform2D::Rotate(bowRotation);
		modelMatrix *= Transform2D::Translate(0, -bowRectHeight / 2);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		//================================ Shuriken ==========================================
		rotateShuriken += deltaTimeSeconds;

		// Intervalul la care apare pe ecran un shuriken
		intervalShuriken += deltaTimeSeconds;

		// Apare fiecare shuriken in plus pe ecran la fiecare 2 secunde
		if (intervalShuriken > 2) {
			intervalShuriken = 0;
			coordShuriken.push_back(glm::vec3(1230, rand() % 580 + 50, 0));
		}

		// Parcurgem vectorul de coordonate ale shurikenelor pentru a le randa pe ecran
		// si a verifica coliziunile
		for (int i = 0; i < coordShuriken.size(); i++) {

			// Variem coordonata pe axa Ox a fiecarui shuriken
			coordShuriken[i].x -= deltaTimeSeconds * shurikenSpeed;

			// Retinem centrul fiecarui shuriken
			glm::vec3 centerShuriken = glm::vec3(coordShuriken[i].x + shurikenHalfSide, coordShuriken[i].y + shurikenHalfSide, 0);

			// Exista coliziune dintre sageata si shuriken asa ca setam a 3a coordonata (care este nefolosita) cu valoarea 2
			if (isCollision(shurikenHalfSide, 0, centerShuriken, coordTipArrow) == true && coordShuriken[i].z == 0) {
				coordShuriken[i].z = 2;
				score += 20;
				cout << "Scor: " << score << endl;
				cout << "Vieti: " << lifes << endl << endl;
			}

			// Exista colizine intre un shuriken si arc asa ca setam a 3a coordonata (care este nefolosita) cu valoarea 3
			if (isCollision(radius * 5, shurikenHalfSide, glm::vec3(bowX, bowY + radius, 0), centerShuriken) == true && coordShuriken[i].z == 0) {
				coordShuriken[i].z = 3;
				lifes -= 1;
				cout << "Scor: " << score << endl;
				cout << "Vieti: " << lifes << endl << endl;
			}

			// Verificam "statusul" unui shuriken, daca a fost lovit de sageata, daca a atins arcul sau daca il 
			// randam pe ecran in mod normal
			if (coordShuriken[i].z == 0) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(coordShuriken[i].x, coordShuriken[i].y);
				modelMatrix *= Transform2D::Rotate(rotateShuriken);
				modelMatrix *= Transform2D::Translate(-shurikenHalfSide, -shurikenHalfSide);
				RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
			}
			else if (coordShuriken[i].z == 3) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(coordShuriken[i].x, coordShuriken[i].y);
				modelMatrix *= Transform2D::Rotate(rotateShuriken);
				modelMatrix *= Transform2D::Scale(0.5f, 0.5f);
				modelMatrix *= Transform2D::Translate(-shurikenHalfSide, -shurikenHalfSide);
				RenderMesh2D(meshes["shuriken"], shaders["VertexNormal"], modelMatrix);
			}
		}

		// ================================= Baloane ============================================

		// Intervalele diferite la care apar pe ecran baloanele galbene si cele rosii.
		intervalRedBalloons += deltaTimeSeconds;
		intervalYellowBalloons += deltaTimeSeconds;

		// Generarea coordonatelor aleatorii ale fiecarui balon.
		if (intervalRedBalloons > 1.5f) {
			intervalRedBalloons = 0;
			coordRedBalloons.push_back(glm::vec3(rand() % 1200 + 200, 50, 0));
		}

		if (intervalYellowBalloons > 2) {
			intervalYellowBalloons = 0;
			coordYellowBalloons.push_back(glm::vec3(rand() % 1200 + 200, 50, 0));
		}

		// Baloane galbene
		for (int i = 0; i < coordYellowBalloons.size(); i++) {

			// Variem coordonata pe axa Oy a fiecarui balon galben
			coordYellowBalloons[i].y += deltaTimeSeconds * balloonsSpeed;

			// Retinem centrul unui balon
			glm::vec3 centerBalloon = glm::vec3(coordYellowBalloons[i].x, coordYellowBalloons[i].y, 0);

			// Verificam daca exista coliziune intre varful sagetii si balon si daca da
			// setam cu valoarea 2 a 3a coordonata a centrului balonului (fiind nefolosita si neinfluentand codul).
			if (isCollision(radius * 6, 0, centerBalloon, coordTipArrow) == true && coordYellowBalloons[i].z == 0) {
				coordYellowBalloons[i].z = 2;
				score -= 10;

				cout << "Ai impuscat un balon galben! :( Fii mai atent ;)" << endl;
				cout << "Scor: " << score << endl << endl;
			}

			// Verificam daca un balon a fost spart de catre sageata sau nu.
			if (coordYellowBalloons[i].z == 0) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(coordYellowBalloons[i].x, coordYellowBalloons[i].y);
				modelMatrix *= Transform2D::Scale(4, 6);
				RenderMesh2D(meshes["yellowBalloon"], shaders["VertexColor"], modelMatrix);

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(coordYellowBalloons[i].x - 12, coordYellowBalloons[i].y - 66);
				modelMatrix *= Transform2D::Scale(0.75f, 0.75f);
				RenderMesh2D(meshes["yellowTriangle"], shaders["VertexColor"], modelMatrix);

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(coordYellowBalloons[i].x - 14, coordYellowBalloons[i].y - 133);
				RenderMesh2D(meshes["yellowPolyline"], shaders["VertexColor"], modelMatrix);
			}
		}

		// Baloane rosii
		for (int i = 0; i < coordRedBalloons.size(); i++) {

			// Variem coordonata pe axa Oy a fiecarui balon galben
			coordRedBalloons[i].y += deltaTimeSeconds * balloonsSpeed;

			// Retinem centrul unui balon
			glm::vec3 centerRedBalloon = glm::vec3(coordRedBalloons[i].x, coordRedBalloons[i].y, 0);

			// Verificam daca exista coliziune intre varful sagetii si balon si daca da
			// setam cu valoarea 2 a 3a coordonata a centrului balonului (fiind nefolosita si neinfluentand codul).
			if (isCollision(radius * 6, 0, centerRedBalloon, coordTipArrow) == true && coordRedBalloons[i].z == 0) {
				coordRedBalloons[i].z = 2;
				score += 15;

				cout << "Ai impuscat un balon rosu! :) Bravo :)" << endl;
				cout << "Scor: " << score << endl << endl;
			}

			// Verificam daca un balon a fost spart de catre sageata sau nu.
			if (coordRedBalloons[i].z == 0) {
				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(coordRedBalloons[i].x, coordRedBalloons[i].y);
				modelMatrix *= Transform2D::Scale(4, 6);
				RenderMesh2D(meshes["redBalloon"], shaders["VertexColor"], modelMatrix);

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(coordRedBalloons[i].x - 12, coordRedBalloons[i].y - 66);
				modelMatrix *= Transform2D::Scale(0.75f, 0.75f);
				RenderMesh2D(meshes["redTriangle"], shaders["VertexColor"], modelMatrix);

				modelMatrix = glm::mat3(1);
				modelMatrix *= Transform2D::Translate(coordRedBalloons[i].x - 18, coordRedBalloons[i].y - 136);
				RenderMesh2D(meshes["redPolyline"], shaders["VertexColor"], modelMatrix);
			}
		}

		// ============================= Power bar ==============================
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(20, bowY - 70);
		modelMatrix *= Transform2D::Scale(buttonMouseHold, 0.1f);
		RenderMesh2D(meshes["powerBar"], shaders["VertexColor"], modelMatrix);

		//============================Render lifes===========================

		if (lifes <= 0) {
			cout << "======================== >>> GAME OVER, MAN :( <<<===================" << endl;
			cout << "You are a dead man. Nice try!" << endl;
			glClearColor(1, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			isEndGame = true;
		}

		for (int i = 0; i < lifes; i++) {
			distanceLifes = 1190 + i * 20;

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(distanceLifes, 5);
			RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
		}

		//=============================== Render bottom========================
		modelMatrix = glm::mat3(1);
		RenderMesh2D(meshes["bottom"], shaders["VertexColor"], modelMatrix);
	}

	// Daca a fost apasata tasta P, atunci jocul intra in modul pauza si pentru
	// cat timp este in pauza se afiseaza pe ecran "PAUSED".
	if (isPaused == true) {
		cout << "=========================>> PAUZA <<======================" << endl;
		glClearColor(0, 0.65f, 0.89f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//P
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(200, 300);
		modelMatrix *= Transform2D::Scale(1, 2);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(200, 500);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(300, 400);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(200, 400);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		//A
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(300, 300);
		modelMatrix *= Transform2D::Scale(1, 3);
		modelMatrix *= Transform2D::Rotate(radians + M_PI / 4);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(370, 500);
		modelMatrix *= Transform2D::Scale(1, 3);
		modelMatrix *= Transform2D::Rotate(radians - M_PI / 4);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(300, 360);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		//U
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(470, 290);
		modelMatrix *= Transform2D::Scale(1, 2.1f);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(480, 290);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(570, 290);
		modelMatrix *= Transform2D::Scale(1, 2.1f);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		//S
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(640, 500);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(630, 400);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(640, 400);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(730, 300);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(640, 300);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		//E
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(770, 500);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(770, 300);
		modelMatrix *= Transform2D::Scale(1, 2);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(770, 400);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(770, 300);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		//D
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(900, 500);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(900, 300);
		modelMatrix *= Transform2D::Scale(1, 2);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1000, 300);
		modelMatrix *= Transform2D::Scale(1, 2);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(900, 300);
		modelMatrix *= Transform2D::Rotate(radians);
		RenderMesh2D(meshes["bowRectangle"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// Arcul se va deplasa in sus la apasarea tastei W.
	if (window->KeyHold(GLFW_KEY_W)) {
		bowY += deltaTime * bowSpeed;
	}

	// Arcul se va deplasa in jos la apasarea tastei S.
	if (window->KeyHold(GLFW_KEY_S)) {
		bowY -= deltaTime * bowSpeed;
	}

	// Daca tinem apasat pe click stanga, puterea de tragere se mareste (power bar-ul creste in lungime)
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (isFlyingArrow == false) {
			buttonMouseHold += deltaTime;
			arrowPower += (deltaTime * 4);

			// Setam o limita superioara pentru viteza de tragere a arcului.
			if (arrowPower >= 20) {
				arrowPower = 0;
				buttonMouseHold = 0;
			}
		}
	}

	// Daca dam drumul butonului stanga al mouse-ului, power bar-ul se reseteaza
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		buttonMouseHold = 0.002f;
	}

}

void Tema1::OnKeyPress(int key, int mods)
{
	//Daca apasam Escape, iesim din joc
	if (key == GLFW_KEY_ESCAPE) {
		glClearColor(1, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		isEndGame = true;
	}

	//Modul de pauza 
	if (key == GLFW_KEY_P) {
		isPaused = !isPaused;
	}

}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Verificam daca sageata este lansata si retinem coordonatele mouse-ului.
	if (isFlyingArrow == false) {
		mouseY = 720 - mouseY;
		coordMouseX = mouseX;
		coordMouseY = mouseY;
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Veroficam daca sageata este lansata si setam valorile de tip bool corect.
	if (isFlyingArrow == false) {
		isMouseButtonRelease = true;
		isFlyingArrow = true;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
