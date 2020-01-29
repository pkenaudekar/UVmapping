// 1725ISA2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
//#include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 

#define VERTICES 0
#define INDICES 1

using namespace std;

#include "getbmp.h"
#include <vector>


// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
GLfloat rotate_y = 0;
GLfloat rotate_x = 0;
GLfloat rotate_z = 0;

//Object cube data
static float vertices1[] =
{
	3.0, 6.0, 0.0,//front
	3.0, 3.0, 0.0,
	6.0, 6.0, 0.0,
	6.0, 6.0, 0.0,
	3.0, 3.0, 0.0,
	6.0, 3.0, 0.0,//front
	3.0, 6.0, -3.0,//back
	3.0, 3.0, -3.0,
	6.0, 6.0, -3.0,
	6.0, 6.0, -3.0,
	3.0, 3.0, -3.0,
	6.0, 3.0, -3.0,//back
	3.0, 6.0, 0.0,//left
	3.0, 3.0, 0.0,
	3.0, 6.0, -3.0,
	3.0, 6.0, -3.0,
	3.0, 3.0, 0.0,
	3.0, 3.0, -3.0,//left
	6.0, 6.0, 0.0,//right
	6.0, 3.0, 0.0,
	6.0, 6.0, -3.0,
	6.0, 6.0, -3.0,
	6.0, 3.0, 0.0,
	6.0, 3.0, -3.0,//right
	3.0, 6.0, 0.0,//top
	3.0, 6.0, -3.0,
	6.0, 6.0, 0.0,
	6.0, 6.0, 0.0,
	3.0, 6.0, -3.0,
	6.0, 6.0, -3.0,//top
	3.0, 3.0, 0.0,//bottom
	3.0, 3.0, -3.0,
	6.0, 3.0, 0.0,
	6.0, 3.0, 0.0,
	3.0, 3.0, -3.0,
	6.0, 3.0, -3.0//bottom
	
};

static float textureCoordinates1[] =
{
	
	0.0, 1.0,//front
	0.0, 0.75,
	0.25, 1.0,
	0.25, 1.0,
	0.0, 0.75,
	0.25, 0.75,//front
	0.25, 1.0,//back
	0.25, 0.75,
	0.5, 1.0,
	0.5, 1.0,
	0.25, 0.75,
	0.5, 0.75,//back
	0.5, 1.0,//left
	0.5, 0.75,
	0.75, 1.0,
	0.75, 1.0,
	0.5, 0.75,
	0.75, 0.75,//left
	0.25, 0.75,//right
	0.25, 0.5,
	0.5, 0.75,
	0.5, 0.75,
	0.25, 0.5,
	0.5, 0.5,//right
	0.25, 0.5,//top
	0.25, 0.25,
	0.5, 0.5,
	0.5, 0.5,
	0.25, 0.25,
	0.5, 0.25,//top
	0.25, 0.25,//bottom
	0.25, 0.0,
	0.5, 0.25,
	0.5, 0.25,
	0.25, 0.0,
	0.5, 0.0//bottom
	
};
static int stripIndices1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
//static int stripIndices1[] = { 0, 1, 2, 3, 4, 5 };

class Object {

private:
	GLuint m_TextureId;
	GLuint m_Buffer[2];

public:
	std::vector<float> m_Vertices;
	std::vector<float> m_VertexNomals;
	std::vector<float> m_TextureCoordinates;
	std::vector<int> m_Indices;

	int m_NumIndices;

	Object() {}
	~Object();

	void loadTexture(const char* filepath);
	void loadBuffers();
	void Draw();

};

Object::~Object() {
	//	glDeleteBuffers(2, buffer);
}

void Object::loadTexture(const char* filepath) {

	// Local storage for bmp image data.
	BitMapFile *image;

	// Load the image.
	image = getbmp(filepath);

	glGenTextures(1, &m_TextureId);

	// Create texture object 
	glBindTexture(GL_TEXTURE_2D, m_TextureId);

	// Specify image data for currently active texture object.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->sizeX, image->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image->data);

	// Set texture parameters for wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture parameters for filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}
void Object::loadBuffers() {
	try {
		glGenBuffers(2, m_Buffer);
	}
	catch (char *excp) {
		cout << "Caught " << excp;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, (m_Vertices.size() * sizeof(float)) + (m_TextureCoordinates.size() * sizeof(float)), NULL, GL_STATIC_DRAW);

	// Copy vertex coordinates data into first half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(float), m_Vertices.data());

	// Copy vertex color data into second half of vertex buffer.
	glBufferSubData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), sizeof(float)*m_TextureCoordinates.size(), m_TextureCoordinates.data());

	// Bind and fill indices buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(float), m_Indices.data(), GL_STATIC_DRAW);

}

void Object::Draw() {

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//activate buffers.
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer[VERTICES]);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);

	glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)0);
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)(m_Vertices.size()*(sizeof(float))));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer[INDICES]);
	glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, (unsigned int*)0 + 0);

}


Object obj1;


void display() {

	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Reset transformations
	glLoadIdentity();

	gluLookAt(0, 0, 20, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Rotate when user changes rotate_x and rotate_y
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);


	obj1.Draw();

	//glFlush();
	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 1.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);


	//Object cube
	for (int i = 0; i < 36*3; i++)
		obj1.m_Vertices.push_back(vertices1[i]);
	for (int i = 0; i < 36*2; i++)
		obj1.m_TextureCoordinates.push_back(textureCoordinates1[i]);
	for (int i = 0; i < 36; i++)
		obj1.m_Indices.push_back(stripIndices1[i]);

	obj1.m_NumIndices = 36;
	obj1.loadTexture("cube.bmp");
	//obj1.loadTexture("photo_1-1.bmp");
	obj1.loadBuffers();

}

void reshape(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		//cube movement------------
	case 'x':
		rotate_x += 1.0;
		break;
	case 'y':
		rotate_y += 1.0;
		break;
	case 'z':
		rotate_z += 1.0;
		break;
	}

	glutPostRedisplay();      // Post re-paint request to activate display()

}





/* Driver function */
int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("uvmappingCube.cpp");

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);

	//glutSpecialFunc(specialKeys);
	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();

	//return 0;
}
