#ifndef _JACOBIAN_H_
#define _JACOBIAN_H_

#include <complex>
#include <iostream>
#include <Eigen/Core>
//ヤコビアンの情報を格納するクラス.

class Jacobian
{
public:
	std::complex<double> eigenValue[3];
	Eigen::Vector3cd eigenVector[3];
	Jacobian(std::complex<double> _eigenValue[3], Eigen::Vector3cd _eigenVector[3]);
	Jacobian(Eigen::Matrix3d jacobMatrix);
	Jacobian(){}
	friend std::ostream& operator<<(std::ostream& stream, const Jacobian &obj);
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif