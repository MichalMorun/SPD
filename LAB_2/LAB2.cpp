#include "pch.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<list>
#include<stdio.h>
#include<algorithm>
#include<cstdlib>
#include <string>
#include<Windows.h>
#include <time.h>

#define ILOSC_TA 120
#define MAX_LICZBA_MASZYN 22 // Maksymalna liczba maszyn w pojedynczej instancji powiekszona o 2 (w celach obliczeniowych)
#define MAX_LICZBA_ZADAN 502 // Maksymalna liczba zadan w pojedynczej instancji powiekszona o 2 (w celach obliczeniowych)

using namespace std;

vector<int> Tab_Cmax;
vector<string> Tab_instancji;
long long START[ILOSC_TA], STOP[ILOSC_TA];


int l_zadan;	// Globalnie przechowywana liczba zadan (definiujace aktualna liczbe wierszy tablicy w instancji)
int l_maszyn;	// Globalnie przechowywana liczba maszyn (definiujaca aktualna liczbe kolumn tablicy w instancji)
// Wartosci te sa nadawane podczas wczytywania kolejnych instancji z pliku wejsciowego

struct ZADANIE
{
	int nr_zadania;
	int czas;
	int laczny_czas;
};

struct AllData
{
	int **Tab_zadan;
	int liczba_zadan; //liczba zadan
	int liczba_maszyn; //liczba maszyn
	list<ZADANIE> Lista_zadan;  //lista zadan posiada nr zadania i calkowity czas potrzebny do spedzenia na maszynach
	vector<int> Tab_czasow; // tablica uszeregowan czasow zadan
};


int Wyczysc(int **Tab, int x, int y);
void Zamien(int &x, int &y);
void Pobierz_dane(AllData *D, fstream & pFile);
void NEH_z_akceleracja(AllData *N);
void Wyswietl();
bool Porownaj(const ZADANIE& _jd1, const ZADANIE& _jd2);

int NEH(ZADANIE tmp[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], ZADANIE dane[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN]);
int licz_Cmax(ZADANIE tab[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], int l_zadan, int l_maszyn);
void Wstaw_przed(ZADANIE ZAD[1][MAX_LICZBA_MASZYN], ZADANIE tab[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], int l_zadan, int l_maszyn, int nr_zadania);
void ZadanieKopia(ZADANIE tab[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], ZADANIE ZAD[1][MAX_LICZBA_MASZYN], int l_maszyn, int numer);
void KolumnaKopia(ZADANIE tab[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], ZADANIE* Kolumna, int l_zadan, int numer);
bool Porownaj2(ZADANIE pierwsze, ZADANIE drugie);

// Globalne tablice statyczne do przechowywania zadan
ZADANIE ZAD[1][MAX_LICZBA_MASZYN];				// Tablica jednowymiarowa na pojedyncze zadanie (pojedynczy wiersz)
ZADANIE* Kolumna;								// Wskaznik na pojedyncza kolumne
ZADANIE dane[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN];
ZADANIE tmp[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN];

