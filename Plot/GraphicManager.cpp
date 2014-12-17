#include "GraphicManager.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

GraphicManager::GraphicManager()
	:grid(NULL), field(NULL)
{
	root = new Graphic();
}

void GraphicManager::Draw()
{
	root->Draw();
}

void GraphicManager::MakeGrid(Vector3f center, Vector3f size, Vector3i split)
{
	if (grid == NULL){
		grid = new GridSpace(center, size, split);
		AddToRoot(grid);
	}
}

Field* const GraphicManager::MakeField(const Vector3i &lftBtmNear, const Vector3i &size)
{
	if( field == NULL)
	{
		field = new Field(lftBtmNear,size);
		AddToRoot(field);
	} 

	return field;
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