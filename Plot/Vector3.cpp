#include "Vector3.h"
/*
template<class T>
Vector3<T>::Vector3(const T &_x, const T &_y, const T &_z) 
	:x(_x), y(_y), z(_z)
{
}
template<class T>
Vector3<T>::Vector3(const Vector3 &other)
	: x(other.x), y(other.y), z(other.z)
{
}

template<class T>
Vector3<T>& Vector3<T>::set(const T &x, const T &y, const T &z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::set(const Vector3 &other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::add(const T &x, const T &y, const T &z)
{
	this->x += x;
	this->y += y;
	this->z += z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::add(const Vector3 &other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::sub(const T &x, const T &y, const T &z)
{
	this->x -= x;
	this->y -= y;
	this->z -= z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::sub(const Vector3 &other){
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::mul(const T &scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	return *this;
}

template<class T>
double Vector3<T>::length() const
{
	return sqrt(x*x + y*y + z*z);
}

template<>
double Vector3< std::complex<double> >::length() const
{
	return sqrt(std::norm(x) + std::norm(y) + std::norm(z));
}

template<class T>
Vector3<T>& Vector3<T>::normalize(){
	float len = length();

	if (len != 0){
		x /= len;
		y /= len;
		z /= len;
	}
	return *this;
}

template<class T>
Vector3<T> Vector3<T>::normalizedVector() const {
	float len = length();
	return len != 0 ? 
		Vector3<T>( x / len, y / len, z / len) : Vector3();
}

template<>
Vector3<std::complex<double> > Vector3< std::complex<double> >::normalizedVector() const {
	float len = length();
	return len != 0 ?
		Vector3<std::complex<double> >(x, y, z) / len : Vector3();
}

//2乗距離
template<class T>
double Vector3<T>::distSquared(const Vector3 &other) const
{
	return distSquared(other.x, other.y, other.z);
}

template<class T>
double Vector3<T>::distSquared(const T &_x, const T &_y, const T &_z) const
{
	float distX = this->x - _x;
	float distY = this->y - _y;
	float distZ = this->z - _z;

	return distX*distX + distY*distY + distZ*distZ;
}

template<>
double Vector3< std::complex<double> >::distSquared(
	const std::complex<double> &_x,
	const std::complex<double> &_y, 
	const std::complex<double> &_z) const
{
	std::complex<double> distX = this->x - _x;
	std::complex<double> distY = this->y - _y;
	std::complex<double> distZ = this->z - _z;

	return std::norm(distX) + std::norm(distY) + std::norm(distZ);
}


template<class T>
double Vector3<T>::dist(const Vector3 &other) const
{
	return sqrt(distSquared(other));
}

template<class T>
double Vector3<T>::dist(const T &_x, const T &_y, const T &_z) const
{
	return sqrt(distSquared(_x, _y, _z));
}

//内積
template<class T>
T Vector3<T>::dot(const Vector3 &other) const
{
	return this->x*other.x + this->y*other.y + this->z*other.z;
}

//外積
template<class T>
Vector3<T> Vector3<T>::cross(const Vector3 &other) const
{
	return Vector3(this->y*other.z - this->z*other.y,
		this->z*other.x - this->x*other.z,
		this->x*other.y - this->y*other.x);
}

template<class T>
double Vector3<T>::angleTo(const Vector3 &other) const
{
	double dot  = this->dot(other);
	double len1 = this->length();
	double len2 = other.length();

	//零ベクトルとの角度は0とする
	if (len1 == 0 || len2 == 0)
		return 0;
	
	return acos(dot / len1 / len2);
}

template<>
double Vector3< std::complex<double> >::angleTo(const Vector3< std::complex<double> > &other) const
{
	double dot = std::abs(this->dot(other));
	double len1 = this->length();
	double len2 = other.length();

	//零ベクトルとの角度は0とする
	if (len1 == 0 || len2 == 0)
		return 0;

	return acos(dot / len1 / len2);
}

template<class T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T> &other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T> &other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T> &other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

template<class T>
Vector3<T>& Vector3<T>::operator*=(const float &scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	return *this;
}

template<class T>
Vector3<T> Vector3<T>::operator+() const
{
	return *this;
}

template<class T>
Vector3<T> Vector3<T>::operator-() const
{
	return Vector3(-x, -y, -z);
}

template<class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T> &rhs) const
{
	return Vector3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

template<class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T> &rhs) const
{
	return Vector3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

template<class T>
Vector3<T> Vector3<T>::operator*(const double &k) const
{
	return Vector3(this->x*k, this->y*k, this->z*k);
}

template<class T>
Vector3<T> Vector3<T>::operator/(const double &k) const
{
	return Vector3(this->x / k, this->y / k, this->z / k);
}

//フロート型に変換
template<class T>
Vector3<float> Vector3<T>::toFloatVector()
{
	return Vector3<float>(x, y, z);
}

template<>
Vector3<float> Vector3< std::complex<double> >::toFloatVector()
{
	return Vector3<float>( std::norm(x), std::norm(y), std::norm(z));
}


template class Vector3 < int >; 
template class Vector3 < double >;
template class Vector3 < float >;
template class Vector3 < std::complex<double> >;
*/