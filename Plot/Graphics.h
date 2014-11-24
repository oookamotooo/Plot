#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include "Vector3.h"

#include <vector>

class Graphic
{
public :
	Graphic();
	virtual void Draw() = 0;
	void SetVisible(bool v){ visible = v; }
	bool GetVisible(){ return visible;}

	//positionからdirectionのベクトルと衝突するかどうか, lengthには衝突した場合距離が入る
	virtual bool IsHit( const Vector3d &position, const Vector3d direction , double &length);
protected:
	unsigned int buffer;
	std::vector<Vector3<float>> vertices;
	void genVertexBuffer();
	void bindVertexBuffer();
private:
	bool visible;
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