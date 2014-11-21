#include "GraphicManager.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

GraphicManager::GraphicManager()
	:grid(NULL)
{

}


void GraphicManager::DrawGrid()
{
	grid->Draw();
}

void GraphicManager::Initialize(int windowX, int windowY, int argc, char** argv)
{
	glutInitWindowSize(windowX, windowY);
	glutInit(&argc, argv);	//glClearColor(0.0, 0.0, 1.0, 1.0);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);

	GLenum err = glewInit();
	if ( err != GLEW_OK )
	{
		cout << "Can not Start GLEW" << endl;
		cerr << glewGetErrorString(err) << endl;
		exit(2);
	}
}