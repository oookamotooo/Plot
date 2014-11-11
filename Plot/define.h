#include <iostream>
#include <vector>
#include "Vector3.h"

#define CP_NUM 12

using namespace std;

const int sizeX = 91;
const int sizeY = 171;
const int sizeZ = 86;
const int Size = sizeX * sizeY * sizeZ;
//const double alpha = 30.0;
const int round_num = 13; //(int)(360.0/alpha);

extern vector<Vector3d> datas;
extern double s, t, u;
extern double jacob[3][3];
extern int cp_count;
extern Vector3d cp[CP_NUM];

extern double e_val[CP_NUM][3];
extern Vector3d e_vec[CP_NUM][3];

extern Vector3d round_cp[CP_NUM][round_num];