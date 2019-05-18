#include "pch.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>
#include <chrono>
#include <string>
#include <ctime>
#include <Windows.h>

using namespace std;


class ZADANIE // pojedyncze zadanie
{
public:
	int R, P, Q, Nr_zad;
	ZADANIE();
	~ZADANIE();
};

//--------------------------------------------------------------------------------------

int SZUKAJ(vector<int> zbior, int szukane) // funkcja szukajaca liczby ze zbioru
{
	for (int i = 0; i < zbior.size(); i++)
	{
		if (zbior[i] == szukane)
		{
			return i;
		}

	}
	return -1;
}

ZADANIE::ZADANIE() // kostruktor
{
	R = P = Q = Nr_zad = 0;
}

ZADANIE::~ZADANIE() {} // destruktor

//--------------------------------------------------------------------------------------

ZADANIE MIN(vector<ZADANIE> dane, vector<int> Nn); // deklaracja funkcji szukajacej minimum
ZADANIE MAX(vector<ZADANIE> dane, vector<int> Ng); // deklaracja funkcji szukajacej maximum

//--------------------------------------------------------------------------------------

vector<int> SHRANGE(vector<ZADANIE> dane) // algorytm shrange
{

	vector<int> kolejnosc(0);
	vector<int> Ng(0); // Ng
	vector<int> Nn(dane.size()); // Nn
	ZADANIE x;
	int i = 0, t;

	for (int i = 0; i < Nn.size(); i++)
	{
		Nn[i] = i + 1;
	}

	t = MIN(dane, Nn).R;

	while (Ng.size() != 0 || Nn.size() != 0)
	{
		while (Nn.size() != 0 && MIN(dane, Nn).R <= t)
		{
			x = MIN(dane, Nn);
			Ng.push_back(x.Nr_zad);

			vector<int>::iterator Gotowe;

			Gotowe = find(Nn.begin(), Nn.end(), x.Nr_zad);
			Nn.erase(Gotowe);
		}

		if (Ng.size() == 0)
		{
			t = MIN(dane, Nn).R;
		}
		else
		{
			x = MAX(dane, Ng);

			vector<int>::iterator Gotowe;

			Gotowe = find(Ng.begin(), Ng.end(), x.Nr_zad);
			kolejnosc.resize(kolejnosc.size() + 1);
			kolejnosc[i] = x.Nr_zad;
			Ng.erase(Gotowe);
			i++;
			t = t + dane[x.Nr_zad - 1].P;
		}
	}
	return kolejnosc;
}

//--------------------------------------------------------------------------------------

int SHRANGE_z_przerwaniami(vector<ZADANIE> & dane) // algorytm shrange z przerwaniami
{
	vector<int> Ng(0); // Ng
	vector<int> Nn(dane.size()); // Nn
	ZADANIE x, y;
	int cmax = 0, t = 0;
	y.P = 0;
	y.Q = INT_MAX;


	for (int i = 0; i < Nn.size(); i++)
	{
		Nn[i] = i + 1;
	}

	while (Ng.size() != 0 || Nn.size() != 0)
	{
		while (Nn.size() != 0 && MIN(dane, Nn).R <= t)
		{
			x = MIN(dane, Nn);
			Ng.push_back(x.Nr_zad);

			vector<int>::iterator Gotowe;

			Gotowe = find(Nn.begin(), Nn.end(), x.Nr_zad);
			Nn.erase(Gotowe);
			if (dane[x.Nr_zad - 1].Q > dane[y.Nr_zad - 1].Q) // moment przerwania  (warunek przerwania)
			{
				dane[y.Nr_zad - 1].P = t - dane[x.Nr_zad - 1].R;
				t = dane[x.Nr_zad - 1].R;

				if (dane[y.Nr_zad - 1].P > 0)
				{
					Ng.push_back(y.Nr_zad);
				}

			}
		}

		if (Ng.size() == 0)
		{
			t = MIN(dane, Nn).R;
		}
		else
		{
			x = MAX(dane, Ng);
			vector<int>::iterator Gotowe;
			Gotowe = find(Ng.begin(), Ng.end(), x.Nr_zad);
			Ng.erase(Gotowe);
			y = x;
			t = t + dane[x.Nr_zad - 1].P;
			cmax = max(cmax, t + dane[x.Nr_zad - 1].Q);
		}
	}
	return cmax;
}

//--------------------------------------------------------------------------------------

ZADANIE MIN(vector<ZADANIE> dane, vector<int> Nn) // szukajaca minimum
{
	ZADANIE min;
	min.R = INT_MAX;

	for (int i = 0; i < dane.size(); i++)
	{
		if (SZUKAJ(Nn, dane[i].Nr_zad) >= 0)
		{
			if (dane[i].R < min.R)
			{
				min = dane[i];
			}
		}
	}
	return min;
}

