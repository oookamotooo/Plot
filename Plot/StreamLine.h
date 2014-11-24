#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "Graphics.h"
#include "Vector3.h"
#include <vector>
#include "Jacobian.h"

//ó¨ê¸
class StreamLine : public Graphic
{
	std::vector<unsigned int> vertOfLine;
public:
	void Draw();
	void AddPoint(Vector3d p);
	void BeginNewLine();
};


#endif