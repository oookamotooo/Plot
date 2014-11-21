#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include "Vector3.h"

#include <vector>
class Graphic
{
public :
	Graphic(){}
	virtual void Draw() = 0;

protected:
	unsigned int buffer;
	std::vector<Vector3<float>> vertex;
};

//グリッド空間
class GridSpace : public Graphic
{
public:
	//中心位置, サイズ, 各軸の分割数
	//template<class T, typename S>
	GridSpace(Vector3<float> center, Vector3<float> size, Vector3i split);
	void Draw();
};

#endif