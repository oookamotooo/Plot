#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <Eigen/Core>
using Eigen::Vector3d;

class Camera
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public:
	static Camera* getCamera()
	{
		static Camera instance;
		return &instance;
	}

	void SetWindowSize(const int &width, const int &height);	//ウィンドウサイズの変更
	void Move(float x, float y, float z);						//平行移動
	void Rotate(float theta, float phi);						//回転
	void Zoom(float delta);										//ズーム
	void SetViewportAndMatrix() const;

	const Vector3d GetPosition() const { return position; }
	const Vector3d GetLook() const { return look; }
	const Vector3d GetUp() const { return up; }
	void GetAxis(Vector3d &axisX, Vector3d &axisY, Vector3d &axisZ)const;
	const float GetRadius() const{ return radius; }
	void SetLook(const Vector3d &l);
	void SetPoistion(const Vector3d &p);
	Vector3d ScreenToWorldVector( const float &x, const float &y);
private:
	Vector3d position, look;
	Vector3d up;
	float theta, phi, radius;	//θ, φ, 半径
	float fov;	//視野角
	float frustumNear, frustumFar;	//クリッピング位置(near, far平面)
	int width, height;
	Camera();

	Camera(const Camera &other);

	void calcPosition();	//位置更新
	void calcAngle();		
};

class CameraManager
{
public:
	static void Keyboard(unsigned char key, int x, int y);
	static void MouseMove(int x, int y);
	static void MousePress(int button, int state, int x, int y);

private:
	static int lastX, lastY;
	static const unsigned char upKey = 'w', downKey = 's', leftKey = 'a', rightKey = 'd', forwardKey = 'z', backwardKey = 'c';
	static const unsigned char zoomInKey = 'q', zoomOutKey = 'e';
};

#endif