#include "pch.h"
#include <iostream>
#include "Algorytm_johnsona.h"
#include <Windows.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> T1, T2, T3, T4, T5; // czasy poszczególnych zadañ na poszczególnych maszynach
vector<int> Zadania; // vector z iloœci¹ zadañ
vector<int> Kolejnosc; // ostateczna kolejnoœæ zadañ
vector<int> L1, L2; // vectory z kolejnoœci¹ zadañ z obu maszyn
int ilosc_maszyn, ilosc_zadan;

void Pobierz_dane()
{

	cout << "Podaj iloœæ maszyn: ";
	cin >> ilosc_maszyn;

	cout << "Podaj iloœæ zadañ: ";
	cin >> ilosc_zadan;

	system("cls");
}

void Utworz_dane()
{
	if (ilosc_maszyn == 2)
	{
		for (int i = 0; i < ilosc_zadan * 2; i++)
		{
			T1.push_back((rand() % 20) + 1);
		}
	}
	else if (ilosc_maszyn == 3)
	{
		for (int i = 0; i < ilosc_zadan * 3; i++)
		{
			T1.push_back((rand() % 25) + 1);
		}
	}
	else
	{
		cout << "Nieobs³ugiwana iloœæ maszyn!" << endl;
	}

	for (int m = 0; m < ilosc_zadan; m++) // tablica zadañ na zasadzie silni
	{
		Zadania.push_back(m + 1);
	}

	T5 = T1;
}

void Algorytm_Johnsona()
{
	int Najmniejszy;
	int i, j;
	int Tymczasowa;

	if (ilosc_maszyn == 2)
	{
		for (int k = 0; k < ilosc_zadan; k++)
		{
			Najmniejszy = T1[0];
			i = 0;
			j = 0;
			while (T1.size() != i)
			{
				if (T1[i] < Najmniejszy)
				{
					Najmniejszy = T1[i];
					j = i;
				}
				i++;
			}
			if (ilosc_zadan > j)
			{
				L1.push_back(j);
				T1[j] = 99;
				T1[j + ilosc_zadan] = 99;
			}
			else
			{
				L2.insert(L2.begin(), j);
				T1[j] = 99;
				T1[j - ilosc_zadan] = 99;
			}

		}

		Kolejnosc.reserve(L1.size() + L2.size());
		Kolejnosc.insert(Kolejnosc.end(), L1.begin(), L1.end());
		Kolejnosc.insert(Kolejnosc.end(), L2.begin(), L2.end());
	}
	else if (ilosc_maszyn == 3)
	{
		for (int f = 0; f < ilosc_zadan; f++)
		{
			T2.push_back(T1[f] + T1[f + ilosc_zadan]);
			T3.push_back(T1[f + ilosc_zadan] + T1[f + 2 * ilosc_zadan]);
		}

		T4.reserve(T2.size() + T3.size());
		T4.insert(T4.end(), T2.begin(), T2.end());
		T4.insert(T4.end(), T3.begin(), T3.end());


		for (int h = 0; h < ilosc_zadan; h++)
		{
			Najmniejszy = T4[0];
			i = 0;
			j = 0;
			while (T4.size() != i)
			{
				if (T4[i] < Najmniejszy)
				{
					Najmniejszy = T4[i];
					j = i;
				}
				i++;
			}
			if (ilosc_zadan > j)
			{
				L1.push_back(j);
				T4[j] = 99;
				T4[j + ilosc_zadan] = 99;
			}
			else
			{
				L2.insert(L2.begin(), j);
				T4[j] = 99;
				T4[j - ilosc_zadan] = 99;
			}

		}

		Kolejnosc.reserve(L1.size() + L2.size());
		Kolejnosc.insert(Kolejnosc.end(), L1.begin(), L1.end());
		Kolejnosc.insert(Kolejnosc.end(), L2.begin(), L2.end());
	}
	else
	{
		cout << "Nieobs³ugiwana iloœæ maszyn!" << endl;
	}
}

void Wyswietl()
{
	cout << "T: ";

	if (ilosc_maszyn == 2)
	{

		for (size_t i = 0; i < T5.size(); i++)
		{
			cout << T5[i] << " ";
			if (T5.size() / 2 - 1 == i)
			{
				cout << endl << "   ";
			}
		}
		cout << endl;
	}
	else if (ilosc_maszyn == 3)
	{
		for (size_t i = 0; i < T1.size(); i++)
		{
			cout << T1[i] << " ";
			if (T1.size() / 3 - 1 == i)
			{
				cout << endl << "   ";
			}

			if ((T1.size() / 3 - 1) * 2 + 1 == i)
			{
				cout << endl << "   ";
			}
		}
		cout << endl;
	}

	cout << "L1: ";
	for (size_t a = 0; a < L1.size(); a++)
	{
		cout << L1[a] << " ";
	}
	cout << endl;

	cout << "L2: ";
	for (size_t b = 0; b < L2.size(); b++)
	{
		cout << L2[b] << " ";
	}
	cout << endl;

	//cout << "Zadania: ";
	//	for (size_t k = 0; k < Zadania.size(); k++)
	//	{
	//		cout << Zadania[k] << " ";
	//	}
	//cout << endl;

	cout << "Kolejnoœæ: ";
	for (size_t k = 0; k < Kolejnosc.size(); k++)
	{
		if (ilosc_zadan < Kolejnosc[k])
		{
			cout << Kolejnosc[k] + 1 - ilosc_zadan << " ";
		}
		else
		{
			cout << Kolejnosc[k] + 1 << " ";
		}
	}
	cout << endl;


	cout << "Cmax: " << C_max() << endl;
}

