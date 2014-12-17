#include "SigmaPlane.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Camera.h"
using Eigen::Vector3d;
using Eigen::Vector3cd;
using namespace std;

using namespace Eigen;
#include <Eigen/Geometry>
namespace
{
	Vector3d CompToReal(const Vector3cd &v)
	{
		return Vector3d(v.x().real(), v.y().real(), v.z().real());
	}

	//複素数成分を持っているか
	bool HasComplexElement(const Vector3cd &c){
		double error = 0.000001;
		return std::abs(c.x().imag()) > error ||
			   std::abs(c.y().imag()) > error ||
			   std::abs(c.z().imag()) > error;
	}
}

Vector3d SigmaPlane::Vec1() const
{
	return CompToReal(jacobian.eigenVector[index1]);
}

Vector3d SigmaPlane::Vec2() const
{
	return CompToReal(jacobian.eigenVector[index2]);
}

void SigmaPlane::GetRoundPoints(std::vector<Vector3d> &points) const
{
	points.clear();
	Vector3d vec = vec1.normalized() * Radius;
	for (int i = 0; i < PolygonSize; i++)
	{
		auto rot3d = AngleAxisd(2.0 * M_PI * i / (double)PolygonSize, normal);
		Vector3d p = (rot3d * vec);
		points.push_back(p + criticalPoint);
	}
}


SigmaPlane::SigmaPlane(const Jacobian &j, const Vector3d &cp)
	:jacobian(j), criticalPoint(cp), indexBuffer(0)
{
	MakePlane(j, cp);
}

void SigmaPlane::MakePlane(const Jacobian &jacob, const Eigen::Vector3d &cp)
{
	criticalPoint = cp;
	jacobian = jacob;

	//3つの固有値のうち, 実部が同符号のものを探す
	for (index1 = 0; index1<3; index1++)
	{
		index2 = (index1 + 1) % 3;

		if (jacobian.eigenValue[index1].real() * jacobian.eigenValue[index2].real() > 0)
		{
			break;
		}
	}

	//法線ベクトルを計算
	//Vector3cd nor = jacobian.eigenVector[index1].cross(jacobian.eigenVector[index2]);

	if (HasComplexElement(jacobian.eigenVector[index1])){
		vec1 = CompToReal(0.5 * (jacobian.eigenVector[index1] + jacobian.eigenVector[index2]));
		vec2 = CompToReal(complex<double>(0,-0.5)*(jacobian.eigenVector[index1] - jacobian.eigenVector[index2]));
	}
	else{
		vec1 = CompToReal(jacobian.eigenVector[index1]);
		vec2 = CompToReal(jacobian.eigenVector[index2]);
	}

	//複素ベクトルを実ベクトルへ変換
	//ここでは暫定的に,虚部を切り捨てることにする.
	normal = vec1.cross(vec2);
	//法線が0の場合平面ができない
	if (normal.isZero()){
		return;
	}

	normal.normalize();

	std::vector<Vector3d> roundPoints;

	const int VertexSize = PolygonSize + 1;
	const int IndexSize = PolygonSize * 4;
	if (vertices.size() != VertexSize)
		vertices.resize(VertexSize);

	if (indices.size() != IndexSize)
		indices.resize(IndexSize);

	vertices[0] = cp.cast<float>();	//クリティカルポイントを保存
	GetRoundPoints(roundPoints);
	for (int i = 0; i < roundPoints.size(); i++){
		vertices[i+1] = roundPoints[i].cast<float>();

		indices[4 * i] = 0;
		indices[4 * i + 1] = i + 1;
		indices[4 * i + 2] = i + 1;
		indices[4 * i + 3] = i == PolygonSize - 1 ? 1 : i + 2;
	}
}

void SigmaPlane::DebugMove(const Jacobian &j, const Eigen::Vector3d &cp)
{
	MakePlane(j, cp);
	//すでにバッファが作られているなら変更
	if (buffer){
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, (GLuint)(vertices.size() * sizeof(Vector3f)), vertices[0].data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	cout << "New CP " << criticalPoint.x() << "," << criticalPoint.y() << "," << criticalPoint.z() << endl;
}

#define BUFFER_OFFSET(bytes) ( (GLubyte*)NULL + (bytes))
void SigmaPlane::Draw()
{
	if (vertices.size() <= 1 || GetVisible() == false)
	{
		return;
	}

	if (!buffer)
		genVertexBuffer();

	if ( indexBuffer == 0 )
	{
		//bufferを生成
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLuint)(indices.size() * sizeof(unsigned int)), reinterpret_cast<GLfloat*>(&indices[0]), GL_STATIC_DRAW);
	}

	glPushAttrib(GL_COLOR);
	glColor3d(1, 0, 0);
	//矢印はカメラの方を向かせるため毎回計算
	//固有ベクトルを矢印で表現
	for (int i = 0; i < 3; i++)
	{
		//実空間に写像
		auto ev = Radius * CompToReal(jacobian.eigenVector[i]).normalized();
		auto nor = ev.cross( Camera::getCamera()->GetLook() - Camera::getCamera()->GetPosition()).normalized() * 0.1 * Radius;
		auto arrow1 = nor + 0.9*ev;
		auto arrow2 = arrow1 - 2 * nor;
		glBegin(GL_LINE_STRIP);
		glVertex3d(criticalPoint.x(), criticalPoint.y(), criticalPoint.z());
		glVertex3d(criticalPoint.x() + ev.x(), criticalPoint.y() + ev.y(), criticalPoint.z() + ev.z());
		glVertex3d(criticalPoint.x() + arrow1.x(), criticalPoint.y() + arrow1.y(), criticalPoint.z() + arrow1.z());
		glVertex3d(criticalPoint.x() + arrow2.x(), criticalPoint.y() + arrow2.y(), criticalPoint.z() + arrow2.z());
		glVertex3d(criticalPoint.x() + ev.x(), criticalPoint.y() + ev.y(), criticalPoint.z() + ev.z());
		glEnd();
	}

	glColor3d(0, 0, 1);
	
	//円を描画
	bindVertexBuffer();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopAttrib();

	Graphic::Draw();	//子を描画
}

bool SigmaPlane::IsHit( const Vector3d &position, const Vector3d direction , double &length)
{
	if( !GetVisible() )
		return false;

	Vector3d dir = criticalPoint - position;
	//面と反対方向に進んでいる場合はfalse
	if( direction.dot( dir ) < 0 )
		return false;

	float n  = normal.dot( dir );
	float n2 = normal.dot( direction );

	//平面と平行な場合は交わらない
	if( std::abs(n2) < 0.000000001 )
		return false;

	Vector3d p = position + direction * n / n2;
	length = (p-position).norm();
	float distance = (p-criticalPoint).norm();
	return distance < Radius;
}