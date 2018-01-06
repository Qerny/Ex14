/*13. В матрице удалить строки с максимальным и минимальным элементами матрицы, а
затем на место первой добавить строку из произведений элементов соответствующих
столбцов.*/

//первые 2 числа в файле - размерность матрицы

//https://github.com/Rubik0000/3CppLab3

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <windows.h>

using namespace std;

//выделение памяти
float **AllocateMemory(const int m, const int n)
{
	//объявляем массив указатеей
	float **matrix = new float *[m];
	//обявляем массивы типа float
	for (int i = 0; i < m; i++)
		matrix[i] = new float[n];
	return matrix;
}

/*Чтение матрицы с консоли, возвращает указатель на матрицу*/
float **ReadMatrixFromConsole(int *m, int *n)
{
	cout << "Enter the number of rows ";
	cin >> *m;
	cout << "Enter the number of columns ";
	cin >> *n;

	float **matrix;
	if ((*m <= 0) || (*n <= 0))
	{
		cout << "Invalid dimension entered" << endl;
		matrix = NULL;
	}
	else
	{
		//объявляем массив указатеей
		matrix = AllocateMemory(*m, *n);
		cout << "Input matrix " << endl;
		for (int i = 0; i < *m; i++)
			for (int j = 0; j < *n; j++)
				if (scanf("%f", &matrix[i][j]) != 1)
				{
					matrix = NULL;
					cout << "Error reading matrix" << endl;
					break;
				}
	}
	return matrix;
}


bool CorrectChar(char c)
{
	bool corr;
	switch (c)
	{
	case '*': corr = false; break;
	case '?': corr = false; break;
	case '<': corr = false; break;
	case '>': corr = false; break;
	case ':': corr = false; break;
	case '|': corr = false; break;
	default: corr = true; break;
	}
	return corr;
}

/*корректность имени файла*/
bool CorrectName(char *name)
{
	int i = 1;
	bool OK;
	if (name[0] != '\0')
	{
		OK = CorrectChar(name[0]);
		while (name[i] != '\0' && OK) //проверяем посимвольно строку
			OK = CorrectChar(name[i++]);
	}
	return OK;
}

//ввод имени
bool InputFName(char *name)
{
	bool OK = true;
	cin >> name;
	if (!CorrectName(name))
		OK = false;
	return OK;
}

/*Чтение матрицы из файла, возвращает указатель на матрицу*/
float **ReadMatrFromFile(char *fname, int *m, int *n)
{
	FILE *f = fopen(fname, "r");
	float **matrix;
	if (f == NULL)
	{
		matrix = NULL;
		cout << "Error opening file" << endl;
	}
	else
	{
		//считывание размерности матрицы
		if (fscanf(f, "%d %d\n", m, n) != 2)
		{
			matrix = NULL;
			cout << "Error reading matrix dimension" << endl;
		}
		else
		{
			//объявляем массив указатеей
			matrix = AllocateMemory(*m, *n);
			for (int i = 0; i < *m; i++)
			{
				for (int j = 0; j < *n; j++)
					if (fscanf(f, "%f", &matrix[i][j]) != 1)
					{
						matrix = NULL;
						cout << "Error reading matrix" << endl;
						break;
					}
			}
		}
	}
	fclose(f);
	return matrix;
}

/*Очистка памяти*/
void Clean(float **matrix, const int m)
{
	for (int i = 0; i < m; i++)
		delete[] matrix[i];
	delete[] matrix;
	matrix = NULL;
}

/*Вывод матрицы на экран*/
void PrintMatr(float **matrix, const int m, const int n)
{
	cout << "************" << endl;
	for (int i = 0; i < m; i++)
	{
		//если указатель не пуст
		if (matrix[i] != NULL)
		{
			for (int j = 0; j < n; j++)
				cout << matrix[i][j] << " ";
			cout << endl;
		}
	}
	cout << "************" << endl;
}

/*Запись матрицы в файл*/
void WriteMatrInFile(char *fname, float **matrix, const int m, const int n)
{
	FILE *f = fopen(fname, "w");
	if (f == NULL)
		cout << "Ошибка открытия файла" << endl;
	else
	{
		int i, rm = 0;
		//определяем реальное кол-во строк в матрице
		for (i = 0; i < m; i++)
		{
			if (matrix[i] != NULL)
				rm++;
		}
		fprintf(f, "%d %d\n", rm, n);
		for (i = 0; i < m; i++)
		{
			//если указатель не пуст
			if (matrix[i] != NULL)
			{
				for (int j = 0; j < n; j++)
					fprintf(f, "%1.2f ", matrix[i][j]);

				fprintf(f, "\n");
			}
		}
	}
	fclose(f);
}

