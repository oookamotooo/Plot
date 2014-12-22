#include "Field.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <Eigen/Geometry>
using std::cout;
using std::endl;

Field::Field(Vector3i lftBtmNear, Vector3i size)
	:Size(size), LftBtmNear(lftBtmNear), Delta(1, size.x(), size.x()*size.y())
{
	data.resize(size.x()*size.y()*size.z());

	planes[XPlane].resize(Size.y()*Size.z()*NumOfVertOfArrow);
	planes[YPlane].resize(Size.x()*Size.z()*NumOfVertOfArrow);
	planes[ZPlane].resize(Size.x()*Size.y()*NumOfVertOfArrow);

	planeKinds = XPlane;
	planeNo = Size.x() / 2;

	SetVisible(false);
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
	double dx = x - i;
	double dy = y - j;
	double dz = z - k;
		
	Vector3d res(0, 0, 0);
	const int index = Index(i, j, k);

	res += (1 - dx)*(1 - dy)*(1 - dz)* data[index];
	res += (1 - dx)*     dy *(1 - dz)* data[index + Delta.y()];
	res += (1 - dx)*(1 - dy)*     dz * data[index + Delta.z()];
	res += (1 - dx)*     dy *     dz * data[index + Delta.y() + Delta.z()];
	res +=      dx *(1 - dy)*(1 - dz)* data[index + Delta.x()];
	res +=      dx *     dy *(1 - dz)* data[index + Delta.x() + Delta.y()];
	res +=      dx *(1 - dy)*     dz * data[index + Delta.x() + Delta.z()];
	res +=      dx *     dy *     dz * data[index + Delta.x() + Delta.y() + Delta.z()];
	//forループによる展開は時間がかかりそうなのでやめる.
		/*
	for (int di = 0; di < 2; di++){
		double cx = (di == 0 ? (1.0 - dx) : dx);

		int deltaI = (di == 0 ? 0 : Delta.x());

		for (int dj = 0; dj < 2; dj++){
			double cy = (dj == 0 ? (1.0 - dy) : dy);

			for (int dk = 0; dk < 2; dk++){
				double cz = (dk == 0 ? (1.0 - dz) : dz);
				res +=  cx*cy*cz*data[Index(i + di, j + dj, k + dk)];
			}
		}
	}*/

	return res;
}

// 実数のインデックスでは線形補完により値を返す(ReadOnly)
Vector3d Field::GetData(const Vector3d &pos) const
{
	return GetData(pos.x(), pos.y(), pos.z());
}

Vector3d Field::rungeKutta(const Vector3d &p, const double &step)
{
	Vector3d del = GetData(p.x(), p.y(), p.z());

	const Vector3d k0 = step * del;
	del = GetData(p + k0/2.0);
	const Vector3d k1 = step*del;
	del = GetData(p + k1/2.0);
	const Vector3d k2 = step*del;
	del = GetData(p + k2/2.0);
	const Vector3d k3 = step*del;
	return (k0 + 2 * k1 + 2 * k2 + k3) / 6.0;
}

void Field::CalcStreamLine( const SigmaPlane &sigmaPlane, StreamLine &streamLine)
{
	Jacobian jacobian = sigmaPlane.Jacob();

	const double len = 0.1;
	const double step = jacobian.eigenValue[sigmaPlane.Index1()].real() < 0 ? -0.1 : 0.1;

	Vector3d vec    = sigmaPlane.Vec1();	//面を構成する固有ベクトルの1つ目を取得
	Vector3d normal = sigmaPlane.Normal();	//面の法線ベクトルを取得 
	bool reverse = step < 0;
	std::vector<Vector3d> roundPoints;
	sigmaPlane.GetRoundPoints(roundPoints);
	for (auto it = roundPoints.begin(); it != roundPoints.end(); it++){
		auto p = *it;
		streamLine.BeginNewLine();
		streamLine.AddPoint(p, reverse);
		while (true)
		{
			const int padding = 5;
			if (p.x() > Size.x() - padding || p.x() < padding ||
				p.y() > Size.y() - padding || p.y() < padding ||
				p.z() > Size.z() - padding || p.z() < padding)
				break;

			Vector3d delta = rungeKutta(p, step);
			delta.normalize();
			delta *= len;
			p += delta;
			streamLine.AddPoint(p, reverse);
		}
	}
}

