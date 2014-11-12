#ifndef _JACOBIAN_H_
#define _JACOBIAN_H_
#include <complex>
#include <iostream>
#include "Vector3.h"

//���R�r�A���̏����i�[����N���X.
class Jacobian
{
public :
	std::complex<double> eigenValue[3];
	Vector3< std::complex<double> >eigenVector[3];
	Jacobian( std::complex<double> _eigenValue[3], Vector3<std::complex<double>> _eigenVector[3])
	{
		for(int i=0; i<3; i++)
		{
			eigenValue[i]  = _eigenValue[i];
			eigenVector[i] = _eigenVector[i]; 
		}
	}
	Jacobian(){}
	friend std::ostream& operator<<(std::ostream& stream, const Jacobian &obj);
};

#endif