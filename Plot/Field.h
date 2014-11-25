#ifndef _FIELD_H__
#define _FIELD_H__
#include "Vector3.h"
#include <vector>
#include "StreamLine.h"
#include "Graphics.h"
#include "SigmaPlane.h"
class Field : public Graphic
{
public:
	const Vector3i Size, LftBtmNear;
	std::vector<Vector3d> data;
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
	void Draw();

	void FildCP(std::vector<Vector3d> &criticalPoints);
private:

	//ルンゲクッタ法
	Vector3d rungeKutta(const Vector3d &delta, const double &step);

};

//インライン化するためにヘッダに記述
const int Field::Index(const int &i, const int &j, const int &k) const{
	return k*Size.x*Size.y + j*Size.x + i;
}

#endif