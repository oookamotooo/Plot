﻿#include "Field.h"

Field::Field(Vector3i size)
:Size(size)
{
	data.resize(size.x*size.y*size.z);
}

Field::Field(int sx, int sy, int sz)
:Size(sx, sy, sz)
{
	data.resize(Size.x*Size.y*Size.z);
}

const int Field::Index(const int &i, const int &j, const int &k) const
{
	return k*Size.x*Size.y + j*Size.x + i;
}

//指定したインデックスの値を返す(代入可能)
Vector3d& Field::Data(const int &i, const int &j, const int &k)
{
	return data[Index(i, j, k)];
}

//指定したインデックスの値を返す(代入可能)
Vector3d& Field::Data(const int &index)
{
	return data[index];
}

// 実数のインデックスでは線形補完により値を返す(ReadOnly)
Vector3d Field::GetData(const double &x, const double &y, const double &z) const
{
	int i = floor(x);
	int j = floor(y);
	int k = floor(z);
	float dx = x - i;
	float dy = y - j;
	float dz = z - k;

	Vector3d res(0, 0, 0);
	for (int di = 0; di < 2; di++){
		double cx = (di == 0 ? (1.0 - dx) : dx);

		for (int dj = 0; dj < 2; dj++){
			double cy = (dj == 0 ? (1.0 - dy) : dy);
			for (int dk = 0; dk < 2; dk++){
				double cz = (dk == 0 ? (1.0 - dz) : dz);
				res.add( cx*cy*cz*data[Index(i + di, j + dj, k + dk)] );
			}
		}
	}

	return res;
}
// 実数のインデックスでは線形補完により値を返す(ReadOnly)
Vector3d Field::GetData(const Vector3d &pos) const
{
	return GetData(pos.x, pos.y, pos.z);
}

namespace
{
	/*
	//単純に引数として座標(小数点含め)を与える形にしたほうが反復処理のとき楽
	//与えられた座標からタイムステップ後の座標を返す関数
	double streamline(double x, double y, double z, int i){

		double test_x[8], test_y[8], test_z[8];
		int x_i, y_i, z_i;
		double x_d, y_d, z_d;

		//printf("%f %f %f\n", x, y, z);

		
		//float x_i = floor(x);
		//float x_d = x - x_i;


		//xyz座標からijkの値とstuの値を出す。

		x_i = floor(x);
		x_d = x - x_i;
		y_i = floor(y);
		y_d = y - y_i;
		z_i = floor(z);
		z_d = z - z_i;

		
		//x_d = modf(x, &x_i);
		//y_d = modf(y, &y_i);
		//z_d = modf(z, &z_i);
	
		//それぞれに対応する線形補完値を返

		//printf("aaaaaaaaaaaaaaaaa\n");
		//printf("%d %d %d, %f %f %f\n", x_i, y_i, z_i, x_d, y_d, z_d);
		//if(x_i+1 > sizeX || y_i+1 > sizeY || z_i+1 > sizeZ) return 0;
		//for(int i=0; i < 1; i++){
		//inTotestでは-1-1-1までいれるので、+1+1+1しておく
		//printf("into %d %d %d\n", x_i+1, y_i+1, z_i+1);
		inTotest(x_i + 1, y_i + 1, z_i + 1, test_x, test_y, test_z);
		switch (i){
		case 1:
			return interpolate(x_d, y_d, z_d, test_x);
		case 2:
			return interpolate(x_d, y_d, z_d, test_y);
		default:
			return interpolate(x_d, y_d, z_d, test_z);
		}
	}*/

}

Vector3d Field::rungeKutta(const Vector3d &p, const double &step)
{
	Vector3d del = GetData(p.x, p.y, p.z);

	const Vector3d k0 = step * del;
	del = GetData(p + k0/2.0);

	const Vector3d k1 = step*del;
	del = GetData(p + k1/2.0);

	Vector3d k2 = step*del;
	del = GetData(p + k2/2.0);

	Vector3d k3 = step*del;
	
	return (k0 + 2 * k1 + 2 * k2 + k3) / 6.0;
}

void Field::CalcStreamLine( const SigmaPlane &sigmaPlane, StreamLine &streamLine)
{
	Jacobian jacobian = sigmaPlane.Jacob();

	const double first_len = sigmaPlane.Radius;
	const double len = 0.1;
	const double step = jacobian.eigenValue[sigmaPlane.Index1()].real() < 0 ? -0.1 : 0.1;

	Vector3d vec    = sigmaPlane.Vec1();	//面を構成する固有ベクトルの1つ目を取得
	Vector3d normal = sigmaPlane.Normal();	//面の法線ベクトルを取得 
	for(int s = 0; s < 19; s++)
	{
		Vector3d v =  vec.rotatedVector(normal, 360.0 / 20.0 * s );
		v.normalize();

		Vector3d p = sigmaPlane.CPoint() + v * first_len;
		streamLine.BeginNewLine();
		streamLine.AddPoint(p);
		while (true)
		{
			const int padding = 5;
			if (p.x > Size.x - padding || p.x < padding ||
				p.y > Size.y - padding || p.y < padding ||
				p.z > Size.z - padding || p.z < padding)
				break;

			Vector3d delta = rungeKutta(p, step);
			delta.normalize().mul(len);

			p.add(delta);
			streamLine.AddPoint(p);
		}
	}
}

void Field::Draw()
{

}