//--------------------------------------------------------------------------------------

ZADANIE MAX(vector<ZADANIE> dane, vector<int> Ng) // szukajaca maximum
{
	ZADANIE max;
	max.Q = 0;

	for (int i = 0; i < dane.size(); i++)
	{
		if (SZUKAJ(Ng, dane[i].Nr_zad) >= 0)
		{
			if (dane[i].Q > max.Q)
			{
				max = dane[i];
			}
		}
	}
	return max;
}

//--------------------------------------------------------------------------------------

int CMAX(vector<int> kolejnosc, vector<ZADANIE> dane, int ilosc_zadan) // funkcja cmax
{
	vector<int> C(ilosc_zadan);
	vector<int> D(C.size() + 1);

	D[0] = 0;
	D[1] = max(dane[kolejnosc[0] - 1].R, 0 + 0);
	C[0] = D[1] + dane[kolejnosc[0] - 1].P;

	for (int i = 2; i <= ilosc_zadan; i++)
	{
		D[i] = max(dane[kolejnosc[i - 1] - 1].R, D[i - 1] + dane[kolejnosc[i - 2] - 1].P);
		C[i - 1] = D[i] + dane[kolejnosc[i - 1] - 1].P;
	}

	for (int i = 0; i < C.size(); i++)
	{
		C[i] = C[i] + dane[kolejnosc[i] - 1].Q;
	}

	vector<int>::iterator iter = max_element(begin(C), end(C));
	int index = distance(C.begin(), iter);

	return C[index];
}

//------------------------------------------------------------------------------------------

int main()
{
	//		Algorytm Schrage z przerwaniami różni się od algorytmu Schrage
	//		zasadniczo jednym krokiem. Za każdym razem gdy do zbioru zadań gotowych
	//		jest dodawane zadanie to sprawdzane jest czy ma większy czas dostarczania
	//		od zadania aktualnie przebywającego na maszynie. Jeżeli tak to wykonywanie zadania
	//		natychmiast jest przerywane, a pozostała część zadania tj. o czasie
	//		trwania t - tmp.r ponownie wkładana jest do zbioru zadań gotowych do realizacji.

	int wybor, ilosc_zadan, ilosc_kolumn;
	ifstream plik;

	cout << "Wybierz plik: ";
	cin >> wybor;
	system("cls");

	switch (wybor)
	{
	case 1:
	{
		plik.open("in50.txt");
		cout << "Liczba zadan: 50\n\n";
		break;
	}
	case 2:
	{
		plik.open("in100.txt");
		cout << "Liczba zadan: 100\n\n";
		break;
	}
	case 3:
	{
		plik.open("in200.txt");
		cout << "Liczba zadan: 200\n\n";
		break;
	}
	case 4:
	{
		plik.open("in500.txt");
		cout << "Liczba zadan: 500\n\n";
		break;
	}
	case 5:
	{
		plik.open("in600.txt");
		cout << "Liczba zadan: 600\n\n";
		break;
	}
	case 6:
	{
		plik.open("in800.txt");
		cout << "Liczba zadan: 800\n\n";
		break;
	}
	default:
		cout << "Nieznany wybor!\n\n";
		break;

	}


	plik >> ilosc_zadan;
	plik >> ilosc_kolumn;

	vector<ZADANIE> dane(ilosc_zadan);

	for (int i = 0; i < ilosc_zadan; i++)
	{
		{
			plik >> dane[i].R;
			plik >> dane[i].P;
			plik >> dane[i].Q;
			dane[i].Nr_zad = i + 1;
		}
	}

	long long START = GetTickCount();
	vector<int> Kolejnosc = SHRANGE(dane);
	long long STOP = GetTickCount();

	long long START1 = GetTickCount();
	int Cmax_zwykle = CMAX(Kolejnosc, dane, ilosc_zadan);
	long long STOP1 = GetTickCount();

	long long START2 = GetTickCount();
	int Cmax_z_podzialem = SHRANGE_z_przerwaniami(dane);
	long long STOP2 = GetTickCount();

	cout << "\t\tCmax\tCzas\n";
	cout << "______________________________\n";
	cout << "Schrange\t" << Cmax_zwykle << "\t" << (STOP - START) + (STOP1 - START1) << " ms" << endl << endl;
	cout << "Schrange Pmtn\t" << Cmax_z_podzialem << "\t" << (STOP2 - START) + (STOP2 - START2) << " ms" << endl << endl;
	

	return 0;


}


