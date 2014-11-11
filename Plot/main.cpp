
#include<iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <gl/glut.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector3.h"
#include "define.h"

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

class Camera
{
public:
	Vector3d position, look, up;
	float theta, phi, radius;
	Camera()
	{
		theta = 0;
		phi = 0;
		position = Vector3d(0,0,1);
		look = Vector3d();
		up = Vector3d(0,1,0);
	}

	void RotateTheta(float deg)
	{

	}

	void RotatePhi(float deg)
	{

	}

};

Camera camera;

double runge_kutta(double x, double y, double z, int i, double step);

Vector3i size(91, 171, 86);

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

float phi = 0, theta = 0, fov = 90, radius = 20;
float px, py, pz;
void normalize()
{
	for(int i=0; i<Size; i++)
		datas[i].normalize();
}

void readText(){
	int index;

	datas.resize(sizeX*sizeY*sizeZ);


	FILE *fp = fopen("bfield_near_cusp.txt", "r");

	//�ȉ��v��������?
	
	for(int k=0;k<sizeZ;k++){
		for(int j=0;j<sizeY;j++){
			for(int i=0;i<sizeX;i++){
				index = Index(i, j, k);

				fscanf(fp,"%lf %lf %lf", &datas[index].x, &datas[index].y, &datas[index].z);
				/*
				fscanf(fp, "%lf",&datas[index].x);
				fscanf(fp, "%lf",&datas[index].y);
				fscanf(fp, "%lf",&datas[index].z);
				*/
			}
		}
	}
	/*
	for(int i=0;i<sizeX*sizeY*sizeZ;i++){
		fscanf(fp,"%lf %lf %lf", &datas[i].x, &datas[i].y, &datas[i].z);
	}
	*/

	//normalize();

	fclose(fp);
	return;
}

void read_cp(){
	FILE *fp = fopen("cp.txt", "r");

	for(int i=0; i < CP_NUM; i++){
		fscanf(fp,"%lf %lf %lf", &cp[i].x, &cp[i].y, &cp[i].z);
	}

	printf("Read CP success!");

	fclose(fp);
	return;
}

