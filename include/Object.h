#ifndef OBJECT_H
#define OBJECT_H

#include "includes.h" 

class Object {
public:

	Object(double x = 0, double y = 0, double z = 0, bool sunStatus = false) : sunStatus(sunStatus) {
		pos.x = x; pos.y = y; pos.z = z;
	}

	virtual double Intersection(const vec3& cameraPos) = 0; // Возвращает точку пересечения вектора с объектом 

	virtual vec3 CalculateNormal(vec3 point) const = 0; // Возвращает вектор нормали к поверхности в заданной точке 

	vec3 pos; // Рассположение центра объекта
	bool sunStatus;
};

class Sphere : public Object {
public:

	Sphere(double x = 0, double y = 0, double z = 0, double R = 1, bool sunStatus = false) : Object(x, y, z, sunStatus), R(R) {}

	double Intersection(const vec3& cameraPos) override { // Возвращает кратчайшее расстояние от точки до сферы 
		return (cameraPos - pos).length() - R;
	}

	vec3 CalculateNormal(vec3 point) const override {

		point = point - pos;
		point.Norm();

		return point;
	}

	double R;
};

class Cube : public Object {
public:

	Cube(double _x = 0, double _y = 0, double _z = 0, double _M = 0, double _edge = 1) : Object(_x, _y, _z, _M) {
		edge = _edge;
	}

	vec3 Intersection(const vec3& cameraPos, const vec3& pixel) {
		// Возвращает либо вектор, началом которого является камера, а концом - точка пересечения c кубом, либо -1, если пересечения нет
		vec3 direction = pixel - cameraPos;
		direction.Norm();

		if (direction.x == 0) {
			if (cameraPos.x < pos.x - edge / 2 || cameraPos.x > pos.x + edge / 2) return vec3(-1, -1, -1);
		}
		if (direction.y == 0) {
			if (cameraPos.y < pos.y - edge / 2 || cameraPos.y > pos.y + edge / 2) return vec3(-1, -1, -1);
		}
		if (direction.z == 0) {
			if (cameraPos.z < pos.z - edge / 2 || cameraPos.z > pos.z + edge / 2) return vec3(-1, -1, -1);
		}

		double tEnter, tExit;

		vec3 tmin(min((edge / 2 + pos.x - cameraPos.x) / direction.x, (-edge / 2 + pos.x - cameraPos.x) / direction.x),
			min((edge / 2 + pos.y - cameraPos.y) / direction.y, (-edge / 2 + pos.y - cameraPos.y) / direction.y),
			min((edge / 2 + pos.z - cameraPos.z) / direction.z, (-edge / 2 + pos.z - cameraPos.z) / direction.z));

		vec3 tmax = vec3(max((edge / 2 + pos.x - cameraPos.x) / direction.x, (-edge / 2 + pos.x - cameraPos.x) / direction.x),
			max((edge / 2 + pos.y - cameraPos.y) / direction.y, (-edge / 2 + pos.y - cameraPos.y) / direction.y),
			max((edge / 2 + pos.z - cameraPos.z) / direction.z, (-edge / 2 + pos.z - cameraPos.z) / direction.z));

		tEnter = max(max(tmin.x, tmin.y), tmin.z);
		tExit = min(min(tmax.x, tmax.y), tmax.z);

		if (tEnter > tExit) return vec3(-1, -1, -1);

		if (tEnter > 0) return direction * tEnter;
		else if (tExit > 0) return direction * tExit;
		else return vec3(-1, -1, -1);
	}

	vec3 CalculateNormal(vec3 point) {

		vec3 del = point - pos;

		if (abs(del.x) >= abs(del.y) && abs(del.x) >= abs(del.z)) {
			return vec3((del.x > 0) ? -1 : 1, 0, 0);
		}
		else if (abs(del.y) >= abs(del.x) && abs(del.y) >= abs(del.z)) {
			return vec3(0, (del.y > 0) ? -1 : 1, 0);
		}
		else {
			return vec3(0, 0, (del.z > 0) ? -1 : 1);
		}
	}

	vec3 pos; // Положение центра куба
	double edge;
};

class Donut : public Object {
public:

	Donut(double x = 0, double y = 0, double z = 0, double r = 1, double R = 3, bool sunStatus = false) : Object(x, y, z, sunStatus), r(r), R(R) {}

	double Intersection(const vec3& point) override { // Возвращает кратчайшее расстояние от точки до тора
		return sqrt(pow(sqrt(pow(point.x - pos.x, 2) + pow(point.y - pos.y, 2)) - R, 2) + pow(point.z - pos.z, 2)) - r;
	}

	vec3 CalculateNormal(vec3 p) const override {

		p = p - pos;
		double xy_dist = sqrt(p.x * p.x + p.y * p.y);
		return vec3(p.x * (1 - R / xy_dist),p.y * (1 - R / xy_dist),p.z).Norm();
	}

	double R, r;
};

#endif OBJECT_H
