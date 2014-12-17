
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
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Dense>
#include <map>
using namespace std;

#define PI  3.14159265358979323846

Vector3i size(91, 171, 86);

Field *field;
vector<StreamLine*> earthLines;
vector<SigmaPlane*> sigmaPlanes;
vector<Eigen::Vector3i> red, green, blue;

void display()
{
	Camera::getCamera()->SetViewportAndMatrix();

	glClear(GL_COLOR_BUFFER_BIT);

	//図形の描画 
	glColor3d(0.0, 0.0, 0.0);

	glPushMatrix();

	//グリッドの描画
	GraphicManager::GetGraphic()->Draw();
	
	// 流線とΣ面の描画
	for (auto it = sigmaPlanes.begin(); it != sigmaPlanes.end(); it++){
		(*it)->Draw();
		Vector3i grid = (*it)->CPoint().cast<int>();
	}

	for (auto it = earthLines.begin(); it != earthLines.end(); it++){
		(*it)->Draw();
	}
	
	for (auto it = blue.begin(); it != blue.end(); it++){
		field->DrawGrid((*it), Vector3i(1, 1, 1), Vector3f(0, 0, 1));
	}
	for (auto it = green.begin(); it != green.end(); it++){
		field->DrawGrid((*it), Vector3i(1, 1, 1), Vector3f(0, 1, 0));
	}	
	for (auto it = red.begin(); it != red.end(); it++){
		field->DrawGrid((*it), Vector3i(1, 1, 1), Vector3f(1, 0, 0));
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

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)//矢印「上」
	{
		field->PlaneNext();
	}

	if (key == GLUT_KEY_DOWN)//矢印「下」
	{
		field->PlaneBack();
	}

}


int hitted = -1;	//マウスがドラッグしているΣ面の番号
Eigen::Vector2i lastPos = Eigen::Vector2i::Zero();	//マウスの位置
map<unsigned char, unsigned int> keyPressMap;	//押されているキー

//キーが押された時の処理
void KeyboardPress(unsigned char key, int x, int y)
{
	CameraManager::Keyboard(key, x, y);

	keyPressMap[key] = true;
	switch( key )
	{
	case ' ' : 
		SetAllGraphicVisible(true);
		break;
	case 'x' :
		field->ChangePlane(Field::PlaneKinds::XPlane);
		break;
	case 'y':
		field->ChangePlane(Field::PlaneKinds::YPlane);
		break;
	case 'z':
		field->ChangePlane(Field::PlaneKinds::ZPlane);
		break;
	case 'v':
		field->SetVisible(false);
		break;
	case 'd':
		if (hitted >= 0){
			sigmaPlanes[hitted]->SetVisible(false);
			hitted = -1;
		}
		break;
	}
}

//キーが離された時の処理
void KeyboardUp(unsigned char key, int x, int y)
{
	keyPressMap[key] = false;
}

//マウスが動いた時の処理
void MouseMove(int x, int y)
{
	if (!keyPressMap['m'])
		CameraManager::MouseMove(x, y);

	if (hitted < 0 || !keyPressMap['m']){
		hitted = -1;
		return;
	}

	//クリティカルポイントの位置を移動
	int dx = x - lastPos.x();
	int dy = y - lastPos.y();	
	auto dir = Camera::getCamera()->ScreenToWorldVector(x, y);
	auto cp = sigmaPlanes[hitted]->CPoint();
	auto camPos = Camera::getCamera()->GetPosition();
	auto dot = (cp-camPos).dot(dir);
	double t = dot / dir.squaredNorm();
	Vector3d newCp = camPos + dir*t;
	Eigen::Matrix3d mat;
	field->GetJacobiMatrix(newCp, mat);
	Jacobian ja(mat);
	sigmaPlanes[hitted]->DebugMove(ja, newCp);
}

