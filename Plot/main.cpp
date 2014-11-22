
#include<iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <gl/glut.h>
#include "FileManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector3.h"
#include "define.h"
#include "Camera.h"
#include "GraphicManager.h"
#include "SigmaPlane.h"
#include <Windows.h>

//#define round_point 12
#define PI  3.14159265358979323846

vector<Vector3d> datas;
double s, t, u;
double jacob[3][3];
int cp_count;
Vector3d cp[CP_NUM];

double e_val[CP_NUM][3];
Vector3d e_vec[CP_NUM][3];
Vector3d round_cp[CP_NUM][round_num];

double runge_kutta(double x, double y, double z, int i, double step);

Vector3i size(91, 171, 86);

vector<SigmaPlane> sigmaPlanes;

int Index(int i, int j, int k)
{
	return k*sizeX*sizeY + j*sizeX + i;
}

bool samesign_check(double a, double b){
	if(a*b >= 0){
		return true;
	}else{
		return false;
	}
}

Vector3d position(0,0,20);
void normalize()
{
	for(int i=0; i<Size; i++)
		datas[i].normalize();
}

void readText(){
	int index;

	datas.resize(sizeX*sizeY*sizeZ);


	FILE *fp = fopen("bfield_near_cusp.txt", "r");

	//以下要書き換え?
	
	for(int k=0;k<sizeZ;k++){
		for(int j=0;j<sizeY;j++){
			for(int i=0;i<sizeX;i++){
				index = Index(i, j, k);
				fscanf(fp,"%lf %lf %lf", &datas[index].x, &datas[index].y, &datas[index].z);
			}
		}
	}

	fclose(fp);
	return;
}

void read_cp(){
	FILE *fp = fopen("cp_sub.txt", "r");

	for (int i = 0; i < 4;/*CP_NUM;*/ i++){
		fscanf(fp, "%lf %lf %lf", &cp[i].x, &cp[i].y, &cp[i].z);
	}

	printf("Read CP success!");

	fclose(fp);
	return;
}

void read_eigen(){
	FILE *fp = fopen("p_eigen_sub.txt", "r");

	for (int i = 0; i<4; i++){
		fscanf(fp, "%lf %lf %lf", &e_val[i][0], &e_val[i][1], &e_val[i][2]);

		fscanf(fp, "%lf %lf %lf", &e_vec[i][0].x, &e_vec[i][1].x, &e_vec[i][2].x);
		fscanf(fp, "%lf %lf %lf", &e_vec[i][0].y, &e_vec[i][1].y, &e_vec[i][2].y);
		fscanf(fp, "%lf %lf %lf", &e_vec[i][0].z, &e_vec[i][1].z, &e_vec[i][2].z);
	}

	fclose(fp);
	return;
}
void read_round(int i){
	char filepath[256];
	sprintf(filepath, "round_point%d.txt", i);
	FILE *fp = fopen(filepath, "r");
	
	for(int s=0; s<round_num; s++){
		fscanf(fp, "%lf %lf %lf", &round_cp[i][s].x, &round_cp[i][s].y, &round_cp[i][s].z);
	}

	fclose(fp);

	return;
}

void inTotest(int i, int j, int k, double test_x[8], double test_y[8], double test_z[8]){
	int index;

	index = Index(i-1, j-1, k-1);
	test_x[0] = datas[index].x;	test_y[0] = datas[index].y;	test_z[0] = datas[index].z;

	index = Index(i, j-1, k-1);
	test_x[1] = datas[index].x;	test_y[1] = datas[index].y;	test_z[1] = datas[index].z;

	index = Index(i, j, k-1);
	test_x[2] = datas[index].x;	test_y[2] = datas[index].y;	test_z[2] = datas[index].z;
	
	index = Index(i-1, j, k-1);
	test_x[3] = datas[index].x;	test_y[3] = datas[index].y;	test_z[3] = datas[index].z;
	
	index = Index(i-1, j-1, k);
	test_x[4] = datas[index].x;	test_y[4] = datas[index].y;	test_z[4] = datas[index].z;
	
	index = Index(i, j-1, k);
	test_x[5] = datas[index].x;	test_y[5] = datas[index].y;	test_z[5] = datas[index].z;
	
	index = Index(i, j, k);
	test_x[6] = datas[index].x;	test_y[6] = datas[index].y;	test_z[6] = datas[index].z;

	index = Index(i-1, j, k);
	test_x[7] = datas[index].x;	test_y[7] = datas[index].y;	test_z[7] = datas[index].z;

	return;
}

//----------------------------こっから応急処置ゾーン------------------------------
//double is, it, iu;

