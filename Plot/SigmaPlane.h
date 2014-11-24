#ifndef _SIGMA_PLANE_H_
#define _SIGMA_PLANE_H_

#include "Graphics.h"
#include "Jacobian.h"
#include "StreamLine.h"

//Σ面
class SigmaPlane : public Graphic
{
public:
	const double Radius;
	SigmaPlane(Jacobian j, Vector3d criticalPoint);
	void Draw();
	bool HasPlane(){ return vertices.size() > 0; }

	Vector3d CPoint() const { return criticalPoint; } 
	Vector3d Normal() const { return normal; }
	Vector3d Vec1() const;
	Vector3d Vec2() const;
	const Jacobian& Jacob() const{ return jacobian; }
	const int Index1() const { return index1; }
	const int Index2() const { return index2; }

	void AddStreamLine(StreamLine *streamLine){ streamLines.push_back(streamLine);}

	bool IsHit( const Vector3d &position, const Vector3d direction , double &length);
private:
	Jacobian jacobian;
	Vector3d criticalPoint;
	Vector3d normal;
	int index1, index2;	//平面を構成する固有ベクトルのインデックス
	unsigned int indexBuffer;
	std::vector<unsigned int> indices;
	std::vector<StreamLine*> streamLines;
};

#endif