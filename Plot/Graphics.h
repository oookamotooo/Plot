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

	//position����direction�̃x�N�g���ƏՓ˂��邩�ǂ���, length�ɂ͏Փ˂����ꍇ����������
	virtual bool IsHit( const Vector3d &position, const Vector3d direction , double &length);
protected:
	unsigned int buffer;
	std::vector<Vector3<float>> vertices;
	void genVertexBuffer();
	void bindVertexBuffer();
private:
	bool visible;
};

//�O���b�h���
class GridSpace : public Graphic
{
public:
	//���S�ʒu, �T�C�Y, �e���̕�����
	//template<class T, typename S>
	GridSpace(Vector3<float> center, Vector3<float> size, Vector3i split);
	void Draw();
};

#endif