#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <errno.h>
#include <fstream>
#include <iostream> 
using namespace std;


// интерполяционный лагранжев сплайн 3 порядка
int count_of_elements; // количество конечных элементов 
double* grid;

int read_grid(double*& grid)
{
	fstream file_grid;										// файл с сеткой
	file_grid.open("grid.txt");

	if (file_grid.is_open())
	{
		file_grid >> count_of_elements;

		grid = new double[count_of_elements * 3];

		for (int i = 0; i < count_of_elements * 3 + 1; i++)
		{
			file_grid >> grid[i];
		}
	}
	else
	{
		file_grid.close();
		perror("ERROR: open file");
		return (1);
	}
	file_grid.close();
	return(0);
}

double Ksi(double x, int i, double h) // страшная буква 
{
	return (x - grid[i]) / h;
}
double Betta(int i, double h)
{
	return (grid[i + 1] - grid[i]) / h;
}
double Gamma(int i, double h)
{
	return (grid[i + 2] - grid[i]) / h;
}
double f_func(double x)
{
	return x*x*x;
}

// 38 в методе по сплайнам
double psi_0(double ksi, double betta, double gamma)
{
	return ((ksi - betta) / -betta) * ((ksi - gamma) / -gamma) * ((ksi - 1) / -1);
}
double psi_1(double ksi, double betta, double gamma)
{
	return (ksi / betta) * ((ksi - gamma) / (betta - gamma)) * ((ksi - 1) / (betta - 1));
}
double psi_2(double ksi, double betta, double gamma)
{
	return (ksi / gamma) * ((ksi - betta) / (gamma - betta)) * ((ksi - 1) / (gamma - 1));
}
double psi_3(double ksi, double betta, double gamma)
{
	return ksi * ((ksi - betta) / (1 - betta)) * ((ksi - gamma) / (1 - gamma));
}

double Spline(double x)
{
	double ksi;
	double betta;
	double gamma;
	double h;

	double f0;
	double f1;
	double f2;
	double f3;

	double phi0;
	double phi1;
	double phi2;
	double phi3;
	double spline = 0;


	for (int i = 0; i < count_of_elements; i++)
	{
		//Получение значения сплайна в точке заключается в поиске нужного интервала 
		//и вычисления линейной комбинации локальных базисных функций//

		if (x >= grid[i * 3] && x <= grid[3 * i + 3]) // если дохожу до последнего  grid[3*2 +3 = 9] 
		{
			h = grid[i * 3 + 3] - grid[i * 3];
			ksi = Ksi(x, i, h);
			betta = Betta(i, h);
			gamma = Gamma(i, h);

			f0 = f_func(grid[i]);
			f1 = f_func(grid[i + 1]);
			f2 = f_func(grid[i + 2]);
			f3 = f_func(grid[i + 3]);

			phi0 = psi_0(ksi, betta, gamma);
			phi1 = psi_1(ksi, betta, gamma);
			phi2 = psi_2(ksi, betta, gamma);
			phi3 = psi_3(ksi, betta, gamma);

			spline = phi0 * f0 + phi1 * f1 + phi2 * f2 + phi3 * f3;
			return spline;
		}
	}
}
void main()
{
	setlocale(LC_ALL, "Russian");
	read_grid(grid);

	double x = 0;
	double step = 0.5;
	printf("x               f(x)	               Spline(x)	            |f(x) - spline(x)|\n");
	for (int i = 0; i <= grid[count_of_elements * 3] / step; i++)
	{

		printf("%2.4le  %2.18le  %2.18le  %2.18le\n", x, f_func(x), Spline(x), abs(f_func(x) - Spline(x)));
		x += step;
	}
}