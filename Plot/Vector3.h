#ifndef VECTOR_3_H
#define VECTOR_3_H
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <sstream>
#include <complex>
/*
template<class T>
class Vector3
{
public:
	T x, y, z;

	Vector3(const T &_x = 0, const T &_y = 0, const T &_z = 0);
	Vector3(const Vector3<T> &other);

	Vector3& set(const T &x, const T &y, const T &z);
	Vector3& set(const Vector3 &other);

	Vector3& add(const T &x, const T &y, const T &z);
	Vector3& add(const Vector3<T> &other);

	Vector3& sub(const T &x, const T &y, const T &z);
	Vector3& sub(const Vector3 &other);

	Vector3& mul(const T &scalar);

	double length() const;

	Vector3& normalize();				//正規化する
	Vector3 normalizedVector() const;	//正規化ベクトルを返す

	//距離
	double dist(const Vector3 &other) const;
	double dist(const T &_x, const T &_y, const T &_z) const;

	//2乗距離
	double distSquared(const Vector3 &other) const;
	double distSquared(const T &_x, const T &_y, const T &_z) const;

	T dot(const Vector3 &other) const;			//内積
	Vector3 cross(const Vector3 &other) const;	//外積

	//ラジアンで返る
	double angleTo(const Vector3 &other) const;

	Vector3& operator=(const Vector3 &other);

	Vector3& operator-=(const Vector3 &other);

	Vector3& operator+=(const Vector3 &other);

	Vector3& operator*=(const float &scalar);

	Vector3 operator+() const;

	Vector3 operator-() const;

	Vector3 operator+(const Vector3 &rhs) const;

	Vector3 operator-(const Vector3 &rhs) const;

	Vector3 operator*(const double &k) const;
		
	Vector3 operator/(const double &k) const;
	
	template< class U>
	friend std::ostream& operator<<(std::ostream& s, const Vector3 &rhs);

	template< class U>
	friend std::stringstream& operator<<(std::stringstream& s, const Vector3 &rhs);

	//フロート型に変換
	Vector3<float> toFloatVector();

	template <class U>
	Vector3<double> rotatedVector(Vector3<U> axis, const float degree);

	template< class U>
	friend Vector3 operator*(const double &k, const Vector3 &rhs);
};

template<class T>
Vector3<T> operator*(const double &k, const Vector3<T> &rhs)
{
	return Vector3<T>(rhs.x*k, rhs.y*k, rhs.z*k);
}

template<class T>
std::ostream& operator<<(std::ostream& s, const Vector3<T> &rhs)
{
	return s << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
}

template<class T>
std::stringstream& operator<<(std::stringstream& s, const Vector3<T> &rhs)
{
	return s << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
}

template<class T>
template <class U>
Vector3<double> Vector3<T>::rotatedVector(Vector3<U> axis, const float degree)
{
	Vector3<double> result;
	double theta;

	theta = degree * M_PI / 180.0;

	double C = cos(theta);
	double S = sin(theta);
	result.x = ((axis.x*axis.x*(1 - C) + C)*x + (axis.x*axis.y*(1 - C) - axis.z*S)*y + (axis.z*axis.x*(1 - C) + axis.y*S)*z);
	result.y = ((axis.x*axis.y*(1 - C) + axis.z*S)*x + (axis.y*axis.y*(1 - C) + C)*y + (axis.y*axis.z*(1 - C) - axis.x*S)*z);
	result.z = ((axis.z*axis.x*(1 - C) - axis.y*S)*x + (axis.y*axis.z*(1 - C) + axis.x*S)*y + (axis.z*axis.z*(1 - C) + C)*z);

	return result;
}

template<>
template<class U>
Vector3<double> Vector3< std::complex<double> >::rotatedVector(Vector3<U> axis, const float degree)
{
	std::cerr << "Complex Vector doesn't have rotated function" << std::cerr;
	exit(2);
	return Vector3<double>();
}

typedef Vector3<int>    Vector3i;
typedef Vector3<double> Vector3d;
typedef Vector3<float>  Vector3f;
typedef Vector3<std::complex<double>> Vector3c;
*/
#endif