//Ищем наибольший элемент в матрице
float GetMaxElem(float **matrix, const int m, const int n)
{
	float max = matrix[0][0];	//изначально максимальный - первый

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] > max)
				max = matrix[i][j];
		}
	return max;
}

//Ищем наименьший элемент в матрице
float GetMinElem(float **matrix, const int m, const int n)
{
	float min = matrix[0][0];

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] < min)
				min = matrix[i][j];
		}
	return min;
}

//Удаление строк с наибольшим и наименьшим элементом
void DeleteStrs(float **matrix, const int m, const int n)
{	
	float max = GetMaxElem(matrix, m, n);
	float min = GetMinElem(matrix, m, n);

	for (int i = 0; i < m; i++)
	{
		int j = 0;
		while ((j < n) && (matrix[i][j] != max) && (matrix[i][j] != min))
		{
			j++;			//Выходим из цикла, если находим максимальный или минимальный, или, если доходим до конца
		}
		if ((matrix[i][j] == max) || (matrix[i][j] == min))
		{
			delete[] matrix[i];
			matrix[i] = NULL;
		}
	}
}

//Возвращаем матрицу с добавленной первой строкой, состоящей из произведений соответствующих элементов столбцов
float **AddColOfCompositionElems(float **matrix, int *m, const int n)
{
	(*m)++;

	float** matrix2 = AllocateMemory(*m, n);
	
	for (int j = 0; j < n; j++)		//Инициализируем единицей первую строку
		matrix2[0][j] = 1;
	
	for (int i = 0; i < *(m)-1; i++)
	{
		if (matrix[i] != NULL)
			for (int j = 0; j < n; j++)
			{
				matrix2[0][j] *= matrix[i][j];
				matrix2[i + 1][j] = matrix[i][j];
			}
		else
			matrix2[i+1] = NULL;
	}

	Clean(matrix, *(m)-1);	//Очищаем первую матрицу
	return matrix2;
}

/*главное меню*/
void MainMenu(float **matrix, int *m, int *n)
{
	int ans;
	const int t = 2;
	bool OK;
	char filename[] = "";
	cout << "1 - Input the matrix from the keyboard" << endl;
	cout << "2 - Read the matrix from the file" << endl;
	cout << "0 - Exit" << endl;
	do
	{
		cout << "Select the appropriate menu item: ";
		cin >> ans;
	} while ((ans < 0) || (ans > t));
	switch (ans)
	{
	case 1:
		matrix = ReadMatrixFromConsole(m, n);
		break;

	case 2:
	{
		cout << "Input file name" << endl;
		if (InputFName(filename))
			matrix = ReadMatrFromFile(filename, m, n);
		else
		{
			cout << "Incorrect file name" << endl;
			matrix = NULL;
		}
	}
	break;

	default:
		matrix = NULL;
		break;
	}
	OK = matrix != NULL;
	//если матрица успешно проинициализирована
	if (OK)
	{
		cout << "Deleted" << endl;
		DeleteStrs(matrix, *m, *n);
		float **matrix2 = AddColOfCompositionElems(matrix, m, *n);

		cout << "1 - Output matrix on the display" << endl;
		cout << "2 - Record matrix in a file" << endl;
		cout << "0 - Exit" << endl;
		do
		{
			cout << "Select the appropriate menu item: ";
			cin >> ans;
		} while ((ans < 0) || (ans > t));
		switch (ans)
		{
		case 1:
			PrintMatr(matrix2, *m, *n);
			break;

		case 2:
		{
			cout << "Input File name: " << endl;
			if (InputFName(filename))
				WriteMatrInFile(filename, matrix2, *m, *n);
			else
			{
				cout << "Incorrect file name" << endl;
				matrix2 = NULL;
			}
		}
		break;
		}
		//очистка памяти
		Clean(matrix2, *m);
	}


}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	float **Matrix = NULL;	//
	int M, N;
	MainMenu(Matrix, &M, &N);
	cout << "To continue, press enter ...";
	_getch();
	return 0;
}
