#include "Camera.h"
#include <gl/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
using namespace std;

Camera::Camera()
	:theta(0), phi(0), fov(90), radius(20), up(0,1,0), frustumNear(1.0), frustumFar(500.0)
{
	calcPosition();	//èâä˙à íuÇÃê›íË
}

void Camera::calcPosition()
{
	position.x = radius * cos(phi) * cos(theta) + look.x;
	position.y = radius * sin(phi) + look.y;
	position.z = radius * cos(phi) * sin(theta) + look.z;
}

void Camera::Move(float x, float y)
{
	Vector3d dir   = look - position;
	Vector3d axisX = dir.cross(up).normalize();
	Vector3d axisY = axisX.cross(dir).normalize();
	axisX.mul(x);
	axisY.mul(y);
	position += axisX - axisY;
	look     += axisX - axisY;
}

void Camera::Rotate(float _theta, float _phi)
{
	theta += _theta;
	phi   += _phi;
	
	calcPosition();
}

void Camera::Zoom(float _fov)
{
	fov = std::max(0.0f, std::min(150.0f, fov+_fov));
	cout << fov << endl;
}

void Camera::SetViewportAndMatrix() const
{
	glViewport(0, 0, width, height);

	glLoadIdentity();

	gluPerspective(fov, (double)width / (double)height, frustumNear, frustumFar);
	gluLookAt(
		position.x, position.y, position.z,
		look.x,  look.y, look.z, 
		up.x, up.y, up.z);

	glutPostRedisplay();
}

void Camera::SetWindowSize(const int &_width, const int &_height)
{
	width  = _width;
	height = _height;
}

int CameraManager::lastX;
int CameraManager::lastY;
void CameraManager::Keyboard(unsigned char key, int x, int y)
{
	float rotSpeed = 1.0;
	int d_th=0, d_ph=0;
	int d_fov = 0;
  switch (key) {
  case upKey:
	  d_ph += rotSpeed;
	break;
  case downKey:
	  d_ph -= rotSpeed;
	  break;
  case rightKey:
	  d_th += rotSpeed;
	  break;
  case leftKey:
	  d_th -= rotSpeed;
	  break;
  case zoomInKey:
	  d_fov -= 1.0;
	  break;
  case zoomOutKey:
	  d_fov += 1.0;
	  break;
  default:
    break;
  }

  if( d_th | d_ph )
	  Camera::getCamera()->Rotate(d_th * M_PI / 180.0, d_ph * M_PI / 180.0);

  if( d_fov )
	  Camera::getCamera()->Zoom(d_fov);
}

void CameraManager::MouseMove(int x, int y)
{
	float speed = 0.1;
	float moveX = speed*(x - lastX);
	float moveY = speed*(y - lastY);
	Camera::getCamera()->Move(moveX, moveY);
	lastX = x;
	lastY = y;
}

void CameraManager::MousePress(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		lastX = x;
		lastY = y;
	}
}