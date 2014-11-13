#ifndef _JACOBIAN_H_
#define _JACOBIAN_H_
#include <complex>
#include <iostream>
#include "Vector3.h"

//ヤコビアンの情報を格納するクラス.
class Jacobian
{
public :
	std::complex<double> eigenValue[3];				//3つの固有値
	Vector3< std::complex<double> >eigenVector[3];	//3つの固有ベクトル

	Jacobian( std::complex<double> _eigenValue[3], Vector3<std::complex<double>> _eigenVector[3]);
	Jacobian(){}

	//ﾃﾞﾊﾞｯｸﾞ用
	// cout << jacobian型の変数 << endl;
	// で固有値,固有ベクトルを表示できる.
	friend std::ostream& operator<<(std::ostream& stream, const Jacobian &obj);
};

#endif