void Field::CalcStreamLine(Vector3d p, StreamLine &streamLine, const double len, const double step)
{
	streamLine.BeginNewLine();
	streamLine.AddPoint(p);
	while (true)
	{
		const int padding = 5;
		if (p.x() > Size.x() - padding || p.x() < padding ||
			p.y() > Size.y() - padding || p.y() < padding ||
			p.z() > Size.z() - padding || p.z() < padding)
			break;

		Vector3d delta = rungeKutta(p, step);
		delta.normalize();
		p += delta*len;
		streamLine.AddPoint(p);
	}
}

Vector3d Field::StreamPoint(const Vector3d &initialPoint, const int n, const double step, const double len)
{
	Vector3d p = initialPoint;
	for (int i = 0; i < n; i++){
		const int padding = 5;
		if (p.x() > Size.x() - padding || p.x() < padding ||
			p.y() > Size.y() - padding || p.y() < padding ||
			p.z() > Size.z() - padding || p.z() < padding)
			break;

		Vector3d delta = rungeKutta(p, step);

		delta.normalize();
		p += delta*len;
	}

	return p;
}
void Field::DrawGrid(Vector3i lbn, Vector3i size, Vector3f color){
	glPushAttrib(GL_COLOR);
	glColor3f(color.x(), color.y(), color.z());
	for (unsigned int j = 0; j < 2; j++){
		float dy = j == 0 ? 0 : size.y();
		glBegin(GL_LINE_LOOP);
		glVertex3f(lbn.x(), lbn.y() + dy, lbn.z());
		glVertex3f(lbn.x() + size.x(), lbn.y() + dy, lbn.z());
		glVertex3f(lbn.x() + size.x(), lbn.y() + dy, lbn.z() + size.z());
		glVertex3f(lbn.x(), lbn.y() + dy, lbn.z() + size.z());
		glEnd();

		float dx = j == 0 ? 0 : size.y();
		glBegin(GL_LINE_LOOP);
		glVertex3f(lbn.x() + dx, lbn.y() + 0, lbn.z());
		glVertex3f(lbn.x() + dx, lbn.y() + size.y(), lbn.z());
		glVertex3f(lbn.x() + dx, lbn.y() + size.y(), lbn.z() + size.z());
		glVertex3f(lbn.x() + dx, lbn.y() + 0, lbn.z() + size.z());
		glEnd();
	}
	glPopAttrib();
}

void Field::PlaneNext()
{
	planeNo = (planeNo + 1) % Size(planeKinds);
	MakeVertices();
}

void Field::PlaneBack()
{
	planeNo = (planeNo -1 + Size(planeKinds)) % Size(planeKinds);
	MakeVertices();
}

void Field::ChangePlane(PlaneKinds kind)
{
	if (planeKinds == kind)
		return;

	planeKinds = kind;
	planeNo = Size(planeKinds) / 2;
	MakeVertices();
}

