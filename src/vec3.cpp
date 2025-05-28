#include "includes.h" 

vec3::vec3(double x, double y, double z) : x(x), y(y), z(z) {}

vec3::vec3(const vec3& other) : x(other.x), y(other.y), z(other.z) {}

double vec3::length() const { return sqrt(x * x + y * y + z * z); }

double vec3::DotProduct(const vec3& other, const vec3& another) { return another.x * other.x + another.y * other.y + another.z * other.z; }

vec3& vec3::Norm() {

	double l = this->length();
	
	if (l == 0) return *this;
	x /= l; y /= l; z /= l;

	return *this;
}

void vec3::RotateX(const double& angle) {

	double _y = y, _z = z;

	y = _y * cos(angle) - _z * sin(angle);
	z = _y * sin(angle) + _z * cos(angle);
}

void vec3::RotateY(const double& angle) {

	double _x = x, _z = z;

	x = _x * cos(angle) - _z * sin(angle);
	z = _x * sin(angle) + _z * cos(angle);
}

void vec3::RotateZ(const double& angle) {

	double _x = x, _y = y;

	x = _x * cos(angle) - _y * sin(angle);
	y = _x * sin(angle) + _y * cos(angle);
}
