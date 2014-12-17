#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "Graphics.h"
#include <vector>
#include "Jacobian.h"
#include <Eigen/Core>
using Eigen::Vector3d;

//流線
class StreamLine : public Graphic
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	std::vector<unsigned int> vertOfLine;
public:
	void Draw();
	void AddPoint(const Vector3d &p, const bool reverse = false);
	void BeginNewLine();
};


#endif