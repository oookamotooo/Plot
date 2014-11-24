#include "StreamLine.h"
#include <GL/glew.h>

void StreamLine::BeginNewLine()
{
	vertOfLine.push_back(0);
}
void StreamLine::AddPoint(Vector3d vertex)
{
	if( vertOfLine.size() == 0)
		BeginNewLine();

	vertices.push_back(toFloatVector(vertex));
	vertOfLine[vertOfLine.size()-1]++;
}

void StreamLine::Draw()
{
	Graphic::Draw();
	bindVertexBuffer();
	
	glColor3d(0,1,0);
	
	for( int i=0, st = 0; i < vertOfLine.size(); i++)
	{
		glDrawArrays(GL_LINE_STRIP, st, vertOfLine[i]);
		st += vertOfLine[i];
	}
	//glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
}
