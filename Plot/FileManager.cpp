#include "FileManager.h"
#include <fstream>
#include <iostream>
#include "Vector3.h"
using namespace std;

bool FileManager::readJacobian(std::ifstream &stream, Jacobian &res)
{
	complex<double> eigen[3];
	for(int i=0; i<3; i++)
	{
		double re, im;
		stream >> re >> im;
		eigen[i] = complex<double>(re,im);
		if( stream.eof() )
			return false;
	}

	Vector3<complex<double>> vec[3];
	for(int i=0; i<3; i++)
	{
		double re, im;
		stream >> re >> im;
		vec[i].x = complex<double>(re,im);
		if( stream.eof() )
			return false;
	}
	
	for(int i=0; i<3; i++)
	{
		double re, im;
		stream >> re >> im;
		vec[i].y = complex<double>(re,im);
		if( stream.eof() )
			return false;
	}
	
	for(int i=0; i<3; i++)
	{
		double re, im;
		stream >> re >> im;
		vec[i].z = complex<double>(re,im);
		if( stream.eof() )
			return false;
	}

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