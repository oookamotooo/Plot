#ifndef _SIGMA_PLANE_H_
#define _SIGMA_PLANE_H_

#include "Graphics.h"
#include "Jacobian.h"

//ƒ°–Ê
class SigmaPlane : public Graphic
{
public:
	static const int Radius = 2;
	SigmaPlane(Jacobian j, Vector3d criticalPoint);
	void Draw();

	bool HasPlane(){ return vertex.size() > 0; }

	const Vector3d& CPoint(){ return criticalPoint; }
	const Jacobian& Jacob(){ return jacobian; }
private:
	Jacobian jacobian;
	Vector3d criticalPoint;
	Vector3d normal;

	unsigned int indexBuffer;
	std::vector<unsigned int> indices;
};

#endif