double interpolate(double is,double it,double iu, double test[8])
{
  return iu*(1-is)*(1-it)*test[4]+iu*it*(1-is)*test[7]+iu*is*(1-it)*test[5]+iu*is*it*test[6]
	+(1-iu)*(1-is)*(1-it)*test[0]+(1-iu)*(1-is)*it*test[3]+(1-iu)*is*(1-it)*test[1]
	+(1-iu)*is*it*test[2];
}

//単純に引数として座標(小数点含め)を与える形にしたほうが反復処理のとき楽
//与えられた座標からタイムステップ後の座標を返す関数
double streamline(double x, double y, double z, int i){

	double test_x[8], test_y[8], test_z[8];
	int x_i, y_i, z_i;
	double x_d, y_d, z_d;

	//printf("%f %f %f\n", x, y, z);
	
	/*
	float x_i = floor(x);
	float x_d = x - x_i;
	*/

	//xyz座標からijkの値とstuの値を出す。
	
	x_i = floor(x);
	x_d = x - x_i;
	y_i = floor(y);
	y_d = y - y_i;
	z_i = floor(z);
	z_d = z - z_i;

	/*
	x_d = modf(x, &x_i);
	y_d = modf(y, &y_i);
	z_d = modf(z, &z_i);
	*/
	//それぞれに対応する線形補完値を返

	//printf("aaaaaaaaaaaaaaaaa\n");
	//printf("%d %d %d, %f %f %f\n", x_i, y_i, z_i, x_d, y_d, z_d);
	//if(x_i+1 > sizeX || y_i+1 > sizeY || z_i+1 > sizeZ) return 0;
	//for(int i=0; i < 1; i++){
	//inTotestでは-1-1-1までいれるので、+1+1+1しておく
	//printf("into %d %d %d\n", x_i+1, y_i+1, z_i+1);
	inTotest(x_i+1, y_i+1, z_i+1, test_x, test_y, test_z);
	switch(i){
	case 1:
		return interpolate(x_d, y_d, z_d, test_x);
	case 2:
		return interpolate(x_d, y_d, z_d, test_y);
	default:
		return interpolate(x_d, y_d, z_d, test_z);
	}
}

double runge_kutta(double x, double y, double z, int i, double step){
	double k0, k1, k2, k3;
	double del;

	//printf("runge\n");
	del = streamline(x, y, z, i);
	k0 = step*del;
	//printf("k0:%f\n", k0);
	del = streamline((x+k0/2.0), (y+k0/2.0), (z+k0/2.0), i);
	k1 = step*del;
	//printf("k1:%f\n", k1);
	del = streamline((x+k1/2.0), (y+k1/2.0), (z+k1/2.0), i);
	k2 = step*del;
	//printf("k2:%f\n", k2);
	del = streamline((x+k2), (y+k2), (z+k2), i);
	k3 = step*del;
	//printf("k3:%f\n", k3);
	return ((k0 + 2*k1 + 2*k2 + k3)/6.0);

}

//-------------------------------ここまで応急処置ゾーン---------------------------

