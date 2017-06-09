#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

GLuint programColor;
GLuint programTexture;
GLuint textureEarth;
GLuint textureSun;
GLuint textureMerkury;
GLuint textureJupiter;
GLuint textureMars;
GLuint textureVenus;
GLuint textureMoon;
GLuint textureSaturn;
GLuint textureUranus;
GLuint textureNeptune;

Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;
float cameraMove = 1;
float cameraAngle = 0;
float cameraRotate = 0;
glm::vec3 cameraPos = glm::vec3(-100, 0, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
glm::vec3 sunXYZ;
glm::vec3 merkuryXYZ;
glm::vec3 venusXYZ;
glm::vec3 earthXYZ;
glm::vec3 moonXYZ;
glm::vec3 marsXYZ;
glm::vec3 jupiterXYZ;
glm::vec3 saturnXYZ;
glm::vec3 saturnRingXYZ;
glm::vec3 saturnRing2XYZ;
glm::vec3 uranusXYZ;
glm::vec3 neptuneXYZ;


void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.3f;
	switch (key)
	{
		case 'z': cameraAngle -= angleSpeed; break;
		case 'x': cameraAngle += angleSpeed; break;
		case 'q': cameraPos -= angleSpeed; break;
		case 'e': cameraPos += angleSpeed; break;
		case 'w': cameraPos += cameraDir * moveSpeed; break;
		case 's': cameraPos -= cameraDir * moveSpeed; break;
		case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0, 1, 0)) * moveSpeed; break;
		case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0, 1, 0)) * moveSpeed; break;
		case 'p': cameraPos += glm::cross(cameraDir, glm::vec3(1, 0, 1)) * moveSpeed; break;
		case 'o': cameraPos -= glm::cross(cameraDir, glm::vec3(1, 0, 1)) * moveSpeed; break;
		case 't': cameraMove -= angleSpeed; break;
		case 'y': cameraMove += angleSpeed; break;
		case 'c': cameraRotate += angleSpeed; break;
		case 'v': cameraRotate -= angleSpeed; break;
	}
}

glm::mat4 createCameraMatrix()
{
	// Obliczanie kierunku patrzenia kamery (w plaszczyznie x-z) przy uzyciu zmiennej cameraAngle kontrolowanej przez klawisze.
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0, 1, 0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint tex)
{
	GLuint program = programTexture;

	glUseProgram(program);

	//tex = textureGrid;
	Core::SetActiveTexture(tex, "uni", program, 0); //+
													//glUniform3f(glGetUniformLocation(program, "textureColor"), tex,tex,tex);
													//glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z); //-
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}


