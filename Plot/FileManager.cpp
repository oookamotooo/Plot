#include "FileManager.h"
#include <fstream>
#include <iostream>
#include "Vector3.h"
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

	Vector3<complex<double>> vec[3];
	complex<double> buf[3];

	//1行読み込んで,それぞれのx成分に代入
	if( !ReadComplexLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].x = buf[i];

	//1行読み込んで,それぞれのy成分に代入
	if( !ReadComplexLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].y = buf[i];

	//1行読み込んで,それぞれのz成分に代入
	if( !ReadComplexLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].z = buf[i];

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

//クリティカルポイントの読み込み
void FileManager::ReadCritialPointData(const string fileName, vector<Vector3d> &res)
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
void FileManager::ReadFieldData(const string fileName, Field &field)
{
	/*
	ifstream in(fileName);
	if (!in){
		cerr << "The file" << fileName << "was not opend" << endl;
		return;
	}
	*/
	FILE *fp = fopen(fileName.c_str(), "r");
	// X, Y, Zの方向に格納されている.
	for (int k = 0; k < field.Size.z; k++){
		for (int j = 0; j < field.Size.y; j++){
			for (int i = 0; i < field.Size.x; i++){
				double res[3];
				/*
				if( !ReadDoubleLine(in, res) )
				{
					cout << "データ数が合いません" << endl;
					exit(2);
				}*/
				fscanf(fp,"%lf %lf %lf", &res[0], &res[1], &res[2]);

				field.Data(i, j, k).set(res[0], res[1], res[2]);
			}
		}
	}

	fclose(fp);
	//in.close();
}

void FileManager::ReadRoundData(const std::string fileName, std::vector<Vector3d> &res)
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
		{
			break;
		}

		res.push_back(Vector3d(buf[0], buf[1], buf[2]));
	}

	in.close();
	return;
}