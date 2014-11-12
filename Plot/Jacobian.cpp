#include "Jacobian.h"
#include <iostream>
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