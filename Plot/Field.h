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
	const Vector3i Size;
	std::vector<Vector3d> data;
	Field(Vector3i size);
	Field(int sx, int sy, int sz);
	Vector3d &Data(const int &i, const int &j, const int &k);
	Vector3d &Data(const int &index);

	Vector3d GetData(const double &i, const double &j, const double &k) const;
	Vector3d GetData(const Vector3d &pos) const;
	const int Index(const int &i, const int &j, const int &k) const;

	void CalcStreamLine( const SigmaPlane &sigmaPlane, StreamLine &streamLine);

	void Draw();
private:

	//ルンゲクッタ法
	Vector3d rungeKutta(const Vector3d &delta, const double &step);

};

#endif