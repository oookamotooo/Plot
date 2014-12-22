#include "Test.h"

#include <complex>
#include <iostream>
#include <vector>

#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Geometry>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

void Test::EigenTest()
{
	Eigen::Matrix2d a;
	a << 1, 2, -2, 1;
	Eigen::Matrix2cd b = a.cast<complex<double>>();
	cout << b << endl;
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix2cd> es(b);
	if (es.info() != Eigen::Success)
		cout << "Failed" << endl;
	else
	{
		cout << "Success " << endl;
		cout << es.eigenvalues()[0] << endl;
	}
}

void Test::VectorTest()
{
	vector<int> X = { 0, 1, 2, 3, 4, 5 };
	for (auto it = X.begin(); it != X.end(); it++){
		cout << (*it);
	}
	cout << endl;
	//auto p = X.insert(X.end(), X.begin()+3, X.begin()+4);
	X.push_back(X[4]);
	X.erase(X.begin() + 4, X.begin() + 5);
	for (int i = 0; i < X.size(); i++){
		cout << X[i];
	}
	cout << endl;
}