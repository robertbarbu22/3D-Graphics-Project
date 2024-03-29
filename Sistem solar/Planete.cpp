#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include "loadShaders.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SOIL.h"
#include <vector>
using namespace std;
GLuint
VaoId1, VaoId2,
VboId1, VboId2,
EboId1, EboId2,
ColorBufferId,
ProgramIdv,
ProgramIdf,
viewLocation,
projLocation,
codColLocation,
myMatrixLocation,
matrUmbraLocation,
depthLocation,
codCol;
GLuint VaoIdRing, VboIdRing, EboIdRing;
GLint objectColorLoc, lightColorLoc, lightPosLoc, viewPosLoc;
GLuint texture;
string numetextura;
float const PI = 3.141592f;
float const U_MIN = -PI / 2, U_MAX = PI / 2, V_MIN = 0, V_MAX = 2 * PI;
int const NR_PARR = 131, NR_MERID = 132;
float step_u = (U_MAX - U_MIN) / NR_PARR, step_v = (V_MAX - V_MIN) / NR_MERID;
float radius = 50;
int index, index_aux;
// variabile pentru matricea de vizualizare
float Obsx = 100.0, Obsy = -1500.0, Obsz = 200;
float Refx = 0.0f, Refy = 1000.0f, Refz = 0.0f;
float Vx = 0.0, Vy = 0.0, Vz = 1.0;
// variabile pentru matricea de proiectie
float width = 800, height = 600, znear = 0.1, fov = 45;
// matrice utilizate
glm::mat4 view, projection;
glm::mat4 myMatrix, matrTrans, matrScale, matrTransRotation;
// sursa de lumina
float xL = 0.0f, yL = 1000.0f, zL =0.0f;
// matricea umbrei
float matrUmbra[4][4];
float angleMercur = 0.0f, angleVenus = 0.0f, anglePamant = 0.0f, angleMarte = 0.0f, angleJupiter = 0.0f, angleSaturn = 0.0f, angleUranus = 0.0f, angleNeptun = 0.0f;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l':
		Vx -= 0.1;
		break;
	case 'r':
		Vx += 0.1;
		break;
	case '+':
		Obsy += 10;
		break;
	case '-':
		Obsy -= 10;
		break;
	}
	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		Obsx -= 20;
		break;
	case GLUT_KEY_RIGHT:
		Obsx += 20;
		break;
	case GLUT_KEY_UP:
		Obsz += 20;
		break;
	case GLUT_KEY_DOWN:
		Obsz -= 20;
		break;
	}
}

void CreateVBO1(void)
{
	glm::vec4 Vertices1[(NR_PARR + 1) * NR_MERID];
	GLushort Indices1[2 * (NR_PARR + 1) * NR_MERID + 4 * (NR_PARR + 1) * NR_MERID];
	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR + 1; parr++)
		{
			float u = U_MIN + parr * step_u;
			float v = V_MIN + merid * step_v;
			float x_vf = radius * cosf(u) * cosf(v);
			float y_vf = radius * cosf(u) * sinf(v);
			float z_vf = radius * sinf(u);
			index = merid * (NR_PARR + 1) + parr;
			Vertices1[index] = glm::vec4(x_vf, y_vf, z_vf, 1.0);
			Indices1[index] = index;
			index_aux = parr * (NR_MERID)+merid;
			Indices1[(NR_PARR + 1) * NR_MERID + index_aux] = index;
			if ((parr + 1) % (NR_PARR + 1) != 0)
			{
				int AUX = 2 * (NR_PARR + 1) * NR_MERID;
				int index1 = index;
				int index2 = index + (NR_PARR + 1);
				int index3 = index2 + 1;
				int index4 = index + 1;
				if (merid == NR_MERID - 1)
				{
					index2 = index2 % (NR_PARR + 1);
					index3 = index3 % (NR_PARR + 1);
				}
				Indices1[AUX + 4 * index] = index1;
				Indices1[AUX + 4 * index + 1] = index2;
				Indices1[AUX + 4 * index + 2] = index3;
				Indices1[AUX + 4 * index + 3] = index4;
			}
		}
	}
	glGenVertexArrays(1, &VaoId1);
	glGenBuffers(1, &VboId1);
	glGenBuffers(1, &EboId1);
	glBindVertexArray(VaoId1);
	glBindBuffer(GL_ARRAY_BUFFER, VboId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1), Vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices1), Indices1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glGenVertexArrays(1, &VaoIdRing);
	glGenBuffers(1, &VboIdRing);
	glBindVertexArray(VaoIdRing);
	glBindBuffer(GL_ARRAY_BUFFER, VboIdRing);

	// Define vertices and indices for the rings
	// Example: You can create a simple ring using a loop
	vector<GLfloat> VerticesRing;
	vector<GLushort> IndicesRing;
	float ringRadius = 55.0f; // Adjust the radius as needed

	for (int i = 0; i <= NR_MERID; i++) {
		float angle = static_cast<float>(i) / NR_MERID * (2.0f * PI);
		float x = ringRadius * cosf(angle);
		float y = ringRadius * sinf(angle);
		float z = 0.0f; // Set the z-coordinate for the ring

		// Add vertex coordinates
		VerticesRing.push_back(x);
		VerticesRing.push_back(y);
		VerticesRing.push_back(z);

		// Add texture coordinates if needed
		// ...

		// Add index
		IndicesRing.push_back(i);
	}

	glBufferData(GL_ARRAY_BUFFER, VerticesRing.size() * sizeof(GLfloat), VerticesRing.data(), GL_STATIC_DRAW);
	// Assuming GL_UNSIGNED_SHORT is sufficient for your number of vertices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesRing.size() * sizeof(GLushort), IndicesRing.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Enable other attributes and set pointers if needed
	// ...


}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId1);
	glDeleteBuffers(1, &EboId1);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId1);
	glDeleteBuffers(1, &VboIdRing);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoIdRing);
}



