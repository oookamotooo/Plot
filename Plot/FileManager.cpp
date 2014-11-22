#include "FileManager.h"
#include <fstream>
#include <iostream>
#include "Vector3.h"
using namespace std;

namespace 
{
	bool ReadLine(ifstream &stream, complex<double> res[3])
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
}
	
bool FileManager::readJacobian(std::ifstream &stream, Jacobian &res)
{
	complex<double> eigen[3];

	//固有値を読み込む
	if ( !ReadLine(stream, eigen) )
		return false;

	Vector3<complex<double>> vec[3];
	complex<double> buf[3];

	//1行読み込んで,それぞれのx成分に代入
	if( !ReadLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].x = buf[i];

	//1行読み込んで,それぞれのy成分に代入
	if( !ReadLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].y = buf[i];

	//1行読み込んで,それぞれのz成分に代入
	if( !ReadLine(stream, buf) )
		return false;
	for(int i=0; i<3; i++)
		vec[i].z = buf[i];

	res = Jacobian(eigen, vec);
	return true;
}

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
}

void FileManager::ReadCritialPoint(const string fileName, vector<Vector3d> &res)
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
		Vector3d cp;
		in >> cp.x >> cp.y;
		if (in.eof())
			break;

		in >> cp.z;
		res.push_back(cp);
	}

	return;
}