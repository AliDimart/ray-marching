#ifndef WORLD_H
#define WORLD_H

#include "includes.h" 

const int maxDist = 1000;
const double epsilon = 0.001;
const std::string gradient = "@$8R960HXEUS4Z1lY7()Tcr/!;:... ";
const std::string inverseGradient = " .:;!/rcT(7YlZSU09R8$@";

class World {
public:

	double RayMarch(const vec3& ro, vec3& rd, double* illum, const int index) {

		vec3 pos;
		double dist = 0.0, maxStep;
		int objectIndex;

		for (int i = 0;i < maxDist; i++) {

			pos = rd * dist + ro; // Расчитываем положение камеры в данный момент 

			objectIndex = GetTheNearestObject(pos); // Получаем индекс ближайшего объекта 
			(objectIndex == -1) ? maxStep = maxDist : maxStep = SDF(pos, objectIndex); // Получаем расстояние до ближайшего объекта (-1 только если объектов нет)

			if (maxStep < epsilon) { // Камера подобралась достаточно близко к одному из объектов 

				if (objectIndex != -1) illum[index] = CalculateIllumination(pos, objectIndex);

				return dist;
			}
			else illum[index] = 0; // Если объекта нет, то освещённость устанавливаем в ноль 

			dist += maxStep;
			if (dist > maxDist) {
				break;
			}
		}
		return -1; // Если нет пересечений на данном направлении, то расстояние отрицательно  
	}

	double SDF(const vec3& pos, const int objectIndex) { // Возвращаем расстояние до ближайшего объекта 

		return objects[objectIndex]->Intersection(pos);
	}

	int GetTheNearestObject(const vec3& pos) {

		if (objects.size() == 0) return -1; // Если нет объектов 

		double minDist = objects[0]->Intersection(pos); // Если объекты есть, то находим индекс ближайшего из них 
		double temp; int index = 0;

		for (int i = 1; i < objects.size(); i++) {
			temp = objects[i]->Intersection(pos);
			if (temp < minDist) {
				minDist = temp;
				index = i;
			}
		}
		return index;
	}

	double CalculateIllumination(const vec3& intersection, int objectIndex) { // Расчитываем освещенность объекта в точке пересечения 

		if (objects[objectIndex]->sunStatus) return (int)lights.size(); // Если объект сам является солнцем, то устанавливаем максимальную яркость 

		vec3 light;
		double illumination = 0; // В противном случае находим суммарную освещённость от всех объектов 
		vec3 norm = objects[objectIndex]->CalculateNormal(intersection); // Расчитываем нормаль к поверкхности объекта в данной точке 

		for (int j = 0;j < lights.size();j++) { // Освещённость расчитывается, как скалярное произведение нормали и луча соединяющего источник света и точку пересечения 
			light = lights[j] - intersection; // Луч соединяющий источник света и точку на поверхности 
			illumination += vec3::DotProduct(light.Norm(), norm) / light.length();
			// Также делим на длину светового луча, чтобы учитывать отдалённость от источника освещения 
		}
		return (illumination + lights.size()) / (lights.size() * 2); // Делаем освещённость положительной от нуля до единицы 
	}

	void CustomizeToGradient(wchar_t* screen, double* distances, double* illum, int arrSize) {

		double max = distances[0], min = 10e9;
		double maxI = illum[0], minI = min;
		// Находим максимальное и минимальное расстояния до объектов 
		for (int i = 0; i < arrSize; i++) {
			if (distances[i] < 0) continue;
			if (distances[i] > max) max = distances[i];
			else if (distances[i] < min) min = distances[i];
		}
		// Находим интервал расстояний 
		max -= min;

		if (lights.size() != 0) {
			for (int i = 1; i < arrSize; i++) {
				if (illum[i] > maxI) maxI = illum[i];
				else if (illum[i] < minI) minI = illum[i];
			}
			// Находим интервал освещённости  
			maxI -= minI;
		}

		if (lights.size() != 0) { // Заполняем массив символами градиента в зависимости от освещённости и удалённости от камеры 
			for (int i = 0; i < arrSize; i++) { // Пробегаем по всем пикселям 

				if (distances[i] < 0) { // Если нет пересечений на данном направлении, то ставим самый тёмный пиксель  
					screen[i] = L' ';
					continue;
				}
				// Есть два варианта, можно подсчитывать яркость в зависимости от расстояния и освещённости 
				// Здесь мы складываем нормализованное расстояние, умноженное на учёт вклада света с нормализованной обратной освещённостью, умноженной на силу света 
				//distances[i] = ((distances[i] - min) / max) * (gradientSize - lights.size() * illumForce) + lights.size() * illumForce - ((illum[i] - minI) / maxI) * (lights.size() * illumForce); 

				distances[i] = lights.size() * illumForce - illum[i] * (lights.size() * illumForce); // Можно только в зависимости от освещённости

				int index = min(gradientSize, max(0, (int)distances[i]));

				screen[i] = gradient[index];
			}
		}
		else { // Заполняем массив символами градиента в зависимости только от удалённости от камеры  
			for (int i = 0; i < arrSize; i++) {
				
				if (distances[i] < 0) { // Если нет пересечений на данном направлении, то ставим самый тёмный пиксель  
					screen[i] = L' ';
					continue;
				}
				// В зависимости от расстояния выбираем подходящий символ (чем дальше от камеры, тем менее яркий символ)  
				distances[i] = ((distances[i] - min) / max) * gradientSize;

				int index = (int)distances[i];

				screen[i] = gradient[index];
			}
		}
	}

	void AddLight(const vec3& light) { lights.push_back(light); }
	void Add(Object* obj) { 
		objects.push_back(obj); // Если объект является солнцем, добавляем его в массив источников света 
		if (obj->sunStatus) lights.push_back(obj->pos); 
	}
	void DeleteLight() {
		if (!lights.empty()) {
			lights.pop_back();
		}
	}
	void Delete() {
		if (!objects.empty()) {
			delete objects.back();
			objects.pop_back();
		}
	}

	~World() {
		for (int i = 0; i < objects.size(); i++) { // Очищаем динамическую память, выделенную под объекты 
			delete objects[i];
		}
		objects.clear();
		lights.clear();
	}

private:
	std::vector<Object*> objects; // Содержит объекты сцены 
	std::vector<vec3> lights; // Содержит источники света (Объекты, помеченные статусом солнце) 
	const int gradientSize = (int)size(gradient) - 1; 
	const double illumForce = 35; // Показатель силы света  
};

#endif WORLD_H