void LoadTexture(const char* photoPath)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Unwrapping the image horizontally/vertically based on texture parameters;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	unsigned char* image = SOIL_load_image(photoPath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CreateShadersFragment(void)
{
	ProgramIdf = LoadShaders("shaderPlanete.vert", "shaderPlanete.frag");
	glUseProgram(ProgramIdf);
}
void DestroyShaders(void)
{
	glDeleteProgram(ProgramIdf);
}

void Initialize(void)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.f);
	CreateVBO1();
	CreateShadersFragment();
	objectColorLoc = glGetUniformLocation(ProgramIdf, "objectColor");
	lightColorLoc = glGetUniformLocation(ProgramIdf, "lightColor");
	lightPosLoc = glGetUniformLocation(ProgramIdf, "lightPos");
	viewPosLoc = glGetUniformLocation(ProgramIdf, "viewPos");
	viewLocation = glGetUniformLocation(ProgramIdf, "view");
	projLocation = glGetUniformLocation(ProgramIdf, "projection");
	myMatrixLocation = glGetUniformLocation(ProgramIdf, "myMatrix");
	matrUmbraLocation = glGetUniformLocation(ProgramIdf, "matrUmbra");
	codColLocation = glGetUniformLocation(ProgramIdf, "codCol");
	glUniform1i(glGetUniformLocation(ProgramIdf, "myTexture"), 0);
	LoadTexture("soare.jpg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(ProgramIdf, "myTexture"), 0);


}
void DrawPlanet(glm::mat4 myMatrix, int codCol = 0)
{
	glBindVertexArray(VaoId1);
	glUniform1i(codColLocation, codCol);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0)
			glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_SHORT, (GLvoid*)((2 * (NR_PARR + 1)
				* (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

}

void drawEllipse(float centerX, float centerY, float a, float b) {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i++) {
		float angle = i * PI / 180.0;
		float x = centerX + a * cos(angle);
		float y = centerY + b * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
}


void DrawShadow(glm::mat4 myMatrix)
{
	glBindVertexArray(VaoId1);
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0)
			glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_SHORT, (GLvoid*)((2 * (NR_PARR + 1)
				* (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}
}
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// vizualizare + proiectie
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz);
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz);
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projection = glm::infinitePerspective(fov, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
	// matricea pentru umbra
	float D = 100.0f;
	matrUmbra[0][0] = zL + D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3]
		= 0;
	matrUmbra[1][0] = 0; matrUmbra[1][1] = zL + D; matrUmbra[1][2] = 0; matrUmbra[1][3]
		= 0;
	matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] =
		-1;
	matrUmbra[3][0] = -D * xL; matrUmbra[3][1] = -D * yL; matrUmbra[3][2] = -D * zL; matrUmbra[3][3] = zL;
	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);
	glUseProgram(ProgramIdf);
	// SOARELE
	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.f, 3.f, 3.f));
	glm::vec3 lightPosition = glm::vec3(0.0f, 1000.0f, 0.0f); // Coordonatele Soarelui
	glUniform3f(lightPosLoc, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(objectColorLoc, 1.0f, 0.4f, 0.2f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;
	DrawPlanet(myMatrix, 2);

	glColor3f(1.0f, 1.0f, 1.0f);
	drawEllipse(0, 0, 200, 200);
	drawEllipse(0, 0, 260, 260);
	drawEllipse(0, 0, 325, 325);
	drawEllipse(0, 0, 400, 400);
	drawEllipse(0, 0, 465, 465);
	drawEllipse(0, 0, 530, 530);
	drawEllipse(0, 0, 595, 595);
	drawEllipse(0, 0, 660, 660);

	// MERCUR
	angleMercur += 0.04f;

	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3( 600.0f * cos(angleMercur), 1000.f + 600.0f * sin(angleMercur), 0.f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	glUniform3f(objectColorLoc, 0.45f, 0.57f, 0.7f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;


	DrawPlanet(myMatrix);
	//DrawShadow(myMatrix);


	// VENUS
	angleVenus += 0.035f;

	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(775.0f * cos(angleVenus), 1000.f + 775.0f * sin(angleVenus), 0.f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.6f, 0.6f, 0.6f));
	glUniform3f(objectColorLoc, 0.8f, 0.33f, 0.0f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;

	DrawPlanet(myMatrix);

	//DrawShadow(myMatrix);
	// PAMANT

	anglePamant += 0.03f;

	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(975.0f * cos(anglePamant), 1000.f + 975.0f * sin(anglePamant), 0.f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 0.7f));
	glUniform3f(objectColorLoc, 0.0f, 0.0f, 0.63f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;
	DrawPlanet(myMatrix);
	//DrawShadow(myMatrix);
	

	//// LUNA
	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(1050.0f * cos(anglePamant), 1000.f + 1050.0f * sin(anglePamant), 0.f));

	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	glUniform3f(objectColorLoc, 0.7f, 0.7f, 0.7f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans *  matrScale;
	DrawPlanet(myMatrix);
	

	// MARTE

	angleMarte += 0.025;
	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(1200.0f * cos(angleMarte), 1000.f + 1200.0f * sin(angleMarte), 0.f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	glUniform3f(objectColorLoc, 0.64f, 0.16f, 0.16f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;
	DrawPlanet(myMatrix);

	//Jupiter

	angleJupiter += 0.02;
	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(1400.0f * cos(angleJupiter), 1000.f + 1400.0f * sin(angleJupiter), 0.f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glUniform3f(objectColorLoc, 0.64f, 0.16f, 0.16f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 150.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;
	DrawPlanet(myMatrix);

	//Saturn

	angleSaturn += 0.015;
	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(1600.0f * cos(angleSaturn), 1000.f + 1600.0f * sin(angleSaturn), 0.f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.85f, 0.85f, 0.85f));
	glUniform3f(objectColorLoc, 0.7f, 0.5f, 0.0f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;
	DrawPlanet(myMatrix);

	glColor3f(1.0f, 1.0f, 1.0f);
	drawEllipse(0, 0, 100, 100);
	drawEllipse(0, 0, 90, 90);
	drawEllipse(0, 0, 80, 80);
	drawEllipse(0, 0, 70, 70);
	drawEllipse(0, 0, 60, 60);
	
	//Uranus

	angleUranus += 0.01;
	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(1800.0f * cos(angleUranus), 1000.f + 1800.0f * sin(angleUranus), 0.f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 0.7f));
	glUniform3f(objectColorLoc, 0.3f, 1.0f, 1.0f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;
	DrawPlanet(myMatrix);

	glColor3f(1.0f, 1.0f, 1.0f);
	drawEllipse(0, 0, 70, 70);
	drawEllipse(0, 0, 60, 60);

	//Neptun

	angleNeptun += 0.005;
	matrTrans = glm::translate(glm::mat4(1.0f), glm::vec3(2000.0f * cos(angleNeptun), 1000.f + 2000.0f * sin(angleNeptun), 0.f));
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.7f, 0.7f, 0.7f));
	glUniform3f(objectColorLoc, 0.0f, 0.5f, 1.0f);
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.f, 1000.f, 0.f);
	glUniform3f(viewPosLoc, Obsx, Obsy, Obsz);
	myMatrix = matrTrans * matrScale;
	DrawPlanet(myMatrix);



	//DrawShadow(myMatrix);
	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(1000, 750);
	glutCreateWindow("Sistemul Solar");
	glewInit();
	Initialize();
	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}