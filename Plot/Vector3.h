#ifndef VECTOR_3_H
#define VECTOR_3_H

#include <cmath>
#include <iostream>
#include <sstream>

template<class T>
class Vector3
{
public:
	static const float TO_RADIANS;// = (1.0f/180.f)*M_PI;
	static const float TO_DEGREE;//  = 180.0f/M_PI;
	T x, y, z;

	Vector3<T>()
		:x(0), y(0), z(0)
	{
	}

	Vector3<T>(const T &_x, const T &_y, const T &_z)
		:x(_x), y(_y), z(_z)
	{
	}

	Vector3<T>(const Vector3<T> &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	Vector3<T>& set(const T &x, const T &y, const T &z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		return *this;
	}

	Vector3<T>& set(const Vector3<T> &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	Vector3<T>& add(const T &x,const T &y, const T &z)
	{
		this->x += x;
		this->y += y;
		this->z += z;
		return *this;
	}

	Vector3<T>& add(const Vector3<T> &other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;    
		return *this;
	}

	Vector3<T>& sub(const T &x,const T &y, const T &z)
	{
		this->x -= x;
		this->y -= y;
		this->z -= z;
		return *this;
	}

	Vector3<T>& sub(const Vector3<T> &other){
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	Vector3<T>& mul(const T &scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	float length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	Vector3<T>& normalize(){
		float len = length();

		if(len != 0){
			x /= len;
			y /= len;
			z /= len;
		}
		return *this;
	}

	Vector3<T> normalizedVector() const {
		float len = length();
		float _x=0, _y=0, _z=0;
		if(len != 0){
			_x = x/len;
			_y = y/len;
			_z = z/len;
		}
		return Vector3<T>(_x,_y,_z);
	}

	//距離
	float dist(Vector3<T> other) const
	{
		float distX = this->x - other.x;
		float distY = this->y - other.y;
		float distZ = this->z - other.z;

		return sqrt(distX*distX + distY*distY + distZ*distZ); 
	}

	float dist(const T &_x, const T &_y, const T &_z) const
	{
		float distX = this->x - _x;
		float distY = this->y - _y;
		float distZ = this->z - _z;
		return sqrt(distX*distX + distY*distY + distZ*distZ); 
	}

	float distanceTo(const Vector3<T> &other) const
	{
		float _x = other.x - this->x;
		float _y = other.y - this->y;
		float _z = other.z - this->z;
		return sqrt(_x*_x + _y*_y + _z*_z);
	}

	//2乗距離
	float distSquared(const Vector3<T> &other) const
	{
		T distX = this->x - other.x;
		T distY = this->y - other.y;
		T distZ = this->z - other.z;

		return distX*distX + distY*distY + distZ*distZ; 
	}

	double distSquared(const T &_x, const T &_y, const T &_z) const
	{
		float distX = this->x - _x;
		float distY = this->y - _y;
		float distZ = this->z - _z;

		return distX*distX + distY*distY + distZ*distZ; 
	}

	//内積
	double dot(const Vector3<T> &other) const
	{
		return this->x*other.x + this->y*other.y + this->z*other.z;
	}

	//外積
	Vector3 cross(const Vector3<T> &other) const
	{
		return Vector3(this->y*other.z - this->z*other.y,
			this->z*other.x - this->x*other.z,
			this->x*other.y - this->y*other.x);
	}

	//ラジアンで返る
	double angleTo(const Vector3<T> &other) const
	{
		float dot  = this->dot(other);
		float len1 = this->length();
		float len2 = other.length();

		//零ベクトルとの角度は0とする
		if(len1 ==0 || len2 == 0)
			return 0;

		return acos(dot/len1/len2);
	}

	Vector3& operator=(const Vector3<T> &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;    
		return *this;
	}

	Vector3& operator-=(const Vector3<T> &other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	Vector3& operator+=(const Vector3<T> &other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	Vector3& operator*=(const float &scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	Vector3 operator+() const
	{
		return *this;
	}

	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 operator+(const Vector3<T> &rhs) const
	{
		return Vector3(this->x+rhs.x, this->y+rhs.y, this->z+rhs.z);
	}

	Vector3 operator-(const Vector3<T> &rhs) const
	{
		return Vector3(this->x-rhs.x, this->y-rhs.y, this->z-rhs.z);
	}

	Vector3 operator*(const double &k) const
	{
		return Vector3(this->x*k, this->y*k, this->z*k);
	}
		
	Vector3<T> operator/(const double &k) const
	{
		return Vector3(this->x/k, this->y/k, this->z/k);
	}
	
	friend std::stringstream& operator<<(std::stringstream& s, const Vector3 &rhs)
	{
	return s << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
	}
	
};

template<class C>
Vector3<C> operator*(const double &k, const Vector3<C> &rhs)
{
	return Vector3<C>(rhs.x*k, rhs.y*k, rhs.z*k);
}

template<class C>
std::ostream& operator<<(std::ostream &s, const Vector3<C> &rhs)
{
	return s << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
}

typedef Vector3<int> Vector3i;
typedef Vector3<double> Vector3d;
#endif
