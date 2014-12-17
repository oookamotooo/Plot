#ifndef _SIGMA_PLANE_H_
#define _SIGMA_PLANE_H_

#include "Graphics.h"
#include "Jacobian.h"
#include "StreamLine.h"

//Σ面

class SigmaPlane : public Graphic
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
	// enumハック
	enum {
		Radius = 1,
		PolygonSize = 20
	};

	SigmaPlane(const Jacobian &j, const Eigen::Vector3d &criticalPoint);
	void Draw();
	bool HasPlane(){ return vertices.size() > 0; }

	Eigen::Vector3d CPoint() const { return criticalPoint; }
	Eigen::Vector3d Normal() const { return normal; }
	Eigen::Vector3d Vec1() const;
	Eigen::Vector3d Vec2() const;
	const Jacobian& Jacob() const{ return jacobian; }
	const int Index1() const { return index1; }
	const int Index2() const { return index2; }
	bool IsHit( const Vector3d &position, const Vector3d direction , double &length);

	void DebugMove(const Jacobian &j, const Eigen::Vector3d &criticalPoint);

	void GetRoundPoints(std::vector<Vector3d> &points) const;
	void MakePlane(const Jacobian &j, const Eigen::Vector3d &criticalPoint);
private:
	Jacobian jacobian;
	Vector3d criticalPoint;
	Vector3d normal;
	int index1, index2;	//平面を構成する固有ベクトルのインデックス
	Vector3d vec1, vec2;
	unsigned int indexBuffer;
	std::vector<unsigned int> indices;
};

#endif