void renderScene()
{

	float time = glutGet(GLUT_ELAPSED_TIME) / 5000.0f;
	sunXYZ = glm::vec3(0, 0, cos(time) * 30);

	//sunXYZ = glm::vec3(0, 0, 0);
	merkuryXYZ = glm::vec3(cos(time*5) * 8, 0, sin(time*5) * 8);
	venusXYZ = glm::vec3(cos(time * 4) * 12, 0, sin(time *4) * 12);
	earthXYZ = glm::vec3(cos(time * 3) * 18, 0, sin(time * 3) * 18);
	moonXYZ = glm::vec3(cos(time * 10) * 3, 0, sin(time * 10) * 3);
	marsXYZ = glm::vec3(cos(time * 5) * 24, 0, sin(time * 5) * 24);
	jupiterXYZ = glm::vec3(cos(time * 6) * 34, 0, sin(time * 6) * 34);
	saturnXYZ = glm::vec3(cos(time * 8) * 44, 0, sin(time * 8) * 44);
	saturnRingXYZ = glm::vec3(cos(time *100000000) * 4, 0, sin(time * 100000000) * 4);
	saturnRing2XYZ = glm::vec3(cos(time * 10000000000) * 4, 0, sin(time * 10000000000) * 4);
	uranusXYZ = glm::vec3(cos(time * 7) * 53, 0, sin(time * 7) * 53);
	neptuneXYZ = glm::vec3(cos(time * 7.5) * 58, 0, sin(time * 7.5) * 58);

	// Aktualizacja macierzy widoku i rzutowania. Macierze sa przechowywane w zmiennych globalnych, bo uzywa ich funkcja drawObject.
	// (Bardziej elegancko byloby przekazac je jako argumenty do funkcji, ale robimy tak dla uproszczenia kodu.
	//  Jest to mozliwe dzieki temu, ze macierze widoku i rzutowania sa takie same dla wszystkich obiektow!)
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Macierz statku "przyczepia" go do kamery. Warto przeanalizowac te linijke i zrozumiec jak to dziala.
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraMove*cameraDir * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-cameraAngle + cameraRotate + glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.001f));
	drawObjectColor(&shipModel, shipModelMatrix, glm::vec3(0.6f));

	glm::mat4 SunModelMatrix = glm::translate(sunXYZ)* glm::scale(glm::vec3(5.0f));
	glm::mat4 MerkuryModelMatrix = glm::translate(merkuryXYZ) * glm::translate(sunXYZ) * glm::rotate(time*100, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f));
	glm::mat4 VenusModelMatrix = glm::translate(venusXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 50, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.0f));
	glm::mat4 EarthModelMatrix = glm::translate(earthXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 40, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.0f));
	glm::mat4 MoonModelMatrix = glm::translate(moonXYZ) * glm::translate(earthXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 40, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.5f));
	glm::mat4 MarsModelMatrix = glm::translate(marsXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.5f));
	glm::mat4 JupiterModelMatrix = glm::translate(jupiterXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(3.0f));
	glm::mat4 SaturnModelMatrix = glm::translate(saturnXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(3.0f));
	glm::mat4 SaturnRingModelMatrix = glm::translate(saturnRingXYZ) * glm::translate(saturnXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 40, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f,0.1f,1.0f));
	glm::mat4 SaturnRingModelMatrix2 = glm::translate(saturnRing2XYZ) * glm::translate(saturnXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 40, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f, 0.1f, 1.0f));
	glm::mat4 UranusModelMatrix = glm::translate(uranusXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.5f));
	glm::mat4 NeptuneModelMatrix = glm::translate(neptuneXYZ) * glm::translate(sunXYZ) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.5f));

	drawObjectTexture(&sphereModel, SunModelMatrix, textureSun);
	drawObjectTexture(&sphereModel, MerkuryModelMatrix, textureMerkury);
	drawObjectTexture(&sphereModel, VenusModelMatrix, textureVenus);
	drawObjectTexture(&sphereModel, EarthModelMatrix, textureEarth);
	drawObjectTexture(&sphereModel, MoonModelMatrix, textureMoon);
	drawObjectTexture(&sphereModel, MarsModelMatrix, textureMars);
	drawObjectTexture(&sphereModel, JupiterModelMatrix, textureJupiter);
	drawObjectTexture(&sphereModel, SaturnModelMatrix, textureSaturn);
	drawObjectTexture(&sphereModel, SaturnRingModelMatrix, textureSaturn);
	drawObjectTexture(&sphereModel, SaturnRingModelMatrix2, textureSaturn);
	drawObjectTexture(&sphereModel, UranusModelMatrix, textureUranus);
	drawObjectTexture(&sphereModel, NeptuneModelMatrix, textureNeptune);


	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	shipModel = obj::loadModelFromFile("models/statek.obj");
	textureSun = Core::LoadTexture("textures/sun.png");
	textureMerkury = Core::LoadTexture("textures/merkury.png");
	textureVenus = Core::LoadTexture("textures/venus.png");
	textureMars = Core::LoadTexture("textures/mars.png");
	textureJupiter = Core::LoadTexture("textures/jupiter.png");
	textureEarth = Core::LoadTexture("textures/earth2.png");
	textureMoon = Core::LoadTexture("textures/moon.png");
	textureSaturn = Core::LoadTexture("textures/saturn.png");
	textureUranus = Core::LoadTexture("textures/uranus.png");
	textureNeptune = Core::LoadTexture("textures/neptune.png");
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(00, 00);
	glutInitWindowSize(900, 900);
	glutCreateWindow("OpenGL Pierwszy Program");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