void Field::MakeVertices()
{	
	const float limitLength = 0.0001;	//これ以下は正規化しない
	switch (planeKinds)
	{
	case XPlane:
		for (int j = 0; j < Size.y(); j++){
			for (int k = 0; k < Size.z(); k++){
				int index = (j*Size.z() + k)*NumOfVertOfArrow;
				int i = planeNo;
				Vector3f gridPoint(1.0f*i, 1.0f*j, 1.0f*k);
				Vector3f data = Data(i, j, k).cast<float>();
				Vector3f dir = data.squaredNorm() <= limitLength ? data : data.normalized();
				planes[planeKinds][index]  = gridPoint;
				planes[planeKinds][index + 1] = gridPoint + dir;
			}
		}
		break;

	case YPlane:
		
		for (int i = 0; i < Size.x(); i++){
			for (int k = 0; k < Size.z(); k++){
				int index = (i*Size.z() + k)*NumOfVertOfArrow;
				int j = planeNo;
				Vector3f gridPoint(i, j, k);
				Vector3f data = Data(i, j, k).cast<float>();
				Vector3f dir = data.squaredNorm() <= limitLength ? data : data.normalized();
				planes[planeKinds][index] = gridPoint;
				planes[planeKinds][index + 1] = gridPoint + dir;
			}
		}
		break;
	case ZPlane:
		for (int i = 0; i < Size.x(); i++){
			for (int j = 0; j < Size.y(); j++){
				int index = (j*Size.x() + i)*NumOfVertOfArrow;
				int k = planeNo;
				Vector3f gridPoint(i, j, k);
				Vector3f data = Data(i, j, k).cast<float>();
				Vector3f dir = data.squaredNorm() <= limitLength ? data : data.normalized();
				planes[planeKinds][index] = gridPoint;
				planes[planeKinds][index + 1] = gridPoint + dir;
			}
		}
		break;
	}

	glBindBuffer(GL_ARRAY_BUFFER, indexBuffers[planeKinds]);
	glBufferData(GL_ARRAY_BUFFER, (GLuint)(planes[planeKinds].size() * sizeof(Vector3f)), planes[planeKinds].data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#define BUFFER_OFFSET(bytes) ( (GLubyte*)NULL + (bytes))
void Field::Draw()
{
	static bool initialDraw = true;
	if (initialDraw){
		//bufferを生成
		glGenBuffers(3, indexBuffers);
		for (int i = 0; i < 3; i++){
			glBindBuffer(GL_ARRAY_BUFFER, indexBuffers[i]);
			glBufferData(GL_ARRAY_BUFFER, (GLuint)(planes[i].size() * sizeof(Vector3f)), planes[i].data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		MakeVertices();
		initialDraw = false;
		return;
	}
	
	if (!GetVisible())
		return;

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffers[planeKinds]);
	glVertexPointer(3, GL_FLOAT, sizeof(Vector3f), BUFFER_OFFSET(0));
	glDrawArrays(GL_LINES, 0, planes[planeKinds].size());
	
	glDisableClientState(GL_VERTEX_ARRAY);

	Graphic::Draw();
}

namespace{

	//2点が同じ符号かどうが(errorは許容誤差)
	inline bool isSameSide2(const double &p1, const double &p2, const double &error)
	{
		if (p1 > error && p2 > error)
			return true;

		if (p1 < -error && p2 < -error)
			return true;

		return false;
	}

	//ベクトルp1, p2 において, 両者の符号が同じである成分がひとつでもあるか
	inline bool existSameSideElem2(const Vector3d &p1, const Vector3d &p2, const double &epsilon)
	{
		return isSameSide2(p1.x(), p2.x(), epsilon) ||
			isSameSide2(p1.y(), p2.y(), epsilon) ||
			isSameSide2(p1.z(), p2.z(), epsilon);
	}

	//4点が同じ符号かどうが(errorは許容誤差)
	inline bool isSameSide4(const double &p1, const double &p2, const double &p3, const double &p4, const double &error)
	{
		if (p1 > error && p2 > error && p3 > error && p4 > error)
			return true;

		if (p1 < -error && p2 < -error && p3 < -error && p4 < -error)
			return true;

		return false;
	}

	inline bool existSameSideElem4(const Vector3d &p1, const Vector3d &p2, const Vector3d &p3, const Vector3d &p4, const double &epsilon)
	{
		return isSameSide4(p1.x(), p2.x(), p3.x(), p4.x(), epsilon) ||
			isSameSide4(p1.y(), p2.y(), p3.y(), p4.y(), epsilon) ||
			isSameSide4(p1.z(), p2.z(), p3.z(), p4.z(), epsilon);
	}

	//8点が同じ符号かどうが(errorは許容誤差)
	inline bool isSameSide8(const double &p1, const double &p2, const double &p3, const double &p4,
		const double &p5, const double &p6, const double &p7, const double &p8,
		const double &error)
	{
		if (p1 > error && p2 > error && p3 > error && p4 > error &&
			p5 > error && p6 > error && p7 > error && p8 > error)
			return true;

		if (p1 < -error && p2 < -error && p3 < -error && p4 < -error &&
			p5 < -error && p6 < -error && p7 < -error && p8 < -error)
			return true;

		return false;
	}

	inline bool existSameSideElem8(const Vector3d &p1, const Vector3d &p2, const Vector3d &p3, const Vector3d &p4,
		const Vector3d &p5, const Vector3d &p6, const Vector3d &p7, const Vector3d &p8, const double &epsilon)
	{
		return isSameSide8(p1.x(), p2.x(), p3.x(), p4.x(), p5.x(), p6.x(), p7.x(), p8.x(), epsilon) ||
			isSameSide8(p1.y(), p2.y(), p3.y(), p4.y(), p5.y(), p6.y(), p7.y(), p8.y(), epsilon) ||
			isSameSide8(p1.z(), p2.z(), p3.z(), p4.z(), p5.z(), p6.z(), p7.z(), p8.z(), epsilon);
	}	
}

//ヤコビ行列の取得
void Field::GetJacobiMatrix(const Vector3i &point, Eigen::Matrix3d &res)
{
	GetJacobiMatrix(point.x(), point.y(), point.z(), res);
}

//ヤコビ行列の取得
void Field::GetJacobiMatrix(const int &i, const int &j, const int &k, Eigen::Matrix3d &res)
{
	res = Eigen::Matrix3d::Zero();
	Vector3d dx = 0.25*( Data(i + 1, j, k) - Data(i - 1, j, k) );
	Vector3d dy = 0.25*( Data(i, j + 1, k) - Data(i, j - 1, k) );
	Vector3d dz = 0.25*( Data(i, j, k + 1) - Data(i, j, k - 1) );

	res.row(0) << dx.transpose();	//1行目はx方向微分
	res.row(1) << dy.transpose();	//2行目はy
	res.row(2) << dz.transpose();	//3行目はz
}

void Field::GetJacobiMatrix(const Vector3d &p, Eigen::Matrix3d &mat)
{
	mat = Eigen::Matrix3d::Zero();
	
	//切り捨てた座標(グリッドの左下前の座標)
	Vector3i index((int)std::floor(p.x()), (int)std::floor(p.y()), (int)std::floor(p.z()));

	//index座標からの距離
	Vector3d c = p - index.cast<double>();
	mat = Eigen::Matrix3d::Zero();

	for (int l = 0; l < 2; l++){
		double cx = (l == 0 ? 1.0 - c.x() : c.x());

		for (int m = 0; m < 2; m++){
			double cy = (m == 0 ? 1.0 - c.y() : c.y());

			for (int n = 0; n < 2; n++){
				double cz = (n == 0 ? 1.0 - c.z() : c.z());

				Eigen::Matrix3d jacob;
				GetJacobiMatrix(index.x() + l, index.y() + m, index.z() + m, jacob);

				mat += cx*cy*cz*jacob;
			}
		}
	}
}

#include <algorithm>
bool Field::SubGridSearch(const Vector3d grid[2][2][2], Vector3d &delta)
{
	const int subGridNum = 500;			//サブグリッド分割数
	const double epsilon = 0.000001;	//許容誤差

	// x,y,z成分のうち, ひとつでも, 8点すべての符号が同じものがあれば, 
	//0ベクトルになることは ありえないのでfalseを返す
	if (existSameSideElem8(grid[0][0][0], grid[0][0][1], grid[0][1][0], grid[0][1][1],
		grid[1][0][0], grid[1][0][1], grid[1][1][0], grid[1][1][1], epsilon))
		return false;

	for (int i = 0; i < subGridNum; i++){
		double s = 1.0*i / (double)subGridNum;
		// x = s で切った面を求める
		Vector3d plane[2][2];
		for (int j = 0; j < 2; j++){
			for (int k = 0; k < 2; k++){
				plane[j][k] = grid[0][j][k] * (1.0 - s) + grid[1][j][k] * s;
			}
		}

		// 分割した平面の4点で, すべて同じ符号となる成分が存在すれば、0ベクトルにはならない
		if (existSameSideElem4(plane[0][0], plane[0][1], plane[1][0], plane[1][1], epsilon))
			continue;

		for (int j = 0; j < subGridNum; j++){
			double t = 1.0*j / (double)subGridNum;

			// y = t で切断した線分を求める
			Vector3d line[2];
			for (int k = 0; k < 2; k++){
				line[k] = plane[0][k] * (1.0 - t) + plane[1][k] * t;
			}

			//さらに分割した線分の両端2点で,どちらも同じ符号となる成分が存在すれば,0ベクトルはない
			if (existSameSideElem2(line[0], line[1], epsilon))
				continue;
			
			// ベクトルのx, y, z成分が許容誤差内にある. u(z方向距離)の変域をあらわす.
			// u_beginが範囲のはじまり(小さいほう), u_endが終わり(大きいほう)
			Vector3d dv = line[1] - line[0];
			Vector3d dEps_p =  epsilon*Vector3d(1, 1, 1) - line[0];
			Vector3d dEps_m = -epsilon*Vector3d(1, 1, 1) - line[0];
			Vector3d   u_end(dEps_p.x() / dv.x(), dEps_p.y() / dv.y(), dEps_p.z() / dv.z());
			Vector3d u_begin(dEps_m.x() / dv.x(), dEps_m.y() / dv.y(), dEps_m.z() / dv.z());

			if (u_end.x() < u_begin.x())
				std::swap(u_end.x(), u_begin.x());

			if (u_end.y() < u_begin.y())
				std::swap(u_end.y(), u_begin.y());

			if (u_end.z() < u_begin.z())
				std::swap(u_end.z(), u_begin.z());

			// vx, vy, vzがすべて誤差内にある変域を計算
			double u_st = std::max(u_begin.x(), std::max(u_begin.y(), u_begin.z()));
			double u_en = std::min(u_end.x(), std::min(u_end.y(), u_end.z()));

			if (u_st > u_en)
				continue;

			/*
			double u = u_st;
			//無理やり2分探索を行う
			while (true)
			{
				double u_cn = (u_st + u_en) / 2.0;

				double f_u_cn = (line[0] * (1.0 - u_cn) + u_cn * line[1]).norm();
				double f_u_st = (line[0] * (1.0 - u_st) + u_st * line[1]).norm();
				double f_u_en = (line[0] * (1.0 - u_en) + u_en * line[1]).norm();

				cout << f_u_cn << endl;
				// st, enの中点がst, enのどちらかより大きくなれば探索終了
				if (f_u_cn > f_u_st ||  f_u_cn > f_u_st){
					 u = f_u_st < f_u_en ? u_st : u_en;
					 break;
				}
				
				if (f_u_en > f_u_st){
					u_en = u_cn;
				}
				else{
					u_st = u_cn;
				}
			}
			*/
			//さらに100分割して絞り込む
			double u_range = u_en - u_st;
			double u = u_st;
			double f_min = (line[0] * (1.0 - u) + u * line[1]).norm();
			for (double _u = u_st; _u <= u_en; _u += u_range / 100.0){
				cout.precision(18);

				double f_u = (line[0] * (1.0 - _u) + _u * line[1]).norm();
				if (f_min > f_u){
					u = _u;
					f_min = f_u;
				}
			}
			delta = Vector3d(s, t, u);
			return true;
		}
	}
	return false;
}

void Field::SearchCP(std::vector<Vector3d> &criticalPoints)
{
	const double epsilon = 0.0001;

	for (int x = 0; x < Size.x() - 1; x++){
		for (int y = 0; y < Size.y() - 1; y++){
			for (int z = 0; z < Size.z() - 1; z++){
				Vector3d grid[2][2][2];

				for (int i = 0; i < 2; i++){
					for (int j = 0; j < 2; j++){
						for (int k = 0; k < 2; k++){
							grid[i][j][k] = Data(x + i, y + j, z + k);
						}
					}
				}

				
				Vector3d delta;
				
				if (SubGridSearch(grid, delta)){
					Vector3d cp(x + delta.x(), y + delta.y(), z + delta.z());
					//cout << "Newton Start "  << endl;
					//NewtonMethod(cp, 20);
					criticalPoints.push_back(cp);
				}

			}
		}
	}
}

#include <Eigen/LU>
bool Field::NewtonMethod(Vector3d &point,  int n)
{
	if (n == 0)
		return false;

	if (!InRegion(point.x(), point.y(), point.z()))
		return false;

	cout.precision(10);
	std::cout << "Newton" << point[0] << "," << point[1] << "," << point[2] << endl;

	auto f0 = GetData(point);
	std::cout << "=" << f0.norm() << " = ";
	std::cout << f0[0] << "," << f0[1] << "," << f0[2] << endl;

	Eigen::Matrix3d jacob;
	GetJacobiMatrix(point, jacob);
	Eigen::Matrix3d inv = jacob.inverse();
	point = point - inv*f0;
	return NewtonMethod(point, n - 1);
}


bool Field::InRegion(const double &x, const double &y, const double &z) const
{
	if (x < 0 || y < 0 || z < 0 || x >= Size.x() || y >= Size.y() || z >= Size.z())
		return false;

	return true;
}