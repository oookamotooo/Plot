#include "Primitive.h"
using namespace std;

SigmaPlane::SigmaPlane(Jacobian j)
	:jacobian(j)
{
	for(int a=0; a<3; a++)
	{
		int b = (a+1)%3;

		//3つの固有ベクトルのうち, 実部が同符号のものを探す
		if( jacobian.eigenValue[a].real() * jacobian.eigenValue[b].real() > 0 )
		{
			//法線ベクトルを計算
			Vector3< complex<double> > normal = jacobian.eigenVector[a].cross(jacobian.eigenVector[b]);
			//normal.normalize();
		}
	}
}