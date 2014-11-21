#include "Primitive.h"
using namespace std;

SigmaPlane::SigmaPlane(Jacobian j)
	:jacobian(j)
{
	for(int a=0; a<3; a++)
	{
		int b = (a+1)%3;

		//3�̌ŗL�x�N�g���̂���, �������������̂��̂�T��
		if( jacobian.eigenValue[a].real() * jacobian.eigenValue[b].real() > 0 )
		{
			//�@���x�N�g�����v�Z
			Vector3< complex<double> > normal = jacobian.eigenVector[a].cross(jacobian.eigenVector[b]);
			//normal.normalize();
		}
	}
}