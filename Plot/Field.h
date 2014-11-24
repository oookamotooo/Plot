#ifndef _FIELD_H__
#define _FIELD_H__
#include "Vector3.h"
#include <vector>
#include "StreamLine.h"

class Field
{
public:
	const Vector3i Size;
	std::vector<Vector3d> data;
	Field(Vector3i size);
	Vector3d &Data(const int &i, const int &j, const int &k);
	Vector3d GetData(const double &i, const double &j, const double &k) const;
	Vector3d GetData(const Vector3d &pos) const;

	void CalcStreamLine( const Vector3d &criticalPoint, const std::vector<Vector3d> &rounds,  StreamLine &streamLine);
private:
	const int index(const int &i, const int &j, const int &k) const;

	//ルンゲクッタ法
	Vector3d rungeKutta(const Vector3d &delta, const double &step);

};

#endif