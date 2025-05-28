#include "includes.h" 

using namespace std;

void Play(World& Map) {
	// Ждём пока инициализируется консоль и считываем её характеристики
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	CONSOLE_SCREEN_BUFFER_INFO userConsole;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &userConsole);

	const int width = userConsole.srWindow.Right - userConsole.srWindow.Left + 1;
	const int height = userConsole.srWindow.Bottom - userConsole.srWindow.Top + 1;
	const double velosity = 25.0; // Скорость вращения 
	const double aspect = (double)width / height;
	const double pixelAspect = 12.0 / 21.0;
	//const double pixelAspect = 6.0 / 11.0; // Для больших разрешений

	vec3 ro; // Ray Origin
	vec3 rd; // Ray Direction 

	double* illum = new double[width * height]; // Значения освещённости объектов 
	double* distances = new double[width * height]; // Расстояние до точек на объекте 
	wchar_t* screen = new wchar_t[width * height + 1]; // Пиксели, которые выводим в консоль 
	vector<vec3> rays(width * height); // Начальные лучи камеры 
	screen[width * height] = '\0';

	// Создаём буфер для консоли, через который будем выводить символы 
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	DWORD dwBytesWritten = 0;

	for (int i = 0; i < height; i++) { // Расчитываем значения лучей камеры
		for (int j = 0; j < width; j++) {

			rd = vec3(1 / 2, (double)j / width, (double)i / height) * 2.0 - 1; // Ray Direction 
			rd.y *= aspect * pixelAspect;

			rays[i * width + j] = rd;
		}
	}

	for (int t = 0;t < 100000; t++) { // Количество кадров 
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				ro = vec3(-5, 0, 0); // Смещаем камеру на 5 единиц от центра сцены 

				rd = rays[i * width + j];

				ro.RotateZ(t / velosity);
				rd.RotateZ(t / velosity);
				ro.RotateX(t / velosity);
				rd.RotateX(t / velosity);

				rd = (rd - ro).Norm(); // Направление луча камеры 

				distances[i * width + j] = Map.RayMarch(ro, rd, illum, i * width + j); // Определяем расстояние до тела в данном направлении и его освещённость 
			}
		}
		// Заполняем массив символами в зависимости от удалённости от камеры и освещённости 
		Map.CustomizeToGradient(screen, distances, illum, width * height);

		WriteConsoleOutputCharacterW(console, screen, width * height, { 0, 0 }, &dwBytesWritten);
		this_thread::sleep_for(chrono::microseconds(200));

		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			break; // Выход из функции Play
		}
	}
	delete[] illum;
	delete[] distances;
	delete[] screen;
}

int main()
{
	World Map;
	Map.Add(new Donut(0, 0, 0, 0.4, 0.65));
	//Map.Add(new Sphere(0, 0, 0, 0.27, true));
	//Map.Add(new Sphere(2, 2, 0, 0.4));

	Play(Map);

	return 0;
}
