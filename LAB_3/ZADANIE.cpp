#include "pch.h"
#include "ZADANIE.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <math.h> 
#include <algorithm>
#include <time.h>
#include <Windows.h>
#include <string>


using namespace std;

char nazwa[10] = "data.txt"; // nazwa pliku

void ZADANIE::pobierz_dane(char *nazwa) // pobiera dane 
{
	fstream plik;
	char temp;
	plik.open(nazwa, ios::in);


	plik >> liczba_zadan;
	plik >> liczba_maszyn;

	dane = new int *[liczba_zadan]; // tworzenie tablicy na dane
	for (int i = 0; i < liczba_zadan; i++)
	{
		dane[i] = new int[liczba_maszyn + 1];
	}

	for (int i = 0; i < liczba_zadan; i++) // zerowanie tablicy z danymi
	{
		dane[i][liczba_maszyn] = 0;
	}

	for (int i = 0; i < liczba_zadan; i++) // wpisywanie danych do tablicy
	{
		for (int j = 0; j < liczba_maszyn; j++)
		{
			plik >> dane[i][j];
			dane[i][liczba_maszyn] += dane[i][j];
		}
	}


	kolejnosc_1 = new int[liczba_zadan]; // jedna kolejnoœæ
	kolejnosc_2 = new int[liczba_zadan]; // druga kolejnoœæ
	for (int i = 0; i < liczba_zadan; i++) // tworzenie jednego wymiaru tablicy
	{
		kolejnosc_1[i] = i + 1;
		kolejnosc_2[i] = i + 1;
	}
	TABLICA = new int *[liczba_maszyn + 1];

	for (int i = 0; i < liczba_maszyn + 1; i++)// tworzenie drugiego wymiaru tablicy
	{
		TABLICA[i] = new int[liczba_zadan + 1];
	}


	for (int i = 0; i < liczba_maszyn + 1; i++) // zerowanie tablicy
	{
		for (int j = 0; j < liczba_zadan + 1; j++)
		{
			TABLICA[i][j] = 0;
		}

	}

	plik.close();
}


void ZADANIE::wyswietl() // wyswietlanie kolejnosci 
{
	for (int i = 0; i < liczba_zadan; i++)
	{
		cout << kolejnosc_2[i] << ' ';
	}
	cout << " P = " << p << endl;
}


void ZADANIE::zmien() 
{
	degree = degree * MI; // wspó³czynnik wych³adzania
}




void ZADANIE::zamien_kolejnosc() // losowanie 2 kolejnoœci, potem sprawdza która lepsza
{
	tymczasowa_1 = (rand() % liczba_zadan) + 1;
	tymczasowa_2 = (rand() % liczba_zadan) + 1;
	swap(kolejnosc_1[tymczasowa_1 - 1], kolejnosc_1[tymczasowa_2 - 1]);
	cout << tymczasowa_1 << ' ' << tymczasowa_2 << endl;
}


int ZADANIE::C_max(int *kolejnosc_1) // liczenie Cmaxa na podstawie jednej tablicy
{
	for (int i = 1; i < liczba_maszyn + 1; i++)
	{
		for (int j = 1; j < liczba_zadan + 1; j++)
		{
			TABLICA[i][j] = max(TABLICA[i - 1][j], TABLICA[i][j - 1]) + dane[kolejnosc_1[j - 1] - 1][i - 1];
		}
	}
	Cmax = TABLICA[liczba_maszyn][liczba_zadan];
	return Cmax;
}


void ZADANIE::sprawdz() // liczenie prawdopodobieñstwa wykonania lub nie wykonania ruchu
{
	if (C_max(kolejnosc_1) < C_max(kolejnosc_2)) // rozwi¹zanie lepsze
	{
		p = 1;
	}
		
	else
	{
		p = exp((C_max(kolejnosc_2) - C_max(kolejnosc_1)) / degree); // rozwi¹zanie gorsze
	}
		

}


void ZADANIE::porownaj() // ustala kolejnoœæ, jesli nowa jest lepsza od starej, to wtedy ustawia j¹ za podstawow¹
{
	float h = (float)rand() / RAND_MAX; // liczba losowa
	if (p >= h)
	{
		for (int i = 0; i < liczba_zadan; i++)
		{
			kolejnosc_2[i] = kolejnosc_1[i]; 
		}
			
	}
	else
	{
		for (int i = 0; i < liczba_zadan; i++)
		{
			kolejnosc_1[i] = kolejnosc_2[i];
		}
			
	}
}


void ZADANIE::wyzarzanie() // algorytm wYrza¿ania
{
	pobierz_dane(nazwa);
	int k = 0;
	while (degree > TEMP_K)
	{
		zamien_kolejnosc();
		sprawdz();
		porownaj();
		zmien();
		wyswietl();
		cout << "Cmax = " << C_max(kolejnosc_2) << endl;
		cout << "-----------------------" << endl;
		k++;
	}
	cout << "liczba iteracji = " << k;

	delete dane; // czyszczenie
	delete kolejnosc_1;
	delete TABLICA;
	delete kolejnosc_2;
}