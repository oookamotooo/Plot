#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector3.h"

class Camera
{
public:
	Vector3d position, look;
	static Camera* getCamera()
	{
		static Camera instance;
		return &instance;
	}

	void SetWindowSize(const int &width, const int &height);
	void Move(float x, float y);			//平行移動
	void Rotate(float theta, float phi);	//回転
	void Zoom(float delta);					//ズーム
	void SetViewportAndMatrix() const;
private:
	Vector3d up;
	float theta, phi, radius;	//θ, φ, 半径
	float fov;	//視野角
	float frustumNear, frustumFar;	//クリッピング位置(near, far平面)
	int width, height;
	Camera();

	Camera(const Camera &other);

	void calcPosition();	//位置更新
};

class CameraManager
{
public:
	static void Keyboard(unsigned char key, int x, int y);
	static void MouseMove(int x, int y);
	static void MousePress(int button, int state, int x, int y);
	static void Resize(int w, int y);
private:
	static int lastX, lastY;
	static const unsigned char upKey = 'w', downKey = 's', leftKey = 'a', rightKey='d'; 
	static const unsigned char zoomInKey = 'q', zoomOutKey = 'e';
};

#endif