#include "Primitive.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Camera.h"
using namespace std;

namespace
{
	template <class S>
	static float CLength(Vector3<complex<S>> v)
	{
		return sqrt(norm(v.x) + norm(v.y) + norm(v.z));
	}

	Vector3d rotate(Vector3d axis, Vector3d vec, double alpha){
		Vector3d result;
		double theta;

		theta = alpha * M_PI / 180.0;

		double C = cos(theta);
		double S = sin(theta);
		result.x = ((axis.x*axis.x*(1 - C) + C)*vec.x + (axis.x*axis.y*(1 - C) - axis.z*S)*vec.y + (axis.z*axis.x*(1 - C) + axis.y*S)*vec.z);
		result.y = ((axis.x*axis.y*(1 - C) + axis.z*S)*vec.x + (axis.y*axis.y*(1 - C) + C)*vec.y + (axis.y*axis.z*(1 - C) - axis.x*S)*vec.z);
		result.z = ((axis.z*axis.x*(1 - C) - axis.y*S)*vec.x + (axis.y*axis.z*(1 - C) + axis.x*S)*vec.y + (axis.z*axis.z*(1 - C) + C)*vec.z);

		return result;
	}

	template<class T>
	Vector3<T> CompToReal(Vector3<complex<T>> v)
	{
		return Vector3<T>(v.x.real(), v.y.real(), v.z.real());
	}

	Vector3<float> ToFloatVector(Vector3d v)
	{
		return Vector3<float>(v.x, v.y, v.z);
	}
}

SigmaPlane::SigmaPlane(Jacobian j, Vector3d cp)
:jacobian(j), criticalPoint(cp)
{
	bool found = false;
	int a, b;
	for(a=0; a<3; a++)
	{
		b = (a+1)%3;

		//3つの固有ベクトルのうち, 実部が同符号のものを探す
		if( jacobian.eigenValue[a].real() * jacobian.eigenValue[b].real() > 0 )
		{
			found = true;
			break;
		}
	}

	if (found)
	{
		//法線ベクトルを計算
		Vector3< complex<double> > nor = jacobian.eigenVector[a].cross(jacobian.eigenVector[b]);

		//複素ベクトルを実ベクトルへ変換
		//ここでは暫定的に,虚部を切り捨てることにする.
		normal = CompToReal(nor).normalize();
		Vector3d rVec1  = CompToReal(jacobian.eigenVector[a]).normalize() * Radius;
		for (int i = 0; i < 12; i++)
		{
			Vector3d p = rotate(normal, rVec1, 360.0 * i / 12);
			vertex.push_back( ToFloatVector(p + cp));
		}


		//bufferを生成
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, (GLuint)(vertex.size() * sizeof(Vector3<float>)), reinterpret_cast<GLfloat*>(&vertex[0]), GL_STATIC_DRAW);
	}
	else
	{
		//実部が同符号の物が見つからなかった場合
	}

}

#define BUFFER_OFFSET(bytes) ( (GLubyte*)NULL + (bytes))
void SigmaPlane::Draw()
{
	if (vertex.size() == 0)
		return;

	glPushAttrib(GL_COLOR);
	glColor3d(1, 0, 0);
	//矢印はカメラの方を向かせるため毎回計算
	//固有ベクトルを矢印で表現
	for (int i = 0; i < 3; i++)
	{
		//実空間に写像
		auto ev        =  Radius * ToFloatVector( CompToReal(jacobian.eigenVector[i])).normalize();
		auto nor       = ev.cross( ToFloatVector( Camera::getCamera()->GetLook() - Camera::getCamera()->GetPosition())).normalize() * 0.1 * Radius;
		auto arrow1    = nor + 0.9*ev;
		auto arrow2    = arrow1 - 2 * nor;
		glBegin(GL_LINE_STRIP);
		glVertex3d(criticalPoint.x           , criticalPoint.y           , criticalPoint.z);
		glVertex3d(criticalPoint.x + ev.x    , criticalPoint.y + ev.y    , criticalPoint.z + ev.z);
		glVertex3d(criticalPoint.x + arrow1.x, criticalPoint.y + arrow1.y, criticalPoint.z + arrow1.z);
		glVertex3d(criticalPoint.x + arrow2.x, criticalPoint.y + arrow2.y, criticalPoint.z + arrow2.z);
		glVertex3d(criticalPoint.x + ev.x    , criticalPoint.y + ev.y    , criticalPoint.z + ev.z);
		glEnd();
	}

	glColor3d(0, 0, 0);
	//円を描画
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexPointer(3, GL_FLOAT, sizeof(Vector3<float>), BUFFER_OFFSET(0));

	glColor3d(0, 0, 0);
	glDrawArrays(GL_LINE_LOOP, 0, vertex.size());

	glEnableClientState(GL_VERTEX_ARRAY);

	glPopAttrib();
}