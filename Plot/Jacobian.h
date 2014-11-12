#ifndef _JACOBIAN_H_
#define _JACOBIAN_H_
#include <complex>
#include <iostream>
#include "Vector3.h"

//���R�r�A���̏����i�[����N���X.
class Jacobian
{
public :
	std::complex<double> eigenValue[3];				//3�̌ŗL�l
	Vector3< std::complex<double> >eigenVector[3];	//3�̌ŗL�x�N�g��

	Jacobian( std::complex<double> _eigenValue[3], Vector3<std::complex<double>> _eigenVector[3]);
	Jacobian(){}

	//���ޯ�ޗp
	// cout << jacobian�^�̕ϐ� << endl;
	// �ŌŗL�l,�ŗL�x�N�g����\���ł���.
	friend std::ostream& operator<<(std::ostream& stream, const Jacobian &obj);
};

#endif