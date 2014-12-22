#ifndef _GEODESIC_H
#define _GEODESIC_H
#include "Graphics.h"
#include <Eigen/Core>
#include <vector>
class Field;

//���n��
class Geodesic : public Graphic
{
public:
	// �N���e�B�J���|�C���g��,���̎���̓_�Q��n��
	Geodesic(Field *field, const Eigen::Vector3d &criticalPoint, const std::vector<Eigen::Vector3d> &roundPoints);

private:
	//��_, �T���J�n�_, �t�̖@��
	Vector3d nextPoint(const Vector3d &basePoint, const Vector3d &start, const Vector3d &leafNormal);
	Vector3d BVP(std::vector<Vector3d> &Band0, std::vector<Vector3d> &Band1, const Vector3d &leaf);
	void Calc(Field *field, const Eigen::Vector3d &criticalPoint, const std::vector<Eigen::Vector3d> &roundPoints);

	enum{
		BandSize = 1000,
	};

	std::vector< std::vector<Eigen::Vector3d>> lines;

	Field *field;
};
#endif