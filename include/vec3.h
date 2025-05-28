#ifndef VEC3_H 
#define VEC3_H

#include "includes.h" 

class vec3 {
public:

	vec3(double x=0, double y=0, double z=0); // Базовый конструктор 
	vec3(const vec3& other); // Конструктор копирования 
	double length() const;
	vec3& Norm(); // Нормализация 
	static double DotProduct(const vec3& other, const vec3& another);
	void RotateX(const double& angle);
	void RotateY(const double& angle);
	void RotateZ(const double& angle);

	vec3& operator = (const vec3& other) {
		this->x = other.x; this->y = other.y; this->z = other.z;
		return *this;
	}

	vec3 operator + (const vec3& other) {

		vec3 temp(this->x + other.x, this->y + other.y, this->z + other.z);
		return temp;
	}

	vec3 operator - (const vec3& other) const {

		vec3 temp(this->x - other.x, this->y - other.y, this->z - other.z);
		return temp;
	}

	vec3 operator * (const vec3& other) const {

		vec3 temp(this->x * other.x, this->y * other.y, this->z * other.z);
		return temp;
	}

	vec3 operator / (const vec3& other) {

		vec3 temp(this->x / other.x, this->y / other.y, this->z / other.z);
		return temp;
	}

	vec3 operator * (const double value) const {

		vec3 temp(this->x * value, this->y * value, this->z * value);
		return temp;
	}

	vec3 operator + (const double value) const {

		vec3 temp(this->x + value, this->y + value, this->z + value);
		return temp;
	}

	vec3 operator - (const double value) const {

		vec3 temp(this->x - value, this->y - value, this->z - value);
		return temp;
	}

	double x, y, z;
};

#endif VEC3_H
