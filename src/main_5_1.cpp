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
GLuint textureJupiter;
GLuint textureMars;
GLuint textureVenus;
GLuint textureMoon;

Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;

float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(-50, 0, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
float angle = 0;
float angleV = 0;
float angleE = 0;
float angleM = 0;
float angleJ = 0;
float angleMoon = 0;

float VenusX = 8;
float VenusY = 8;

float EarthX = 16;
float EarthY = 16;

float MarsX = 24;
float MarsY = 24;

float JupiterX = 32;
float JupiterY = 32;

float MoonX = 17.5;
float MoonY = 17.5;


void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
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

float spin(float ile)
{
	angle += ile;
	if (angle >= 360)
	{
		angle = 0;
	}
	return angle;
}

float spinV(float ile)
{
	angleV += ile;
	if (angleV >= 360)
	{
		angleV = 0;
	}
	return angleV;
}

float spinE(float ile)
{
	angleE += ile;
	if (angleE >= 360)
	{
		angleE = 0;
	}
	return angleE;
}

float spinM(float ile)
{
	angleM += ile;
	if (angleM >= 360)
	{
		angleM = 0;
	}
	return angleM;
}

float spinJ(float ile)
{
	angleJ += ile;
	if (angleJ >= 360)
	{
		angleJ = 0;
	}
	return angleJ;
}

float spinMoon(float ile)
{
	angleMoon += ile;
	if (angleMoon >= 360)
	{
		angleMoon = 0;
	}
	return angleMoon;
}


void renderScene()
{

	VenusX = cos(spinV(0.003)) * 8;
	VenusY = sin(spinV(0.003)) * 8;

	EarthX = cos(spinE(0.0025)) * 16;
	EarthY = sin(spinE(0.0025)) * 16;

	MarsX = cos(spinM(0.002)) * 24;
	MarsY = sin(spinM(0.002)) * 24;

	JupiterX = cos(spinJ(0.001)) * 32;
	JupiterY = sin(spinJ(0.001)) * 32;

	MoonX = EarthX + cos(spinMoon(0.01)) * 1.5;
	MoonY = EarthY + sin(spinMoon(0.01)) * 1.5;

	// Aktualizacja macierzy widoku i rzutowania. Macierze sa przechowywane w zmiennych globalnych, bo uzywa ich funkcja drawObject.
	// (Bardziej elegancko byloby przekazac je jako argumenty do funkcji, ale robimy tak dla uproszczenia kodu.
	//  Jest to mozliwe dzieki temu, ze macierze widoku i rzutowania sa takie same dla wszystkich obiektow!)
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Macierz statku "przyczepia" go do kamery. Warto przeanalizowac te linijke i zrozumiec jak to dziala.
	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.001f));
	drawObjectColor(&shipModel, shipModelMatrix, glm::vec3(0.6f));

	glm::mat4 SunModelMatrix = glm::translate(glm::vec3(0, 0, 0)) *glm::scale(glm::vec3(4.0f)) * glm::rotate(spin(0.001), glm::vec3(0, 1, 0));
	glm::mat4 VenusModelMatrix = glm::translate(glm::vec3(VenusX, 0, VenusY)) * glm::rotate(spin(0.001), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.0f));
	glm::mat4 MarsModelMatrix = glm::translate(glm::vec3(MarsX, 0, MarsY)) * glm::rotate(spin(0.001), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.50f));
	glm::mat4 EarthModelMatrix = glm::translate(glm::vec3(EarthX, 0, EarthY)) *glm::rotate(spin(0.001), glm::vec3(0, 1, 0));
	glm::mat4 JupiterModelMatrix = glm::translate(glm::vec3(JupiterX, 0, JupiterY)) * glm::rotate(spin(0.001), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.0f));
	glm::mat4 MoonModelMatrix = glm::translate(glm::vec3(MoonX, 0, MoonY)) * glm::rotate(spin(0.001), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.25f));

	drawObjectTexture(&sphereModel, SunModelMatrix, textureSun);
	drawObjectTexture(&sphereModel, VenusModelMatrix, textureVenus);
	drawObjectTexture(&sphereModel, EarthModelMatrix, textureEarth);
	drawObjectTexture(&sphereModel, MoonModelMatrix, textureMoon);
	drawObjectTexture(&sphereModel, MarsModelMatrix, textureMars);
	drawObjectTexture(&sphereModel, JupiterModelMatrix, textureJupiter);


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
	textureVenus = Core::LoadTexture("textures/venus.png");
	textureMars = Core::LoadTexture("textures/mars.png");
	textureJupiter = Core::LoadTexture("textures/jupiter.png");
	textureEarth = Core::LoadTexture("textures/earth2.png");
	textureMoon = Core::LoadTexture("textures/moon.png");
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
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);
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