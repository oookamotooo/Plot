#include "StreamLine.h"
#include <GL/glew.h>
#include "Camera.h"
#include <Eigen/Geometry>
void StreamLine::BeginNewLine()
{
	vertOfLine.push_back(0);
}
void StreamLine::AddPoint(const Vector3d &vertex, const bool reverse)
{
	if( vertOfLine.size() == 0)
		BeginNewLine();

	//最初の1点では矢印が書けないのでそのまま追加するだけ
	if (vertOfLine[vertOfLine.size() - 1] == 0){
		vertices.push_back(vertex.cast<float>());
		vertOfLine[vertOfLine.size() - 1]++;
		return;
	}

	Vector3f nex = vertex.cast<float>();
	Vector3f cur = vertices[vertices.size() - 1];
	Vector3f d = nex - cur;
	Vector3f c = (Camera::getCamera()->GetLook() - Camera::getCamera()->GetPosition()).normalized().cast<float>();	//矢じりの法線
	Vector3f n = d.cross(c).normalized() * 0.05f;
	if (reverse){
		Vector3f arrow1 = cur + 0.3f*d + n;
		Vector3f arrow2 = cur + 0.3f*d - n;

		vertices.push_back(arrow1);	//先に矢印を書く
		vertices.push_back(arrow2);
		vertices.push_back(cur);
		vertices.push_back(nex);
		vertOfLine[vertOfLine.size() - 1] += 4;
	}
	else
	{
		//矢印を書く
		Vector3f arrow1 = cur + 0.7f*d + n;
		Vector3f arrow2 = cur + 0.7f*d - n;
		vertices.push_back(nex);	//次の頂点
		vertices.push_back(arrow1);	//矢印
		vertices.push_back(arrow2);
		vertices.push_back(nex);	//次の頂点
		vertOfLine[vertOfLine.size() - 1] += 4;
	}
}

void StreamLine::Draw()
{
	if (!GetVisible())
		return;

	if (!buffer)
		genVertexBuffer();

	bindVertexBuffer();
	
	glColor3d(0,1,0);
	
	for( int i=0, st = 0; i < vertOfLine.size(); i++)
	{
		glDrawArrays(GL_LINE_STRIP, st, vertOfLine[i]);
		st += vertOfLine[i];
	}

	Graphic::Draw();
}