//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	AllData *ins = new AllData[ILOSC_TA];
	fstream Plik_in, Plik_out;
	int ostateczny = 0;
	int licznik = 0;

	Plik_in.open("bench_fs.txt", ios::in);
	for (int instance = 0; instance < ILOSC_TA; instance++) // for dla kolejnych instancji
	{
		Pobierz_dane(ins, Plik_in); //wczytaj dane z pliku
		ins++;
	}
	ins = ins - ILOSC_TA;

	for (int instance = 0; instance < ILOSC_TA; instance++)
	{
		START[instance] = GetTickCount();
		NEH_z_akceleracja(ins); //uszereguj je NEH'em
		STOP[instance] = GetTickCount();
		ins++;
	}
	Plik_in.close();
	Plik_in.open("bench_fs.txt", ios::in);
	for (int z = 0; z < 120; z++) // Ustawiona na stałe liczba instancji w pliku in. Ta petla wykonuje sie dla kazdej instancji
	{
		string nazwa_instancji;	// Zmienna przechowujaca nazwe instancji
		int liczba_zadan = 0;	// Zmienna przechowujaca wczytana liczbe zadan
		int liczba_maszyn = 0;	// Zmienna przechowujaca wczytana liczbe maszyn
		int koncowy_Cmax = 0;	// Koncowa wartosc Cmax
		int minimum = 0;		// Minimalny z Cmaxow
		int Najlepsza_opcja = 0;// Najlepsze miejsce do wstawienia nowego zadania

		// Wczytanie nazwy instancji. Wczytanie informacji na temat instancji i przekazanie ich do zmiennych globalnych
		Plik_in >> nazwa_instancji >> liczba_zadan >> liczba_maszyn;
		l_zadan = liczba_zadan;
		l_maszyn = liczba_maszyn;
		// Wczytanie zadan z instancji
		for (int i = 0; i < l_zadan; i++)
		{
			for (int j = 0; j < l_maszyn; j++)
			{
				// Ignorowanie numeru zadania - w pliku in odpowiada wykonywaniu petli i
				int tmp;
				Plik_in >> tmp;
				// Nadanie numeru zadania zgodnie z aktualnym wskazaniem petli i
				dane[i][j].nr_zadania = i + 1;
				// Wczytanie reszty danych
				Plik_in >> dane[i][j].czas;
			}
		}
		licznik++; // Wczytano instancje

		// Wyliczenie wartosci pierwszej kolumny - na sume czasow wykonywania poszczegolnych zadan na wszystkich maszynach
		Kolumna = new ZADANIE[l_zadan];
		for (int i = 0; i < l_zadan; i++)
		{
			// Zerowanie
			dane[i][0].laczny_czas = 0;
			// Liczenie lacznego czasu
			for (int j = 0; j < l_maszyn; j++)
			{
				dane[i][0].laczny_czas = dane[i][0].laczny_czas + dane[i][j].czas;
			}
		}
		// Kopiowanie kolumny i sortowanie zadan od tego o najwiekszej sumie czasow wykonywania
		KolumnaKopia(dane, Kolumna, l_zadan, 0);
		stable_sort(Kolumna, Kolumna + l_zadan, Porownaj2); // Wykorzystanie stable_sort dla optymalnego czasu sortowania
		// Posortowanie umozliwia odczytanie kolejnosci zadan (Kolumna zawiera numery zadan w odpowiedniej kolejnosci)
		// Wstawianie zadan do tab we wczesniej ustalonej kolejnosci
		for (int k = 0; k < l_maszyn; k++)
		{
			tmp[0][k].czas = dane[Kolumna[0].nr_zadania - 1][k].czas;
			tmp[0][k].nr_zadania = dane[Kolumna[0].nr_zadania - 1][k].nr_zadania;
		}
	
	
		long int poczatek_m = GetTickCount();
		koncowy_Cmax = NEH(tmp, dane);
		long int koniec_m = GetTickCount();

		ostateczny += koniec_m - poczatek_m;
		cout << Tab_instancji[licznik] << ": " << "Cmax: " << Tab_Cmax[licznik] << " s" << " = " << Tab_Cmax[licznik] / 60 << " min" << "    Czas operacji: " << koniec_m - poczatek_m << " ms" << endl;

	} 
	cout << endl << endl << endl;
	cout << "************************************************************************************************" << endl;
	cout << "************************************************************************************************" << endl;
	cout << endl << endl << endl;
	Wyswietl();
	Plik_in.close();

}




void Pobierz_dane(AllData *D, fstream & pFile_In)
{
	int czas_zadania;//suma czasow jednego zadania na wszystkich maszynach
	int czas; //czas zadania na maszynie
	int pomocnicza; //pomijana
	char nazwa_instancji[6];//nazwa instancji w pliku 
	ZADANIE ZADANIE_O;

	pFile_In >> nazwa_instancji;
	pFile_In >> D->liczba_zadan;
	pFile_In >> D->liczba_maszyn;

	Tab_instancji.push_back(nazwa_instancji);


	D->Tab_zadan = new int *[D->liczba_maszyn]; // i-zadan na j-maszynach
	for (int j = 0; j < D->liczba_maszyn; j++)
	{
		D->Tab_zadan[j] = new int[D->liczba_zadan];
	}

	for (int i = 0; i < D->liczba_zadan; i++) // Wczytanie zadan z instancji
	{
		czas_zadania = 0;
		for (int j = 0; j < D->liczba_maszyn; j++)
		{
			pFile_In >> pomocnicza; // Ignorowanie numeru zadania
			pFile_In >> czas;
			czas_zadania = czas_zadania + czas;
			D->Tab_zadan[j][i] = czas;
		}
		ZADANIE_O.nr_zadania = i + 1;
		ZADANIE_O.czas = czas_zadania;
		D->Lista_zadan.push_back(ZADANIE_O);
	}

}


void NEH_z_akceleracja(AllData *N)
{
	int max, min;
	int _dict = 0;
	int cmax;
	int nowe_zadanie;
	int aktualne_zadanie;
	int licznik = 0;
	vector<int>max_vect;
	N->Tab_czasow.clear();
	N->Lista_zadan.sort(Porownaj); //mam uszeregowane zadania ze zgledu na czas
	N->Tab_czasow.push_back(N->Lista_zadan.front().nr_zadania);// dodaje do tablicy 1 element posortowanej listy zadan 
	N->Lista_zadan.pop_front(); // sciagam ten element z listy, na liscie o jedno zadanie mniej


	int **E = new int*[N->liczba_maszyn + 1]; //tworze macierz E
	for (int i = 0; i < N->liczba_maszyn + 1; i++)
	{
		E[i] = new int[N->liczba_zadan];
	}

	int **Q = new int*[N->liczba_maszyn + 1]; //tworze macierz Q
	for (int i = 0; i < N->liczba_maszyn + 1; i++)
	{
		Q[i] = new int[N->liczba_zadan];
	}
	int **F = new int*[N->liczba_maszyn + 1]; //tworze macierz F
	for (int i = 0; i < N->liczba_maszyn + 1; i++)
	{
		F[i] = new int[N->liczba_zadan];
	}

	int **F_Q = new int*[N->liczba_maszyn + 1]; //tworze macierz F+Q
	for (int i = 0; i < N->liczba_maszyn + 1; i++)
	{
		F_Q[i] = new int[N->liczba_zadan];
	}

	Wyczysc(E, N->liczba_maszyn + 1, N->liczba_zadan); //
	Wyczysc(Q, N->liczba_maszyn + 1, N->liczba_zadan); // wypelniam je zerami
	Wyczysc(F, N->liczba_maszyn + 1, N->liczba_zadan); //
	Wyczysc(F_Q, N->liczba_maszyn + 1, N->liczba_zadan); //


	// poczatek petli dla kazdego zadania
	while (N->Lista_zadan.size() != 0)
	{
		max_vect.clear();
		for (int i = 0; i < (int)N->Tab_czasow.size(); i++) ////////////////////////////////////////////////////////////////////////////////// macierz E
		{
			for (int j = 0; j < N->liczba_maszyn; j++)// w pierwszej kolumnie
			{

				E[j + 1][i + 1] = max(E[j][i + 1], E[j + 1][i]) + N->Tab_zadan[j][N->Tab_czasow[i] - 1];
			}

		}
		for (int i = (int)N->Tab_czasow.size() - 1; i >= 0; i--) ////////////////////////////////////////////////////////////////////////////////// macierz Q
		{
			for (int j = N->liczba_maszyn - 1; j >= 0; j--)// w pierwszej kolumnie
			{

				Q[j][i] = max(Q[j + 1][i], Q[j][i + 1]) + N->Tab_zadan[j][N->Tab_czasow[i] - 1];
			}

		}

		nowe_zadanie = N->Lista_zadan.front().nr_zadania; // sciagam nowe zadanie
		N->Lista_zadan.pop_front();

		for (int i = 0; i < (int)N->Tab_czasow.size() + 1; i++) ////////////////////////////////////////////////////////////////////////////////// macierz F
		{
			for (int j = 0; j < N->liczba_maszyn; j++)// w pierwszej kolumnie
			{

				F[j + 1][i] = max(E[j + 1][i], F[j][i]) + N->Tab_zadan[j][nowe_zadanie - 1];
			}

		}
		for (int i = 0; i < (int)N->Tab_czasow.size() + 1; i++) ////////////////////////////////////////////////////////////////////////////////// macierz F_Q
		{
			for (int j = 0; j < N->liczba_maszyn; j++)// w pierwszej kolumnie
			{

				F_Q[j][i] = Q[j][i] + F[j + 1][i];
			}

		}

		for (int i = 0; i < (int)N->Tab_czasow.size() + 1; i++) // znajduje maxa poszczegolnych kolumn w macierzy F+Q
		{
			max = 0;
			for (int j = 0; j < N->liczba_maszyn; j++)
			{
				if (F_Q[j][i] > max)
				{
					max = F_Q[j][i];
				}
			}
			max_vect.push_back(max);
		}

		min = 999999;
		for (int i = 0; i < (int)max_vect.size(); i++)
		{
			if (max_vect[i] < min) {
				min = max_vect[i];
				_dict = i;
			}
		}
		cmax = min;

		N->Tab_czasow.push_back(0);
		for (int i = _dict; i < (int)N->Tab_czasow.size(); i++)
		{
			Zamien(N->Tab_czasow[i], aktualne_zadanie);
		}
		N->Tab_czasow[_dict] = nowe_zadanie;
	}
	Tab_Cmax.push_back(cmax);

}

void Zamien(int &x, int &y)
{
	int temp;
	temp = x;
	x = y;
	y = temp;
}

int Wyczysc(int **Tab, int x, int y)
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			Tab[i][j] = 0;
		}
	}

	return 0;
}

void Wyswietl()
{

	for (int i = 0; i < ILOSC_TA; i++)
	{
		cout << Tab_instancji[i] << ": " << "Cmax: " << Tab_Cmax[i] << " s" << " = " << Tab_Cmax[i] / 60 << " min" << "    Czas operacji: " << (double)(STOP[i] - START[i]) << " ms" << endl;
	}
}

bool Porownaj(const ZADANIE& _jd1, const ZADANIE& _jd2)
{
	if (_jd1.czas > _jd2.czas)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Porownaj2(ZADANIE pierwsze, ZADANIE drugie)
{
	if (pierwsze.laczny_czas > drugie.laczny_czas)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------



int NEH(ZADANIE tmp[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], ZADANIE dane[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN])
{
	int minimum = 0;
	int Najlepsza_opcja = 0;

	//N->Tab_czasow.clear();
	//N->Lista_zadan.sort(Porownaj); //mam uszeregowane zadania ze zgledu na czas

	for (int y = 1; y < l_zadan; y++)
	{
		// Pobranie zadania z tablicy
		ZadanieKopia(dane, ZAD, l_maszyn, (Kolumna[y].nr_zadania - 1));
		// Wstawianie skopiowanego zadania na koniec
		Wstaw_przed(ZAD, tmp, y, l_maszyn, y);
		int Cmax = licz_Cmax(tmp, y + 1, l_maszyn);
		minimum = Cmax;			// Minimalny Cmax
		Najlepsza_opcja = y;    // Po wstawieniu przed tym zadaniem uzyskamy minimalnego Cmaxa

		// Przesuwanie zadania i liczenie aktualnego Cmaxa
		for (int k = y; k > 0; k--)
		{
			for (int j = 0; j < l_maszyn; j++)
			{
				swap(tmp[k][j].czas, tmp[k - 1][j].czas);
				swap(tmp[k][j].nr_zadania, tmp[k - 1][j].nr_zadania);
			}
			// Wyliczanie Cmax odbywa sie po kazdej zmianie w tablicy tmp
			Cmax = licz_Cmax(tmp, y + 1, l_maszyn);
			// Sprawdzenie nowego Cmaxa
			if (Cmax <= minimum)
			{
				minimum = Cmax;
				Najlepsza_opcja = k - 1;
			}
		}
		// Przesuniecie zadania z poczatku tablicy na wyznaczona optymalna pozycje
		for (int i = 0; i < Najlepsza_opcja; i++)
		{
			// Zrobienie miejsca na zadanie
			for (int j = 0; j < l_maszyn; j++)
			{
				tmp[i][j].czas = tmp[i + 1][j].czas;
				tmp[i][j].nr_zadania = tmp[i + 1][j].nr_zadania;
			}
		}
		// Wstawienie zadania w powstala luke (lepsze czasowo niz swapowanie zadania do pozycji optymalnej)
		for (int j = 0; j < l_maszyn; j++)
		{
			tmp[Najlepsza_opcja][j].czas = ZAD[0][j].czas;
			tmp[Najlepsza_opcja][j].nr_zadania = ZAD[0][j].nr_zadania;
		}
	}
	return minimum;
}

int licz_Cmax(ZADANIE tab[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], int l_zadan, int l_maszyn)
{
	ZADANIE tab_Cmax[MAX_LICZBA_ZADAN + 1][MAX_LICZBA_MASZYN + 1];
	// Zerowanie tablicy
	for (int i = 0; i <= l_zadan; ++i)
	{
		for (int j = 0; j <= l_maszyn; ++j)
		{
			tab_Cmax[i][j].czas = 0;
		}
	}
	// Obliczanie Cmax
	for (int i = 1; i <= l_zadan; ++i)
	{
		for (int j = 1; j <= l_maszyn; ++j)
		{
			tab_Cmax[i][j].czas = max(tab_Cmax[i - 1][j].czas, tab_Cmax[i][j - 1].czas) + tab[i - 1][j - 1].czas;
		}
	}
	return tab_Cmax[l_zadan][l_maszyn].czas;
}

void Wstaw_przed(ZADANIE ZAD[1][MAX_LICZBA_MASZYN], ZADANIE tab[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], int l_zadan, int l_maszyn, int nr_zadania)
{
	// Tworzenie miejsca w tablicy na nowe zadanie przez przesuwanie dalszych w prawo (dzieki tablicy statycznej o maksymalnym rozmiarze
	// nie trzeba sie martwic o rozszerzanie tablicy)
	for (int i = l_zadan; i > nr_zadania; i--)
	{
		for (int j = 0; j < l_maszyn; j++)
		{
			tab[i][j].czas = tab[i - 1][j].czas;
			tab[i][j].nr_zadania = tab[i - 1][j].nr_zadania;
		}
	}
	// Wstawianie
	for (int i = 0; i < l_maszyn; i++)
	{
		tab[nr_zadania][i].czas = ZAD[0][i].czas;
		tab[nr_zadania][i].nr_zadania = ZAD[0][i].nr_zadania;
	}
}


void ZadanieKopia(ZADANIE tab[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], ZADANIE ZAD[1][MAX_LICZBA_MASZYN], int l_maszyn, int numer)
{
	for (int j = 0; j < l_maszyn; j++)
	{
		ZAD[0][j].nr_zadania = tab[numer][j].nr_zadania;
		ZAD[0][j].czas = tab[numer][j].czas;
	}
}


// Tworzenie kopii zadania z kolumny
void KolumnaKopia(ZADANIE tab[MAX_LICZBA_ZADAN][MAX_LICZBA_MASZYN], ZADANIE* Kolumna, int l_zadan, int numer)
{
	for (int j = 0; j < l_zadan; j++)
	{
		Kolumna[j].nr_zadania = tab[j][numer].nr_zadania;
		Kolumna[j].czas = tab[j][numer].czas;
		Kolumna[j].laczny_czas = tab[j][numer].laczny_czas;
	}
}
