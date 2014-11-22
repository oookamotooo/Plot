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


#endif