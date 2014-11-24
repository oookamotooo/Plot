#include "Field.h"

Field::Field(Vector3i size)
:Size(size)
{
	data.reserve(size.x*size.y*size.z);
}

const int Field::index(const int &i, const int &j, const int &k) const
{
	return k*Size.x*Size.y + j*Size.x + i;
}

//指定したインデックスの値を返す(代入可能)
Vector3d& Field::Data(const int &i, const int &j, const int &k)
{
	return data[index(i, j, k)];
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
				res.add( cx*cy*cz*data[index(i + di, j + dj, k + dk)] );
			}
		}
	}

	return res;
}
// 実数のインデックスでは線形補完により値を返す(ReadOnly)
Vector3d Field::GetData(const Vector3d &pos) const
{
	int i = floor(pos.x);
	int j = floor(pos.y);
	int k = floor(pos.z);
	float dx = pos.x - i;
	float dy = pos.y - j;
	float dz = pos.z - k;

	Vector3d res(0, 0, 0);
	for (int di = 0; di < 2; di++){
		double cx = (di == 0 ? (1.0 - dx) : dx);

		for (int dj = 0; dj < 2; dj++){
			double cy = (dj == 0 ? (1.0 - dy) : dy);
			for (int dk = 0; dk < 2; dk++){
				double cz = (dk == 0 ? (1.0 - dz) : dz);
				res.add(cx*cy*cz*data[index(i + di, j + dj, k + dk)]);
			}
		}
	}

	return res;
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
Vector3d Field::rungeKutta(const Vector3d &delta, const double &step)
{
	Vector3d del = GetData(delta.x, delta.y, delta.z);

	const Vector3d k0 = step * del;

	//xの偏微分
	del.x = GetData(delta + Vector3d(k0.x, k0.x, k0.x)).x;	// TODO : x成分はstepではないのか？
	del.y = GetData(delta + Vector3d(k0.y, k0.y, k0.y)).y;
	del.z = GetData(delta + Vector3d(k0.z, k0.z, k0.z)).z;
	const Vector3d k1 = step*del;

	del.x = GetData(delta + Vector3d(k1.x, k1.x, k1.x)).x;
	del.y = GetData(delta + Vector3d(k1.y, k1.y, k1.y)).y;
	del.z = GetData(delta + Vector3d(k1.z, k1.z, k1.z)).z;
	Vector3d k2 = step*del;

	del.x = GetData(delta + Vector3d(k2.x, k2.x, k2.x)).x;
	del.y = GetData(delta + Vector3d(k2.y, k2.y, k2.y)).y;
	del.z = GetData(delta + Vector3d(k2.z, k2.z, k2.z)).z;
	Vector3d k3 = step*del;

	return (k0 + 2 * k1 + 2 * k2 + k3) / 6.0;

	/*
	//printf("runge\n");
	del = streamline(x, y, z, i);
	k0 = step*del;
	//printf("k0:%f\n", k0);
	del = streamline((x + k0 / 2.0), (y + k0 / 2.0), (z + k0 / 2.0), i);
	k1 = step*del;
	//printf("k1:%f\n", k1);
	del = streamline((x + k1 / 2.0), (y + k1 / 2.0), (z + k1 / 2.0), i);
	k2 = step*del;
	//printf("k2:%f\n", k2);
	del = streamline((x + k2), (y + k2), (z + k2), i);
	k3 = step*del;
	//printf("k3:%f\n", k3);
	return ((k0 + 2 * k1 + 2 * k2 + k3) / 6.0);
	*/
}
void Field::CalcStreamLine(const Vector3d &criticalPoint, const std::vector<Vector3d> &rounds, StreamLine &streamLine)
{

	const double first_len = 0.5;
	const double len = 0.1;
	const double step = 0.5;

	for (int s = 0; s < rounds.size()-1; s++)
	{
		Vector3d delta = criticalPoint + rounds[s] * first_len;
		streamLine.AddPoint(delta);
		while (true)
		{
			const int padding = 5;
			if (delta.x > Size.x - padding || delta.x < padding ||
				delta.y > Size.y - padding || delta.y < padding ||
				delta.z > Size.z - padding || delta.z < padding)
				break;

			Vector3d delta2 = rungeKutta(delta, step);
			delta2.normalize().mul(len);

			delta.add(delta2);
			streamLine.AddPoint(delta);
			//TODO : round間がつながってしまう。
		}
	}

	/*
	for (int s = 0; s<round_num - 1; s++){
		dx1 = cp[t].x + round_cp[t][s].x*(first_len);
		dy1 = cp[t].y + round_cp[t][s].y*(first_len);
		dz1 = cp[t].z + round_cp[t][s].z*(first_len);

		while (true){
			if (dx1 > sizeX - 5 || dx1 < 5 || dy1 > sizeY - 5 || dy1 < 5 || dz1 > sizeZ - 5 || dz1 < 5)
				break;

			dx2 = runge_kutta(dx1, dy1, dz1, 1, step);
			dy2 = runge_kutta(dx1, dy1, dz1, 2, step);
			dz2 = runge_kutta(dx1, dy1, dz1, 3, step);

			d_size = sqrt(dx2*dx2 + dy2*dy2 + dz2*dz2);

			dx2 = (dx2 / d_size)*len;
			dy2 = (dy2 / d_size)*len;
			dz2 = (dz2 / d_size)*len;

			glBegin(GL_LINES);
			glVertex3f(dx1 - (double)sizeX / 2.0, dy1 - (double)sizeY / 2.0, dz1 - (double)sizeZ / 2.0);
			glVertex3f((dx1 - (double)sizeX / 2.0) + dx2, (dy1 - (double)sizeY / 2.0) + dy2, (dz1 - (double)sizeZ / 2.0) + dz2);
			glEnd();

			dx1 += dx2;
			dy1 += dy2;
			dz1 += dz2;
		}
	}*/
}