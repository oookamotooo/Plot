#include "Camera.h"
#include <gl/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <algorithm>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry> 
using namespace Eigen;

Camera::Camera()
	:theta(0), phi(0), fov(90), radius(20), up(0,1,0), frustumNear(0.1), frustumFar(500.0)
{
	calcPosition();	//初期位置の設定
}

void Camera::calcPosition()
{
	position.x() = radius * cos(phi) * cos(theta) + look.x();
	position.y() = radius * sin(phi) + look.y();
	position.z() = radius * cos(phi) * sin(theta) + look.z();
}

void Camera::Move(float x, float y, float z)
{
	Vector3d axisZ = (look - position).normalized();
	Vector3d axisX = axisZ.cross(up).normalized();
	Vector3d axisY = axisX.cross(axisZ).normalized();
	axisX*=x;
	axisY*=y;
	axisZ*=z;

	position += axisX + axisY + axisZ;
	look     += axisX + axisY ;	//z方向はlookに関係ない(回転ではあるけど)
	radius += z;
}

void Camera::Rotate(float _theta, float _phi)
{
	theta += _theta;
	if (theta > 2*M_PI) theta -= 2*M_PI;
	if (theta < 0) theta += 2*M_PI;

	phi   = std::min( M_PI_2 - 0.0001f, std::max( -M_PI_2 + 0.0001f, 1.0*phi + _phi));
	
	calcPosition();
}

void Camera::Zoom(float r)
{
	radius = std::max(1.0f, std::min(150.0f, radius + r));
	calcPosition();
}

void Camera::GetAxis(Vector3d &axisX, Vector3d &axisY, Vector3d &axisZ)const
{
	axisZ = (look - position);
	axisZ.normalize();

	axisX = axisZ.cross(up);
	axisX.normalize();

	axisY = axisX.cross(axisZ);
	axisY.normalize();
}

void Camera::SetViewportAndMatrix() const
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(fov, (double)width / (double)height, frustumNear, frustumFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		position.x(), position.y(), position.z(),
		look.x(), look.y(), look.z(),
		up.x(), up.y(), up.z());
}

void Camera::SetWindowSize(const int &_width, const int &_height)
{
	width  = _width;
	height = _height;
}

Vector3d Camera::ScreenToWorldVector( const float &x, const float &y)
{
	// -0.5 ~ 0. 5の範囲にスケーリング
	float screenX = (          x - width/2.0) / width;	
	float screenY = ( height - y - height/2.0) / height;

	float ratio = 1.0 * width / (float)height;

	float farHeight = 2*frustumFar*tan(0.5 * fov * M_PI / 180.0);
	float farWidth  = farHeight * ratio;

	Vector3d axisZ = (look - position);
	axisZ.normalize();

	Vector3d axisX = axisZ.cross(up);
	axisX.normalize();
	Vector3d axisY = axisX.cross(axisZ);
	axisY.normalize();

	Vector3d direction = axisY*screenY*farHeight + axisX*screenX*farWidth + axisZ*frustumFar;
	direction.normalize();
	return direction;
}

int CameraManager::lastX;
int CameraManager::lastY;
void CameraManager::Keyboard(unsigned char key, int x, int y)
{
	float rotSpeed = 1.0;
	int d_th=0, d_ph=0;
	int d_zoom = 0;
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
	  d_zoom += 1.0;
	  break;
  case zoomOutKey:
	  d_zoom -= 1.0;
	  break;
  default:
    break;
  }

  if( d_th | d_ph )
	  Camera::getCamera()->Rotate(d_th * M_PI / 180.0, d_ph * M_PI / 180.0);

  if (d_zoom)
	  Camera::getCamera()->Zoom(d_zoom);
}

void CameraManager::MouseMove(int x, int y)
{
	float speed = 0.1;
	float moveX = speed*(x - lastX);
	float moveY = speed*(y - lastY);
	Camera::getCamera()->Move(moveX, -moveY, 0);
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