﻿
#include<iostream>
#include <cstdio>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#include <gl/glut.h>
#include "FileManager.h"
#include "Vector3.h"
#include "define.h"
#include "Camera.h"
#include "GraphicManager.h"
#include "SigmaPlane.h"
#include <Windows.h>
#include <iomanip>

#define PI  3.14159265358979323846

vector<SigmaPlane*> sigmaPlanes;

Vector3i size(91, 171, 86);

Field *field;

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

	glPushMatrix();

	//グリッドの描画
	GraphicManager::GetGraphic()->DrawGrid();

	// 流線とΣ面の描画
	for (auto it = sigmaPlanes.begin(); it != sigmaPlanes.end(); it++){
		(*it)->Draw();
	}

	glPopMatrix();
	glFlush();
}


//存在するすべてのグラフィックを設定する
void SetAllGraphicVisible( bool visible )
{
	for ( auto it = sigmaPlanes.begin(); it != sigmaPlanes.end(); it++ )
	{
		(*it)->SetVisible(visible);
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	CameraManager::Keyboard(key, x, y);

	switch( key )
	{
	case ' ' : 
		SetAllGraphicVisible(true);
		break;
	}
}

void MouseMove(int x, int y)
{
	CameraManager::MouseMove(x, y);
}

void MousePress(int button, int state, int x, int y)
{
	CameraManager::MousePress(button, state, x, y);


	if( state == GLUT_DOWN ){
		Vector3d dir = Camera::getCamera()->ScreenToWorldVector(x, y);
		double minLength = -1;
		int hitted = -1;
		for( int i=0; i< sigmaPlanes.size(); i++)
		{
			SigmaPlane *s = sigmaPlanes[i];
			double length;
			if ( s->IsHit( Camera::getCamera()->GetPosition(), dir, length) &&
				( minLength < 0 || length < minLength ))
			{
				minLength = length;
				hitted = i;
			}
		}

		if( hitted >= 0 )
			sigmaPlanes[hitted]->SetVisible(false);
	}
}

void resize(int w, int h)
{
	Camera::getCamera()->SetWindowSize(w,h);
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void idle()
{
	Sleep(10);	//CPUの負荷を減らすために10msec待つ
	glutPostRedisplay();
}

ostream& OK(ostream &s)
{
	s << setw(10) << right << "OK";
	return s;
}

int main(int argc, char *argv[])
{
	int charWidth = 50; //文字幅1
	//ヤコビアンのデータを読み込み
	cout << std::setw(charWidth) << left << "ヤコビアンのデータを読み込み";
	vector<Jacobian> jacobians;	//ヤコビアンを格納する動的配列
	FileManager::ReadJacobianData("p_eigen_out.txt", jacobians);
	cout << OK << endl;

	//クリティカルポイントのデータを読み込み
	cout << setw(charWidth) << left << "クリティカルポイントの読み込み";
	vector<Vector3d> cpoints;	//クリティカルポイントを格納する動的配列
	FileManager::ReadCritialPointData("cp.txt", cpoints);
	cout << OK << endl;

	// Σ面を配列に保存
	// ヤコビアンとクリティカルポイントの数が同じでなければならない
	cout << setw(charWidth) << left <<  "ヤコビアンとクリティカルポイントからΣ面を生成" ;
	for (int i = 0; i < cpoints.size(); i++){
		auto *s = new SigmaPlane(jacobians[i], cpoints[i]);

		if ( s->HasPlane() )
			sigmaPlanes.push_back(s);
	}
	cout << OK << endl;

	//フィールドの生成
	field = GraphicManager::GetGraphic()->MakeField(size);

	//生成したフィールドにデータを読み込む
	cout << setw(charWidth) << left << "磁場のベクトルデータの読み込み";
	FileManager::ReadFieldData("bfield_near_cusp.txt", *field);
	cout << OK << endl;

	cout << setw(charWidth) << left << "流線の計算";
	for(int i=0; i<sigmaPlanes.size(); i++)
	{
		StreamLine *streamLine = new StreamLine();
		field->CalcStreamLine( *sigmaPlanes[i], *streamLine);
		sigmaPlanes[i]->AddStreamLine(streamLine);
	}
	cout << OK << endl;

	Camera::getCamera()->SetLook(size/2.0);


	//OpenGL関連の初期化
	GraphicManager::Initialize(600, 800, argc, argv);

	//グリッドの生成
	GraphicManager::GetGraphic()->MakeGrid(size/2.0, size, Vector3i(2,2,2)); 

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