int C_max()
{
	if (ilosc_maszyn == 2)
	{
		int cmax = T5[0];
		int tmp_span = 0;
		for (int i = 1; i < ilosc_zadan; i++)
		{
			cmax += T5[i];
			if (tmp_span == 0)
				tmp_span = T5[i - 1 + ilosc_zadan];

			if (tmp_span <= T5[i])
			{
				if (ilosc_zadan - 1 != i)
					tmp_span = 0;
			}
			else
			{
				tmp_span = tmp_span + T5[i + ilosc_zadan] - T5[i];
			}
		}
		cmax += tmp_span;

		return cmax;

	}
	else if (ilosc_maszyn == 3)
	{
		for (int f = 0; f < ilosc_zadan; f++)
		{
			T2.push_back(T1[f] + T1[f + ilosc_zadan]);
			T3.push_back(T1[f + ilosc_zadan] + T1[f + 2 * ilosc_zadan]);
		}

		T4.reserve(T2.size() + T3.size());
		T4.insert(T4.end(), T2.begin(), T2.end());
		T4.insert(T4.end(), T3.begin(), T3.end());

		int cmax = T4[0];
		int tmp_span = 0;
		for (int i = 1; i < ilosc_zadan; i++)
		{
			cmax += T4[i];
			if (tmp_span == 0)
				tmp_span = T4[i - 1 + ilosc_zadan];

			if (tmp_span <= T4[i])
			{
				if (ilosc_zadan - 1 != i)
					tmp_span = 0;
			}
			else
			{
				tmp_span = tmp_span + T4[i + ilosc_zadan] - T4[i];
			}
		}
		cmax += tmp_span;

		return cmax;
	}
}



void Przeglad_zupelny()
{
	int *Tablica_M1 = new int[ilosc_zadan];
	int *Tablica_M2 = new int[ilosc_zadan];

	for (int i = 0; i < ilosc_zadan; i++)
	{
		Tablica_M1[i] = (rand() % 15) + 1;
		Tablica_M2[i] = (rand() % 25) + 1;
	}

	for (int j = 0; j < ilosc_zadan; j++)
	{
		for (int k = 0; k < ilosc_zadan - 1; k++)
		{
			if (Tablica_M1[k] > Tablica_M1[k + 1])
			{
				swap(Tablica_M1[k], Tablica_M1[k + 1]);
				swap(Tablica_M2[k], Tablica_M2[k + 1]);
			}
		}
	}

	cout << endl << endl;
	cout << "T1\t" << "T2" << endl;
	cout << "--------------" << endl;
	for (int h = 0; h < ilosc_zadan; h++)
	{
		cout << Tablica_M1[h] << "\t";
		cout << Tablica_M2[h] << endl;
	}


	cout << endl;
	cout << "Permutacje:" << endl;
	cout << "T1" << endl;
	Permutacje(Tablica_M1, ilosc_zadan, ilosc_zadan, 1);
	cout << "--------------" << endl;
	cout << "T2" << endl;
	Permutacje(Tablica_M2, ilosc_zadan, ilosc_zadan, 2);

	/*for ()
	{

	}*/

	int cmax = T5[0];
	int tmp_span = 0;

	/*for (int i = 1; i < ilosc_zadan; i++)
	{
		cmax += T5[i];
		if (tmp_span == 0)
			tmp_span = T5[i - 1 + ilosc_zadan];

		if (tmp_span <= T5[i])
		{
			if (ilosc_zadan - 1 != i)
				tmp_span = 0;
		}
		else
		{
			tmp_span = tmp_span + T5[i + ilosc_zadan] - T5[i];
		}
	}
	cmax += tmp_span;

	cout << cmax << endl;*/

	delete[] Tablica_M1;
	delete[] Tablica_M2;
}



void Permutacje(int a[], int size, int n, int abc)
{

	if (size == 1)
	{
		for (int i = 0; i < n; i++)
		{
			cout << a[i] << " ";
			if (abc == 1)
			{
				T1.push_back(a[i]);
			}
			else
			{
				T2.push_back(a[i]);
			}
		}
		cout << endl;
	}

	for (int i = 0; i < size; i++)
	{
		if (abc == 1)
		{
			Permutacje(a, size - 1, n, 1);
		}
		else
		{
			Permutacje(a, size - 1, n, 2);
		}
		


		if (size % 2 == 1)
		{
			swap(a[0], a[size - 1]);
		}
		else
		{
			swap(a[i], a[size - 1]);
		}
			
	}

}
