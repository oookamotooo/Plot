
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

	//�}�`�̕`�� 
	glColor3d(0.0, 0.0, 0.0);

	glPushMatrix();

	//�O���b�h�̕`��
	GraphicManager::GetGraphic()->DrawGrid();

	// �����ƃ��ʂ̕`��
	for (auto it = sigmaPlanes.begin(); it != sigmaPlanes.end(); it++){
		(*it)->Draw();
	}

	glPopMatrix();
	glFlush();
}


//���݂��邷�ׂẴO���t�B�b�N��ݒ肷��
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
	Sleep(10);	//CPU�̕��ׂ����炷���߂�10msec�҂�
	glutPostRedisplay();
}

ostream& OK(ostream &s)
{
	s << setw(10) << right << "OK";
	return s;
}

int main(int argc, char *argv[])
{
	int charWidth = 50; //������1
	//���R�r�A���̃f�[�^��ǂݍ���
	cout << std::setw(charWidth) << left << "���R�r�A���̃f�[�^��ǂݍ���";
	vector<Jacobian> jacobians;	//���R�r�A�����i�[���铮�I�z��
	FileManager::ReadJacobianData("p_eigen_out.txt", jacobians);
	cout << OK << endl;

	//�N���e�B�J���|�C���g�̃f�[�^��ǂݍ���
	cout << setw(charWidth) << left << "�N���e�B�J���|�C���g�̓ǂݍ���";
	vector<Vector3d> cpoints;	//�N���e�B�J���|�C���g���i�[���铮�I�z��
	FileManager::ReadCritialPointData("cp.txt", cpoints);
	cout << OK << endl;

	// ���ʂ�z��ɕۑ�
	// ���R�r�A���ƃN���e�B�J���|�C���g�̐��������łȂ���΂Ȃ�Ȃ�
	cout << setw(charWidth) << left <<  "���R�r�A���ƃN���e�B�J���|�C���g���烰�ʂ𐶐�" ;
	for (int i = 0; i < cpoints.size(); i++){
		auto *s = new SigmaPlane(jacobians[i], cpoints[i]);

		if ( s->HasPlane() )
			sigmaPlanes.push_back(s);
	}
	cout << OK << endl;

	//�t�B�[���h�̐���
	field = GraphicManager::GetGraphic()->MakeField(size);

	//���������t�B�[���h�Ƀf�[�^��ǂݍ���
	cout << setw(charWidth) << left << "����̃x�N�g���f�[�^�̓ǂݍ���";
	FileManager::ReadFieldData("bfield_near_cusp.txt", *field);
	cout << OK << endl;

	cout << setw(charWidth) << left << "�����̌v�Z";
	for(int i=0; i<sigmaPlanes.size(); i++)
	{
		StreamLine *streamLine = new StreamLine();
		field->CalcStreamLine( *sigmaPlanes[i], *streamLine);
		sigmaPlanes[i]->AddStreamLine(streamLine);
	}
	cout << OK << endl;

	Camera::getCamera()->SetLook(size/2.0);


	//OpenGL�֘A�̏�����
	GraphicManager::Initialize(600, 800, argc, argv);

	//�O���b�h�̐���
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
