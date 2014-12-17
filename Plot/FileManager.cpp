#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <Eigen/Core>
using Eigen::Vector3cd;
using Eigen::Vector3d;

using namespace std;

namespace 
{
	// 複素数値を3つ分読み込む
	inline bool ReadComplexLine(ifstream &stream, complex<double> res[3])
	{
		for(int i=0; i<3; i++)
		{
			double re, im;
			stream >> re >> im;
			res[i] = complex<double>(re,im);
			if( stream.eof() )
				return false;
		}

		return true;
	}

	// 実数値を3つ分読み込む
	inline bool ReadDoubleLine(ifstream &stream, double res[3])
	{
		for (int i = 0; i < 3; i++)
		{
			if (stream.eof())
				return false;

			double d;
			stream >> d;
			res[i] = d;
		}

		return true;
	}
}
	
bool FileManager::readJacobian(std::ifstream &stream, Jacobian &res)
{
	complex<double> eigen[3];

	//固有値を読み込む
	if ( !ReadComplexLine(stream, eigen) )
		return false;

	Vector3cd vec[3];
	complex<double> buf[3];

	//1行読み込んで,それぞれのx成分に代入
	if( !ReadComplexLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].x() = buf[i];

	//1行読み込んで,それぞれのy成分に代入
	if( !ReadComplexLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].y() = buf[i];

	//1行読み込んで,それぞれのz成分に代入
	if( !ReadComplexLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].z() = buf[i];

	res = Jacobian(eigen, vec);
	return true;
}

//ヤコビアンの読み込み
void FileManager::ReadJacobianData(const string fileName, vector<Jacobian> &res)
{
	ifstream in(fileName);
	res.reserve(20);
	if(!in)
	{
		cerr << "The file" << fileName << "was not opend" << endl; 
		return;
	}
	while(!in.eof())
	{
		Jacobian j;
		if( !readJacobian(in, j) )
			break;
		res.push_back(j);
	}

	in.close();
}

void FileManager::WriteSigmaPlaneData(const std::string fileName, const std::vector<SigmaPlane*> &sigmas)
{
	ofstream out(fileName);
	if (!out){
		cerr << "The file" << fileName << "was not opened" << endl;
		return;
	}

	for (auto it = sigmas.begin(); it != sigmas.end(); it++){
		auto cp = (*it)->CPoint();
		out << cp.x() << " " << cp.y() << " " << cp.z() << endl;

		auto jac = (*it)->Jacob();
		for (int i = 0; i < 3; i++)
			out << jac.eigenValue[i].real() << " " << jac.eigenValue[i].imag() << " ";
		out << endl;
		for (int i = 0; i < 3; i++)
			out << jac.eigenVector[i].x().real() << " " << jac.eigenVector[i].x().imag() << " ";
		out << endl;
		for (int i = 0; i < 3; i++)
			out << jac.eigenVector[i].y().real() << " " << jac.eigenVector[i].y().imag() << " ";
		out << endl;
		for (int i = 0; i < 3; i++)
			out << jac.eigenVector[i].z().real() << " " << jac.eigenVector[i].z().imag() << " ";
		out << endl;
	}
}

void FileManager::ReadSigmaPlaneData(const string fileName, std::vector<SigmaPlane*> &sigma)
{
	ifstream in(fileName);
	if (!in)
	{
		cerr << "The file" << fileName << "was not opened" << endl;
		return;
	}

	while (!in.eof())
	{
		double buf[3];

		if (!ReadDoubleLine(in, buf))
			break;

		Vector3d cp(buf[0], buf[1], buf[2]);

		Jacobian j;
		if (!readJacobian(in, j))
			break;

		auto s = new SigmaPlane(j, cp);
		if (s->HasPlane()){
			sigma.push_back(s);
		}
		else{
			delete s;
		}
	}
}

//クリティカルポイントの読み込み
void FileManager::ReadCriticalPointData(const string fileName, vector<Vector3d, Eigen::aligned_allocator<Vector3d>> &res)
{
	res.clear();
	ifstream in(fileName);
	if (!in)
	{
		cerr << "The file" << fileName << "was not opend" << endl;
		return;
	}

	while (!in.eof())
	{
		double buf[3];

		if (!ReadDoubleLine(in, buf))
			break;

		res.push_back(Vector3d(buf[0], buf[1], buf[2]));
	}

	in.close();
	return;
}

//磁気圏のデータの読み込み.
void FileManager::ReadFieldData(const string fileName, Field *field)
{
	FILE *fp = fopen(fileName.c_str(), "r");
	// X, Y, Zの方向に格納されている.
	for (int k = 0; k < field->Size.z(); k++){
		for (int j = 0; j < field->Size.y(); j++){
			for (int i = 0; i < field->Size.x(); i++){
				double res[3];

				fscanf(fp,"%lf %lf %lf", &res[0], &res[1], &res[2]);

				field->Data(i, j, k) = Vector3d(res[0], res[1], res[2]);
			}
		}
	}
	fclose(fp);
}