void MousePress(int button, int state, int x, int y)
{
	CameraManager::MousePress(button, state, x, y);

	if (state == GLUT_UP && hitted >= 0)
	{
		sigmaPlanes[hitted]->DeleteChild(0);
		StreamLine *s = new StreamLine();
		field->CalcStreamLine(*sigmaPlanes[hitted], *s);
		sigmaPlanes[hitted]->AddChild(s);
		hitted = -1;
	}

	if( state == GLUT_DOWN ){
		Vector3d dir = Camera::getCamera()->ScreenToWorldVector(x, y);
		double minLength = -1;
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

		lastPos << x, y;

		if (hitted < 0)
			return;

		if (keyPressMap['p']){
			sigmaPlanes[hitted]->SetVisible(false);
			hitted = -1;
			return;
		}
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

ostream& Forming(ostream &s)
{
	s << setw(50) << left;
	return s;
}

ostream& OK(ostream &s)
{
	s << setw(4) << right << "OK";
	return s;
}

void TestEigen()
{
	Eigen::Matrix2d a;
	a << 1, 2, -2, 1;
	Eigen::Matrix2cd b = a.cast<complex<double>>();
	cout << b << endl;
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix2cd> es(b);
	if (es.info() != Eigen::Success)
		cout << "Failed" << endl;
	else
	{
		cout << "Success " << endl;
		cout << es.eigenvalues()[0] << endl;
	}

	/*
	Eigen::Vector3cd a(complex<double>(1, 1), complex<double>(3, 3), complex<double>(2, 2));

	Eigen::Matrix3d X = Eigen::Matrix3d::Zero();

	X << -3, -1, -1, -1, 1, -1, -1, -1, 3;

	cout << X << endl;

	Eigen::Matrix2f A;
	A << 1, 2, 2, 3;
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix2f> solv(A);

	cout << solv.eigenvalues() << endl;
	cout << solv.eigenvectors() << endl;
	Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> es(X);

	if (es.info() != Eigen::Success)
		cout << "Failed" << endl;
	cout << "Value" << endl;
	cout << es.eigenvalues() << endl;
	cout << "Vectior" << endl;
	cout << es.eigenvectors() << endl;*/
}

int SearchSigmaRound(SigmaPlane *sigmaPlane, Vector3d &move)
{
	std::vector<Vector3d> roundPoints;
	sigmaPlane->GetRoundPoints(roundPoints);
	Vector3d cp = sigmaPlane->CPoint();
	Vector3d n = sigmaPlane->Normal().normalized();

	bool reverse = sigmaPlane->Jacob().eigenValue[sigmaPlane->Index1()].real() < 0;
	const double step = 0.02;
	move = Vector3d::Zero();
	int cnt = 0;
	for (auto it = roundPoints.begin(); it != roundPoints.end(); it++){
		auto dir = (*it) - cp;				//中心から周りの点への方向ベクトル
		auto vec = field->GetData((*it)).normalized();	//周りの点上のベクトル

		//Σ面へ写像
		auto proj_vec = vec - vec.dot(n)*n;

		const double error = 0.1;
		//写像後のベクトルが非常に小さい => Σ面にほぼ垂直になっている
		//とりあえずはOKとする.
		if (proj_vec.norm() < error){
			cnt++;
			move -= vec;
			continue;
		}

		//写像後のベクトルが放射状にのびていない
		//すこしずらす
		double dot = proj_vec.dot(dir);
		if( (reverse && dot > 0) || (!reverse && dot < 0)){
			move -= vec*step;
			cout << dir.transpose() << "   " << proj_vec.transpose() << endl;
			cnt++;
		}
	}
	return cnt;
}

void Adjastment()
{
	SigmaPlane *s = sigmaPlanes[0];

	//最大30回調整を行う.
	for (;true;){
		Vector3d move = Vector3d::Zero();
		int cnt = SearchSigmaRound(s, move);
		if ( cnt < 2)
			break;
		cout << "Count " << cnt << endl;
		Vector3d newCp = s->CPoint() + move;

		Eigen::Matrix3d jacob;
		field->GetJacobiMatrix(newCp, jacob);

		Jacobian j(jacob);

		s->DebugMove(j, newCp);
		cout << newCp.transpose() << endl;
		//cout << field->GetData(newCp).norm() << endl;
	}
}
void ReadSigmaData()
{
	cout << Forming << "Σ面のデータを読み込み";
	FileManager::ReadSigmaPlaneData("SigmaPlanes.txt", sigmaPlanes);
	cout << OK << endl;
}

void CalcSigmaData()
{
	cout << Forming << "クリティカルポイントの探索";
	vector<Vector3d> criticalPoints;
	field->SearchCP(criticalPoints);
	cout << OK << endl;

	for (auto it = criticalPoints.begin(); it != criticalPoints.end(); it++){
		cout << (*it).x() << "," << (*it).y() << "," << (*it).z() << endl;
		cout << field->GetData(*it) << endl;

		Eigen::Matrix3d jacob;
		field->GetJacobiMatrix((*it), jacob);
		Eigen::SelfAdjointEigenSolver<Eigen::Matrix3cd> es(jacob.cast<complex<double>>());

		complex<double> eigen[3];
		Eigen::Vector3cd vec[3];
		for (int i = 0; i < 3; i++){
			eigen[i] = es.eigenvalues()[i];
			vec[i] = es.eigenvectors().row(i);
		}

		Jacobian ja(eigen, vec);
		auto *s = new SigmaPlane(ja, (*it));

		if (s->HasPlane()){
			sigmaPlanes.push_back(s);
			cout << "Sigma" << endl;
		}
	}
	cout << Forming << "Σ面のデータを保存中";
	FileManager::WriteSigmaPlaneData("SigmaPlanes.txt", sigmaPlanes);
	cout << OK << endl;;
}

int main(int argc, char *argv[])
{
	TestEigen(); //Eigenライブラリのテスト用

	//フィールドの生成
	field = GraphicManager::GetGraphic()->MakeField(Vector3i(), size);

	//生成したフィールドにデータを読み込む
	cout << Forming << "磁場のベクトルデータの読み込み";
	FileManager::ReadFieldData("bfield_near_cusp.txt", field);
	cout << OK << endl;

	//毎回計算すると遅いので
	//すでに書き出したテキストデータからΣ面のデータを読み込む
	ReadSigmaData();
	//計算してΣ面を導出する
	//CalcSigmaData();

	//Adjastment();
	/*
	Eigen::Matrix3d jacob;
	field->GetJacobiMatrix(Vector3d(57.052, 75.082, 54.968), jacob);
	cout << jacob << endl;
	*/
	for (int i = 0; i < field->Size.x()-1; i++){
		for (int j = 0; j < field->Size.y()-1; j++){
			for (int k = 0; k < field->Size.z()-1; k++){
				int cnt = 0;

				for (int n = 0; n < 4; n++){
					int dx = n == 1, dy = n == 2, dz = n == 3;
					Vector3d p1 = field->Data(i + dx, j + dy, k + dz).normalized();
					Vector3d p2 = field->Data(i + 1 - dx, j + 1 - dy, k + 1 - dz).normalized();
					bool check = p1.x()*p2.x() < 0 && p1.y()*p2.y()<0 && p1.z()*p2.z()<0;
					if ( check )
						cnt++;
				}

				if (cnt == 2)
					blue.push_back(Vector3i(i, j, k));
				if (cnt == 3)
					green.push_back(Vector3i(i, j, k));
				if (cnt == 4)
					red.push_back(Vector3i(i, j, k));
			}
		}
	}
	//古いタイプの読み込み方法
	/*
	//ヤコビアンのデータを読み込み
	cout << Forming << "ヤコビアンのデータを読み込み";
	vector<MR::Jacobian> jacobians;	//ヤコビアンを格納する動的配列
	FileManager::ReadJacobianData("p_eigen_out.txt", jacobians);
	cout << OK << endl;

	//クリティカルポイントのデータを読み込み
	cout << Forming << "クリティカルポイントの読み込み.";
	vector<Vector3d, Eigen::aligned_allocator<Vector3d>> cpoints;	//クリティカルポイントを格納する動的配列
	FileManager::ReadCritialPointData("cp.txt", cpoints);
	cout << OK << endl;

	// Σ面を配列に保存
	// ヤコビアンとクリティカルポイントの数が同じでなければならない
	cout << Forming << "ヤコビアンとクリティカルポイントからΣ面を生成";
	for (int i = 0; i < cpoints.size(); i++){
	auto *s = new SigmaPlane(jacobians[i], cpoints[i]);
	if ( s->HasPlane() )
	sigmaPlanes.push_back(s);
	}
	cout << OK << endl;
	*/

	cout << Forming << "流線の計算";
	for(int i=0; i<sigmaPlanes.size(); i++)
	{
		StreamLine *streamLine = new StreamLine();
		field->CalcStreamLine( *sigmaPlanes[i], *streamLine);
		sigmaPlanes[i]->AddChild(streamLine);
	}
	cout << OK << endl;
	
	cout << Forming << "地球近辺の流線を計算";
	//フィールドを基準とした地球の位置
	//フィールドの外にあるので, ここからずらした位置から流線を求める必要がある
	Vector3d earth = Vector3d(60.0, 82.0, -13.0);

	//試行錯誤でできた,x成分のオフセット量
	int dx[4] = { 0, 3, -4, -6 };

	glColor3d(0.0, 1.0, 0.0);
	for (int l = 0; l < 4; l++){
		StreamLine *streamLine = new StreamLine();
		Vector3d start(earth.x() + dx[l], earth.y(), earth.z() + 20);
		field->CalcStreamLine(start, *streamLine, 0.1, -0.1);
		earthLines.push_back(streamLine);
	}
	cout << OK << endl;

	Camera::getCamera()->SetLook(size.cast<double>()/2.0);

	
	//OpenGL関連の初期化
	GraphicManager::Initialize(600, 800, argc, argv);

	//グリッドの生成
	GraphicManager::GetGraphic()->MakeGrid(size.cast<float>()/2.0, size.cast<float>(), Vector3i(2,2,2)); 

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(KeyboardPress);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(MousePress);
	glutMotionFunc(MouseMove);
	init();
	glutMainLoop();
	
	return 0;
}
