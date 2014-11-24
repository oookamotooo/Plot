#include "Jacobian.h"
#include <iostream>
using namespace std;

namespace
{

}

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

Jacobian::Jacobian( std::complex<double> _eigenValue[3], Vector3<std::complex<double>> _eigenVector[3])
{
	for(int i=0; i<3; i++){
		eigenValue[i]  = _eigenValue[i];
		eigenVector[i] = _eigenVector[i]; 
	}
}
