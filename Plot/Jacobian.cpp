#include "Jacobian.h"
#include <iostream>
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Dense>
using namespace std;

ostream& operator<<(ostream &stream, const Jacobian &obj)
{
	stream << "eigenValue--------------------" << endl;
	stream << obj.eigenValue[0] << " , " << obj.eigenValue[1] << " , " << obj.eigenValue[2] << endl; 
	stream << "eigenVector--------------------" << endl;
	for(int i=0; i<3; i++)
		stream << obj.eigenVector[i] << endl;
	stream << "------------------------------------" << endl;

	return stream;
}

Jacobian::Jacobian(std::complex<double> _eigenValue[3], Eigen::Vector3cd _eigenVector[3])
{
	for(int i=0; i<3; i++){
		eigenValue[i]  = _eigenValue[i];
		eigenVector[i] = _eigenVector[i]; 
	}
}

Jacobian::Jacobian(Eigen::Matrix3d jacobMatrix)
{
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3cd> es(jacobMatrix.cast<complex<double>>());

	for (int i = 0; i < 3; i++){
		eigenValue[i] = es.eigenvalues()[i];
		eigenVector[i] = es.eigenvectors().row(i);
	}
}