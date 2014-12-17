#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include <vector>
#include <Eigen/Core>
#include<Eigen/StdVector>
using Eigen::Vector3f;
using Eigen::Vector3d;
using Eigen::Vector3i;

class Graphic
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public :
	Graphic();
	virtual void Draw();
	void SetVisible(bool v){ visible = v; }
	bool GetVisible(){ return visible;}

	//positionからdirectionのベクトルと衝突するかどうか, lengthには衝突した場合距離が入る
	virtual bool IsHit( const Vector3d &position, const Vector3d direction , double &length);

	void AddChild(Graphic *child);
	int ChildSize() const { return children.size(); }
	Graphic* const GetChild(const int &i){ return children[i]; }
	void DeleteChild(int i);
	
	void SetColor(float r, float g, float b);
	virtual ~Graphic();
protected:
	unsigned int buffer;
	std::vector<Vector3f, Eigen::aligned_allocator<Eigen::Vector3d>> vertices;
	void genVertexBuffer();
	void bindVertexBuffer();
private:
	bool visible;
	std::vector<Graphic*> children;
	Vector3f color;
};

//グリッド空間
class GridSpace : public Graphic
{
public:
	//中心位置, サイズ, 各軸の分割数
	GridSpace(Vector3f center, Vector3f size, Vector3i split);
	void Draw();
};

#endif