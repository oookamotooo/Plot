#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "Graphics.h"
#include "Vector3.h"
#include <vector>
#include "Jacobian.h"

//ó¨ê¸
class StreamLine : public Graphic
{
	std::vector<Vector3d> points;
public:
	void Draw();
	void AddPoint(Vector3d p);
};

//É∞ñ 
class SigmaPlane : public Graphic
{
public:
	static const int Radius = 2;
	SigmaPlane(Jacobian j, Vector3d criticalPoint);
	void Draw();
private:
	Jacobian jacobian;
	Vector3d criticalPoint;
	Vector3d normal;
};

#endif