void display()
{
	Camera::getCamera()->SetViewportAndMatrix();
  Vector3d plot;	
  Vector3d cp_disp;
  Vector3d ya;
  double ya_size;

  glClear(GL_COLOR_BUFFER_BIT);

  //図形の描画 
  glColor3d(0.0, 0.0, 0.0);

  GraphicManager::GetGraphic()->DrawGrid();
  glPushMatrix();

  //これから描画する空間をglTranslateの分だけ移動する.
  glTranslatef(-sizeX / 2, -sizeY / 2, -sizeZ / 2);

  //Σ面の描画
  for (auto it = sigmaPlanes.begin(); it != sigmaPlanes.end(); it++)
  {
	  (*it).Draw();
  }

  /*
	//十字特異点描画
  
	glColor3d(1.0, 0.0, 0.0);
	for(int i=0; i<CP_NUM; i++){
		printf("i = %d\n", i);
		if(cp[i].x == 0 && cp[i].y == 0 && cp[i].z == 0) break;

		//printf("%f %f %f\n", cp[i].x, cp[i].y, cp[i].z);

		plot.x = cp[i].x - (double)sizeX/2;	plot.y = cp[i].y - (double)sizeY/2;	plot.z = cp[i].z - (double)sizeZ/2;

		glBegin(GL_LINES);
		glVertex3f(plot.x-1, plot.y, plot.z);
		glVertex3f(plot.x+1, plot.y, plot.z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(plot.x, plot.y-1, plot.z);
		glVertex3f(plot.x, plot.y+1, plot.z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(plot.x, plot.y, plot.z-1);
		glVertex3f(plot.x, plot.y, plot.z+1);
		glEnd();
	}
	*/

	//以下流線描画
	
	double step;// = -0.1;
	double dx1, dy1, dz1;
	double dx2, dy2, dz2;
	double d_size;

	const double first_len = 0.5;
	const double len = 0.1;

	//double tmp;

	//streamline

	glColor3d(0.0, 1.0, 0.0);
	/*
	for(int t=0; t<4; t++){
		if(t == 2){
			step = 0.1;
		}else{
			step = -0.1;
		}
		for(int s = 0; s<round_num-1; s++){
			dx1 = cp[t].x + round_cp[t][s].x*(first_len);	
			dy1 = cp[t].y + round_cp[t][s].y*(first_len);
			dz1 = cp[t].z + round_cp[t][s].z*(first_len);

			while(true){
				if(dx1 > sizeX-5 || dx1 < 5 || dy1 > sizeY-5 || dy1 < 5 || dz1 > sizeZ-5 || dz1 < 5 )
					break;

				dx2 = runge_kutta(dx1, dy1, dz1, 1, step);
				dy2 = runge_kutta(dx1, dy1, dz1, 2, step);
				dz2 = runge_kutta(dx1, dy1, dz1, 3, step);

				d_size = sqrt(dx2*dx2 + dy2*dy2 + dz2*dz2);
				
				dx2 = (dx2/d_size)*len;
				dy2 = (dy2/d_size)*len;
				dz2 = (dz2/d_size)*len;

				glBegin(GL_LINES);
				glVertex3f(dx1-(double)sizeX/2.0, dy1-(double)sizeY/2.0, dz1-(double)sizeZ/2.0);
				glVertex3f((dx1-(double)sizeX/2.0)+dx2, (dy1-(double)sizeY/2.0)+dy2, (dz1-(double)sizeZ/2.0)+dz2);
				glEnd();

				dx1 += dx2;	
				dy1 += dy2;
				dz1 += dz2;
			}
		}
	}
	*/
	/*
	for(int i=0; i<4; i++){
		for(int j=0; j<3; j++){
			if(i == 2){
				if(e_val[i][j] < 0){
					step = 0.1;
				}
			}else{
				if(e_val[i][j] > 0){
					step = -0.1;
				}
		}
	}
	}*/
  glPopMatrix();
	glFlush();
}
void Keyboard(unsigned char key, int x, int y)
{
	CameraManager::Keyboard(key, x, y);
}

void MouseMove(int x, int y)
{
	CameraManager::MouseMove(x, y);
}

void MousePress(int button, int state, int x, int y)
{
	CameraManager::MousePress(button, state, x, y);
}

void resize(int w, int h)
{
	Camera::getCamera()->SetWindowSize(w,h);
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void toVector(float *data)
{
	for(int i=0; i<Size; i++)
	{
		datas[i].x = data[i];
	}
	for(int i=0; i<Size; i++)
	{
		datas[i].y = data[i+Size];
	}
	for(int i=0; i<Size; i++)
	{
		datas[i].z = data[i+2*Size];
	}
}

void toVector2(float *data)
{
	for(int i=0; i<sizeX; i++)
	{
		for(int j=0; j<sizeY; j++)
		{
			for(int k=0; k<sizeZ; k++)
			{
				int index = Index(i,j,k);
				datas[index].x = data[3*index];
				datas[index].y = data[3*index+1];
				datas[index].z = data[3*index+2];
			}
		}
	}
}

void idle()
{
	Sleep(10);	//CPUの負荷を減らすために10msec待つ
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	//ヤコビアンのデータを読み込み
	vector<Jacobian> jacobians;	//ヤコビアンを格納する動的配列
	FileManager::ReadJacobianData("p_eigen_out.txt", jacobians);

	//クリティカルポイントのデータを読み込み
	vector<Vector3d> cpoints;	//クリティカルポイントを格納する動的配列
	FileManager::ReadCritialPoint("cp.txt", cpoints);

	// Σ面を配列に保存
	// ヤコビアンとクリティカルポイントの数が同じでなければならない
	for (int i = 0; i < cpoints.size(); i++){
		SigmaPlane s(jacobians[i], cpoints[i]);

		if (s.HasPlane())
			sigmaPlanes.push_back(s);
	}
	readText();
	read_cp();

	read_eigen();

	system("pause");	
	
	for(int i=0; i < 4; i++){
		read_round(i);
	}

	//streamline 描画
	GraphicManager::Initialize(600, 800, argc, argv);
	GraphicManager::GetGraphic()->MakeGrid(Vector3d(), size, Vector3i(2,2,2)); 

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(Keyboard);

	glutMouseFunc(MousePress);
	glutMotionFunc(MouseMove);
	init();
	glutMainLoop();
	

	return 0;
}
