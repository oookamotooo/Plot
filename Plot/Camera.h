#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector3.h"

class Camera
{
public:
	static Camera* getCamera()
	{
		static Camera instance;
		return &instance;
	}

	void SetWindowSize(const int &width, const int &height);
	void Move(float x, float y, float z);			//���s�ړ�
	void Rotate(float theta, float phi);	//��]
	void Zoom(float delta);					//�Y�[��
	void SetViewportAndMatrix() const;

	const Vector3d GetPosition() const { return position; }
	const Vector3d GetLook() const { return look; }
private:
	Vector3d position, look;
	Vector3d up;
	float theta, phi, radius;	//��, ��, ���a
	float fov;	//����p
	float frustumNear, frustumFar;	//�N���b�s���O�ʒu(near, far����)
	int width, height;
	Camera();

	Camera(const Camera &other);

	void calcPosition();	//�ʒu�X�V
};

class CameraManager
{
public:
	static void Keyboard(unsigned char key, int x, int y);
	static void MouseMove(int x, int y);
	static void MousePress(int button, int state, int x, int y);

private:
	static int lastX, lastY;
	static const unsigned char upKey = 'w', downKey = 's', leftKey = 'a', rightKey='d'; 
	static const unsigned char zoomInKey = 'q', zoomOutKey = 'e';
};

#endif