void read_eigen(){
	FILE *fp = fopen("p_eigen.txt", "r");

	for(int i=0; i<2; i++){
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

//----------------------------�������牞�}���u�]�[��------------------------------
//double is, it, iu;

double interpolate(double is,double it,double iu, double test[8])
{
  //printf("inter\n");
  return iu*(1-is)*(1-it)*test[4]+iu*it*(1-is)*test[7]+iu*is*(1-it)*test[5]+iu*is*it*test[6]
	+(1-iu)*(1-is)*(1-it)*test[0]+(1-iu)*(1-is)*it*test[3]+(1-iu)*is*(1-it)*test[1]
	+(1-iu)*is*it*test[2];
}

//�P���Ɉ����Ƃ��č��W(�����_�܂�)��^����`�ɂ����ق������������̂Ƃ��y
//�^����ꂽ���W����^�C���X�e�b�v��̍��W��Ԃ��֐�
double streamline(double x, double y, double z, int i){

	double test_x[8], test_y[8], test_z[8];
	int x_i, y_i, z_i;
	double x_d, y_d, z_d;

	//printf("%f %f %f\n", x, y, z);
	
	/*
	float x_i = floor(x);
	float x_d = x - x_i;
	*/

	//xyz���W����ijk�̒l��stu�̒l���o���B
	
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
	//���ꂼ��ɑΉ�������`�⊮�l���

	//printf("aaaaaaaaaaaaaaaaa\n");
	//printf("%d %d %d, %f %f %f\n", x_i, y_i, z_i, x_d, y_d, z_d);
	//if(x_i+1 > sizeX || y_i+1 > sizeY || z_i+1 > sizeZ) return 0;
	//for(int i=0; i < 1; i++){
	//inTotest�ł�-1-1-1�܂ł����̂ŁA+1+1+1���Ă���
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

//-------------------------------�����܂ŉ��}���u�]�[��---------------------------

void display()
{
  Vector3d plot;	
  Vector3d cp_disp;
  Vector3d ya;
  double ya_size;

  glClear(GL_COLOR_BUFFER_BIT);

  //�}�`�̕`�� 
  glColor3d(0.0, 0.0, 0.0);

  cout << "diso" << endl;
  //int sx = 110, sy=55, sz=30;
  
  /*
  for(int k = z_start; k < z_start + sizeZ; k++)
	  for(int j = y_start; j < y_start + sizeY; j++)
		  for(int i = x_start; i < x_start + sizeX; i++){
			  //if(k%10 == 0){
				int index = Index(i, j, k);
				float x = i - (double)sizeX/2;
				float y = j - (double)sizeY/2;
				float z = k - (double)sizeZ/2;
				glBegin(GL_LINES);
				glVertex3f(x, y, z);
				glVertex3f(x+datas[index].x,y+datas[index].y, z+datas[index].z);
				glEnd();
			  //}
			}*/

  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,-(double)sizeY/2.0,-(double)sizeZ/2.0);
  glVertex3f(-(double)sizeX/2.0,-(double)sizeY/2.0,(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,(double)sizeY/2.0,-(double)sizeZ/2.0);
  glVertex3f(-(double)sizeX/2.0,(double)sizeY/2.0,(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f((double)sizeX/2.0,-(double)sizeY/2.0,-(double)sizeZ/2.0);
  glVertex3f((double)sizeX/2.0,-(double)sizeY/2.0,(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f((double)sizeX/2.0,(double)sizeY/2.0,-(double)sizeZ/2.0);
  glVertex3f((double)sizeX/2.0,(double)sizeY/2.0,(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,-(double)sizeY/2.0,(double)sizeZ/2.0);
  glVertex3f(-(double)sizeX/2.0,(double)sizeY/2.0,(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f((double)sizeX/2.0,-(double)sizeY/2.0,(double)sizeZ/2.0);
  glVertex3f((double)sizeX/2.0,(double)sizeY/2.0,(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,-(double)sizeY/2.0,-(double)sizeZ/2.0);
  glVertex3f(-(double)sizeX/2.0,(double)sizeY/2.0,-(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f((double)sizeX/2.0,-(double)sizeY/2.0,-(double)sizeZ/2.0);
  glVertex3f((double)sizeX/2.0,(double)sizeY/2.0,-(double)sizeZ/2.0);
  glEnd;

  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,(double)sizeY/2.0,(double)sizeZ/2.0);
  glVertex3f((double)sizeX/2.0,(double)sizeY/2.0,(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,-(double)sizeY/2.0,(double)sizeZ/2.0);
  glVertex3f((double)sizeX/2.0,-(double)sizeY/2.0,(double)sizeZ/2.0);
  glEnd;

  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,(double)sizeY/2.0,-(double)sizeZ/2.0);
  glVertex3f((double)sizeX/2.0,(double)sizeY/2.0,-(double)sizeZ/2.0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,-(double)sizeY/2.0,-(double)sizeZ/2.0);
  glVertex3f((double)sizeX/2.0,-(double)sizeY/2.0,-(double)sizeZ/2.0);
  glEnd;

  glBegin(GL_LINES);
  glVertex3f(-(double)sizeX/2.0,0,0);
  glVertex3f((double)sizeX/2.0,0,0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f(0,-(double)sizeY/2.0,0);
  glVertex3f(0,(double)sizeY/2.0,0);
  glEnd;
  glBegin(GL_LINES);
  glVertex3f(0,0,-(double)sizeZ/2.0);
  glVertex3f(0,0,(double)sizeZ/2.0);
  glEnd;


  for(int i=0; i < 2; i++){
	  cp_disp.x = cp[i].x - (double)sizeX/2;	cp_disp.y = cp[i].y - (double)sizeY/2;	cp_disp.z = cp[i].z - (double)sizeZ/2;
	  for(int j=0; j< 3; j++){
		  ya.x = e_vec[i][j].y*pz - e_vec[i][j].z*py;	ya.y = e_vec[i][j].z*px- e_vec[i][j].x*pz;	ya.z = e_vec[i][j].x*py - e_vec[i][j].y*px;
		  ya_size = sqrt(ya.x*ya.x + ya.y*ya.y + ya.z*ya.z);
		  
		  //�ŗL�x�N�g���`��
		  glColor3d(1.0, 0.0, 0.0);
		  glBegin(GL_LINE_STRIP);
		  glVertex3f(cp_disp.x, cp_disp.y, cp_disp.z);
		  glVertex3f(cp_disp.x + e_vec[i][j].x, cp_disp.y + e_vec[i][j].y, cp_disp.z + e_vec[i][j].z);
		  glVertex3f(cp_disp.x + (3.0/4.0)*e_vec[i][j].x + (ya.x/ya_size)*(1.0/4.0), cp_disp.y + (3.0/4.0)*e_vec[i][j].y + (ya.y/ya_size)*(1.0/4.0), cp_disp.z + (3.0/4.0)*e_vec[i][j].z + (ya.z/ya_size)*(1.0/4.0));
		  glVertex3f(cp_disp.x + e_vec[i][j].x, cp_disp.y + e_vec[i][j].y, cp_disp.z + e_vec[i][j].z);
		  glVertex3f(cp_disp.x + (3.0/4.0)*e_vec[i][j].x - (ya.x/ya_size)*(1.0/4.0), cp_disp.y + (3.0/4.0)*e_vec[i][j].y - (ya.y/ya_size)*(1.0/4.0), cp_disp.z + (3.0/4.0)*e_vec[i][j].z - (ya.z/ya_size)*(1.0/4.0));
		  glEnd();
	  }
	  
	  //���ʕ`��
		  glColor3d(0.0, 0.0, 1.0);
		  for(int k=0; k<round_num-1; k++){
			  glBegin(GL_LINE_LOOP);
			  glVertex3f(cp_disp.x, cp_disp.y, cp_disp.z);
			  glVertex3f(cp_disp.x + round_cp[i][k].x, cp_disp.y + round_cp[i][k].y, cp_disp.z + round_cp[i][k].z);
			  glVertex3f(cp_disp.x + round_cp[i][k+1].x, cp_disp.y + round_cp[i][k+1].y, cp_disp.z + round_cp[i][k+1].z);
			  glEnd();
		  }
		  
  }

  /*
	//�\�����ٓ_�`��
  
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

	//�ȉ������`��
	
	/*
	double step = 0.1;
	double dx1, dy1, dz1;
	double dx2, dy2, dz2;
	//double tmp;

	//printf("%f\n", cp[0].x);
	//printf("d-series = %f %f %f\n", dx1, dy1, dz1);
	//streamline

	for(int s = 0; s<5; s++){
		dx1=cp[s].x; dy1=cp[s].y; dz1=cp[s].z;
		glColor3d(0.0, 0.0, 1.0);
		//for(int i=0; i<4500; i++){
		while(true){
			if(dx1 > sizeX-5 || dx1 < 5 || dy1 > sizeY-5 || dy1 < 5 || dz1 > sizeZ-5 || dz1 < 5 ) break;
			//printf("%d %d %d\n", dx1, dy1, dz1);
			dx2 = runge_kutta(dx1, dy1, dz1, 1, step);
			dy2 = runge_kutta(dx1, dy1, dz1, 2, step);
			dz2 = runge_kutta(dx1, dy1, dz1, 3, step);

			glBegin(GL_LINES);
			glVertex3f(dx1-(double)sizeX/2.0, dy1-(double)sizeY/2.0, dz1-(double)sizeZ/2.0);
			glVertex3f((dx1-(double)sizeX/2.0)+dx2, (dy1-(double)sizeY/2.0)+dy2, (dz1-(double)sizeZ/2.0)+dz2);
			glEnd();

			//printf("d1 = %f, %f, %f\n", dx1, dy1, dz1);
			dx1 += dx2;	dy1 += dy2;	dz1 += dz2;
		}
	}
	*/

	glFlush();
}

int width, height;

void look()
{
	
  glViewport(0, 0, width, height);

  glLoadIdentity();
	  //glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);
  cout << "look" << endl;
  gluPerspective(fov, (double)width / (double)height, 1.0, 500.0);
  
  px = radius * cos(phi) * cos(theta);
  py = radius * sin(phi);
  pz = radius * cos(phi) * sin(theta);
  gluLookAt(px, py, pz,
	   0,  0, 0, 
	   0.0, 1.0, 0.0);

  glutPostRedisplay();
  
}

void resize(int w, int h)
{
	width = w;
	height = h;

  look();
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
	cout << "keyborad" << endl;
  switch (key) {
  case 'w':
    phi += 1.0 / 180.0 * M_PI;
	break;
  case 's':
	  phi -= 1.0 / 180.0 * M_PI;
	  break;
  case 'a':
	  theta += 1.0 / 180.0 * M_PI;
	  break;
  case 'd':
	  theta -= 1.0 /180.0 *  M_PI;
	  break;
  case 'q':
	  //fov+=1;
	  radius += 1.0;
	  break;
  case 'e':
	  radius = max(1.0, radius-1.0);
	  break;
  default:
    break;
  }
  look();
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

Vector3d rotate(Vector3d housen, Vector3d vec, double alpha){
	Vector3d result;
	double theta;
	
	theta = alpha * PI / 180.0;

	result.x = ((housen.x*housen.x*(1-cos(theta)) + cos(theta))*vec.x + (housen.x*housen.y*(1-cos(theta)) - housen.z*sin(theta))*vec.y + (housen.z*housen.x*(1-cos(theta)) + housen.y*sin(theta))*vec.z);
	result.y = ((housen.x*housen.y*(1-cos(theta)) + housen.z*sin(theta))*vec.x + (housen.y*housen.y*(1-cos(theta)) + cos(theta))*vec.y + (housen.y*housen.z*(1-cos(theta)) - housen.x*sin(theta))*vec.z);
	result.z = ((housen.z*housen.x*(1-cos(theta)) - housen.y*sin(theta))*vec.x + (housen.y*housen.z*(1-cos(theta)) + housen.x*sin(theta))*vec.y + (housen.z*housen.z*(1-cos(theta)) + cos(theta))*vec.z);

	return result; 
}

void make_round(double sub_val[CP_NUM][3], Vector3d sub_vec[CP_NUM][3], int n){
	int a, b;
	double h_size;
	double alpha = 30.0;
	Vector3d housen, rotated;

	FILE *fp;
	//char name = (char)n;
	char filepath[256];

	sprintf(filepath, "round_point%d.txt", n);
	fp = fopen(filepath, "w");
	
	/*
	FILE *fp = fopen("round_point.txt", "w");
	*/
	//�������x�N�g���T��

	//���͂��ׂĂ�++-or--+�Ȃ̂ł���ł������A�����I�ɂ��ׂē������p�^�[�������܂ꂽ�ꍇ�͏�������

	for(int i=0; i<3; i++){
		a = i;
		if(i == 2){
			b = 0;
		}else{
			b = i+1;
		}

		if(samesign_check(sub_val[n][a], sub_val[n][b])){
			printf("samesign value = (%lf, %lf)\n", sub_val[n][a], sub_val[n][b]);
			break;
		}
	}

	//�@���x�N�g�����߂�
	housen.x = (sub_vec[n][a].y*sub_vec[n][b].z - sub_vec[n][a].z*sub_vec[n][b].y);
	housen.y = (sub_vec[n][a].z*sub_vec[n][b].x - sub_vec[n][a].x*sub_vec[n][b].z);
	housen.z = (sub_vec[n][a].x*sub_vec[n][b].y - sub_vec[n][a].y*sub_vec[n][b].x);

	h_size = sqrt(housen.x*housen.x + housen.y*housen.y + housen.z*housen.z);

	housen.x = (housen.x/h_size);
	housen.y = (housen.y/h_size);
	housen.z = (housen.z/h_size);

	printf("unit vector make %lf %lf %lf -> %lf\n", housen.x, housen.y, housen.z, sqrt(housen.x*housen.x + housen.y*housen.y + housen.z*housen.z));
	//�@���x�N�g�������ɉ�]�s�񂩂���
	rotated.x = sub_vec[n][a].x;	rotated.y = sub_vec[n][a].y;	rotated.z = sub_vec[n][a].z;
	fprintf(fp, "%lf %lf %lf\n", rotated.x, rotated.y, rotated.z);

	for(int i=0;i < (int)(360.0/alpha); i++){
		rotated = rotate(housen, rotated, alpha);
		fprintf(fp, "%lf %lf %lf\n", rotated.x, rotated.y, rotated.z);
	}

	printf("aaa\n");

	fclose(fp);
	return;
}

int main(int argc, char *argv[])
{
	Vector3i s;
	Vector3i a = Vector3i()+s;

	cout << size << endl;
	//readText();

	read_cp();
	
	read_eigen();

	
	for(int i=0; i < 2; i++){
		read_round(i);
	}
	

	system("pause");
	/*
	for(int k=0; k<2; k++){
		make_round(e_val, e_vec, k);
	}

	system("pause");
	*/

	
	//streamline �`��
	
	glutInitWindowSize(600, 600);
	glutInit(&argc, argv);
	//glClearColor(0.0, 0.0, 1.0, 1.0);
	//3d
	glutInitDisplayMode(GLUT_RGBA);
	//3d
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	//3d
	glutReshapeFunc(resize);
	//3d
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();
	
	return 0;
}
