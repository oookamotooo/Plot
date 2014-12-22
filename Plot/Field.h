#ifndef _FIELD_H__
#define _FIELD_H__
#include "Vector3.h"
#include <vector>
#include "StreamLine.h"
#include "Graphics.h"
#include "SigmaPlane.h"
#include <Eigen/Core>
#include <Eigen/StdVector>
using Eigen::Vector3d;
using Eigen::Vector3i;

class Field : public Graphic
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
public:
	const Vector3i Size, LftBtmNear, Delta;
	Field(Vector3i center, Vector3i size);

	//フィールドのデータを取得(代入可能)
	Vector3d &Data(const int &i, const int &j, const int &k);
	Vector3d &Data(const int &index);

	//線形補完を用いることで実数インデックスのデータも取得可能
	Vector3d GetData(const double &i, const double &j, const double &k) const;
	Vector3d GetData(const Vector3d &pos) const;
	const inline int Index(const int &i, const int &j, const int &k) const;

	//Σ面から出る流線を計算する
	void CalcStreamLine(const SigmaPlane &sigmaPlane, StreamLine &streamLine);

	//ある点から出る流線を計算する.
	void CalcStreamLine(Vector3d start, StreamLine &streamLine, const double len = 0.1, const double step = 0.5);

	//ある点から流線を流したnステップ後の位置を取得する.
	// step 流入 or 流出を含めたルンゲクッタ法に用いる係数, len = 次の線までの長さ
	Vector3d StreamPoint(const Vector3d &initialPoint, const int n, const double step = 0.5, const double len = 0.1);

	void Draw();

	//クリティカルポイントを探す
	void SearchCP(std::vector<Vector3d> &criticalPoints);

	enum PlaneKinds{
		XPlane,
		YPlane,
		ZPlane
	};

	void PlaneNext();
	void PlaneBack();

	void ChangePlane(PlaneKinds kind);

	//ヤコビアンの導出
	void GetJacobiMatrix(const Vector3d &criticalPoint, Eigen::Matrix3d &res);
	void GetJacobiMatrix(const Vector3i &point, Eigen::Matrix3d &res);
	void GetJacobiMatrix(const int &i, const int &j, const int &k, Eigen::Matrix3d &res);
	bool NewtonMethod(Vector3d &initialPoint, int n);

	//void DrawGrid(Vector3i lbn, Vector3f color = Vector3f(1,0,0));
	void DrawGrid(Vector3i lbn, Vector3i size = Vector3i(1, 1, 1), Vector3f color = Vector3f(1, 0, 0));

	bool InRegion(const double &x, const double &y, const double &z) const;

	//ルンゲクッタ法
	Vector3d rungeKutta(const Vector3d &delta, const double &step);

private:
	std::vector<Vector3d, Eigen::aligned_allocator<Vector3d> > data;

	//グリッド内を探す.
	bool SubGridSearch(const Vector3d grid[2][2][2], Vector3d &delta);

	void MakeVertices();

	std::vector<Vector3f> planes[3];
	unsigned int indexBuffers[3];
	PlaneKinds planeKinds;
	int planeNo;
	const int NumOfVertOfArrow = 2;
};

//インライン化するためにヘッダに記述
const int Field::Index(const int &i, const int &j, const int &k) const{
	return k*Delta.z() + j*Delta.y() + i;
	//return k*Size.x()*Size.y() + j*Size.x() + i;
}

#endif