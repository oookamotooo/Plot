#include "StreamLine.h"
#include <GL/glew.h>

void StreamLine::AddPoint(Vector3d vertex)
{
	vertices.push_back(toFloatVector(vertex));
}

void StreamLine::Draw()
{
	Graphic::Draw();
	bindVertexBuffer();

	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
}