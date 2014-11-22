#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_
#include <complex>
#include <string>
#include "Jacobian.h"
#include "Vector3.h"
#include <vector>
class FileManager
{
public:
	/**
	ヤコビアンのデータを "fileName" から読み込む.
	データフォーマットは

	固有値1 固有値2 固有値3
	固有ベクトル1.x 固有ベクトル2.x 固有ベクトル3.x
	固有ベクトル1.y 固有ベクトル2.y 固有ベクトル3.y
	固有ベクトル1.z 固有ベクトル2.z 固有ベクトル3.z

	の繰り返し, 固有値,固有ベクトルの要素は複素数型で 実部 虚部の順にすべて空白で区切られている.
	なので,具体的には一行目は

	固有値1実部 固有値1虚部 固有値2実部 固有値2虚部 固有値3実部 固有値3虚部
	
	となっている(固有ベクトルも同様)
	Rで書き出したデータなら, JacobianDataFormatter.rb で上記のフォーマットに変換できる
	読み込み結果はresに格納される
	*/ 
	static void ReadJacobianData(const std::string fileName, std::vector<Jacobian> &res);

	static void ReadCritialPoint(const std::string fileName, std::vector<Vector3d> &res);
private:
	static bool readJacobian(std::ifstream &stream, Jacobian &res);
};

#endif