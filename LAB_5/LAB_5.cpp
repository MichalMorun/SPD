#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
#include <Windows.h>

using namespace std;

class ZADANIE
{
public:
	int R, P, Q, Nr_zad;
	ZADANIE();
	~ZADANIE();
};

ZADANIE::ZADANIE() // kostruktor
{
	R = P = Q = Nr_zad = -1;
}

ZADANIE::~ZADANIE() {} // destruktor


void WczytajDane(vector<ZADANIE>& Zadania, string nazwa_pliku)
{
	ifstream plik(nazwa_pliku, ios::in);
	int ilosc_zadan;


	plik >> ilosc_zadan;
	for (int i = 0; i < ilosc_zadan; i++)
	{
		Zadania.push_back(ZADANIE());
		Zadania[i].Nr_zad = i;
		for (int j = 0; j < 3; j++)
		{
			switch (j % 3)
			{
			case 0:
				plik >> Zadania[i].R;
				break;
			case 1:
				plik >> Zadania[i].P;
				break;
			case 2:
				plik >> Zadania[i].Q;
				break;

			default:
				break;
			}
		}

	}
}


int Cmax(const vector<ZADANIE>& Zadania)
{
	int t = 0, cmax = 0;

	for (int i = 0; i < Zadania.size(); i++)
	{
		t = max(t, Zadania[i].R) + Zadania[i].P;
		cmax = max(cmax, t + Zadania[i].Q);
	}
	return cmax;
}

int Cmax_q(const vector<ZADANIE> & Zadania, const int size)
{
	int t = 0, cmax = 0;

	for (int i = 0; i <= size; i++)
	{
		t = max(t, Zadania[i].R) + Zadania[i].P;
		cmax = max(cmax, t);
	}
	return cmax;
}

int Cmax_rq(const vector<ZADANIE> & Zadania, int poczatek, const int koniec)
{
	int cmax = 0;

	for (int i = poczatek; i <= koniec; i++)
	{
		cmax += Zadania[i].P;
	}
	return cmax;
}


int sumaP(const vector<ZADANIE> Wekt, const int poczatek, const int koniec)
{
	int suma = 0;
	for (int i = poczatek; i <= koniec; i++)
	{
		suma += Wekt[i].P;
	}
	return suma;
}


int h_od_K(int a, int b, int c)
{
	return a + b + c;
}

void sortowanie_N(vector<ZADANIE> & Wynik, vector<ZADANIE> & Dane_z_pliku)
{
	ZADANIE tmp;
	int iterator = -1;

	while (Dane_z_pliku.size() != 0)
	{
		tmp.R = 0;
		for (int i = 0; i < Dane_z_pliku.size(); i++)
		{
			if (Dane_z_pliku[i].R >= tmp.R)
			{
				tmp = Dane_z_pliku[i];
				iterator = i;
			}
		}
		Dane_z_pliku[iterator] = Dane_z_pliku[Dane_z_pliku.size() - 1];
		Dane_z_pliku.pop_back();
		Wynik.push_back(tmp);
	}
}

void sortowanie_G(vector<ZADANIE> & G)
{
	ZADANIE max;
	max.Q = -1;
	int iterator = -1;


	for (int i = 0; i < G.size(); i++)
	{
		if (G[i].Q >= max.Q)
		{
			max = G[i];
			iterator = i;
		}
	}
	G[iterator] = G[G.size() - 1];
	G[G.size() - 1] = max;


}

vector<ZADANIE> Shrage(vector<ZADANIE> TMP)
{
	vector<ZADANIE> N;
	vector<ZADANIE> G;

	sortowanie_N(N, TMP);

	vector<ZADANIE> pi;
	ZADANIE tmp;
	int t = 0, Cmax = 0;

	while (G.size() != 0 || N.size() != 0)
	{
		while (N.size() != 0 && N[N.size() - 1].R <= t)
		{
			G.push_back(N[N.size() - 1]);
			N.pop_back();
		}

		if (G.size() == 0)
		{
			t = N[N.size() - 1].R;

		}
		else
		{
			sortowanie_G(G);
			pi.push_back(G[G.size() - 1]);
			G.pop_back();

			t += pi[pi.size() - 1].P;
			Cmax = max(Cmax, t + pi[pi.size() - 1].Q);
		}

	}
	return pi;
}

int Shrage_z_przerwaniami(vector<ZADANIE> TMP)
{
	vector<ZADANIE> G;
	vector<ZADANIE> N;
	sortowanie_N(N, TMP);

	vector<ZADANIE> pi;
	ZADANIE tmp, l;
	l.Q = INT_MAX;
	int t = 0, Cmax = 0;

	while (G.size() != 0 || N.size() != 0)
	{
		while (N.size() != 0 && N[N.size() - 1].R <= t)
		{
			G.push_back(N[N.size() - 1]);
			N.pop_back();

			if (G[G.size() - 1].Q > l.Q)
			{
				l.P = t - G[G.size() - 1].R;
				t = G[G.size() - 1].R;
				if (l.P > 0)
				{
					G.push_back(l);
					sortowanie_G(G);
				}
			}
		}

		if (G.size() == 0)
		{
			t = N[N.size() - 1].R;
		}
		else
		{
			sortowanie_G(G);
			l = G[G.size() - 1];
			pi.push_back(G[G.size() - 1]);
			G.pop_back();
			t += pi[pi.size() - 1].P;
			Cmax = max(Cmax, t + pi[pi.size() - 1].Q);
		}

	}
	return Cmax;
}


int Wyznacz_A(const vector<ZADANIE> Wekt, int const b) // b - do którego indeksu ma szukać
{
	int cmax = Cmax(Wekt);

	for (int a = 0; a <= b; a++)
	{
		if (cmax == (Wekt[a].R + Cmax_rq(Wekt, a, b) + Wekt[b].Q))
		{
			return a;
		}
	}
	return -1;
}

int Wyznacz_B(const vector<ZADANIE> Wekt)
{
	for (int i = Wekt.size(); i > 0; --i)
	{
		if (Cmax(Wekt) == (Cmax_q(Wekt, i) + Wekt[i].Q))
		{
			return i;
		}
	}
	return -1;
}

int Wyznacz_C(const vector<ZADANIE> Wekt, const int a, const int b) 
{
	int c = -1;

	for (int i = b; i >= a; i--)
	{
		if (Wekt[i].Q < Wekt[b].Q)
		{
			c = i;
			break;
		}
	}
	return c;
}


int MIN_R(const vector<ZADANIE> Wekt, const int poczatek, const int koniec)
{
	int min = INT_MAX;
	for (int i = poczatek; i <= koniec; i++)
	{
		if (Wekt[i].R < min)
		{
			min = Wekt[i].R;
		}
	}
	return min;
}

int MIN_Q(const vector<ZADANIE> Wekt, const int poczatek, const int koniec)
{
	int min = INT_MAX;
	for (int i = poczatek; i <= koniec; i++)
	{
		if (Wekt[i].Q < min)
		{
			min = Wekt[i].Q;
		}
	}
	return min;
}

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

vector<ZADANIE> CARLIER(vector<ZADANIE> & N)
{
	vector<ZADANIE> oryg_N = N;
	int LB = -1;
	int UB = INT_MAX;
	int U = Cmax(Shrage(N)); // 1 krok, obliczamy cmax
	int A, B, C = 0;

	if (U < UB) // 2 krok, górna granica
	{
		UB = U;
		N = Shrage(N);
	}

	B = Wyznacz_B(N); // 3 krok, wyznaczanie A, B i C
	A = Wyznacz_A(N, B);
	C = Wyznacz_C(N, A, B);

	if (C == 0) // 4 krok, warunek z c 
	{
		return N;
	}

	int Kp = C + 1, Kk = B; // 5 krok, od poczatku do konca
	int KR = MIN_R(N, Kp, Kk); // r od k
	int KQ = MIN_Q(N, Kp, Kk); // q od k
	int KP = sumaP(N, Kp, Kk); // p od k
	
	int KRC = MIN_R(N, C, Kk); // 6 krok, od zmodyfikowanego zadania do konca
	int KQC = MIN_Q(N, C, Kk);
	int KPC = sumaP(N, C, Kk);


	int daw_R = N[C].R; // START - modyfikacja R
	N[C].R = max(N[C].R, KR + KP);

	LB = Shrage_z_przerwaniami(N); // 7 krok, ustalenie dolnej granicy

	if (LB < UB) // 8 krok, czy są bardziej optymalne rozwiązanie
	{
		CARLIER(N);
		return N;
	}

	N[C].R = daw_R; 


	int daw_Q = N[C].Q; // START - modyfikacja Q
	N[C].Q = max(N[C].Q, KQ + KP);

	LB = Shrage_z_przerwaniami(N); // 9 krok, ustalenie dolnej granicy

	if (LB < UB)
	{
		CARLIER(N);
	}
	N[C].Q = daw_Q;
	return N;
}

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

int main()
{
	vector<ZADANIE> temp;
	int wybor;

	cout << "GOTOWY" << endl;
	do
	{
		temp.clear();
		cin >> wybor;
		switch (wybor)
		{
		case 1:
		{
			WczytajDane(temp, "data0.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 2:
		{
			WczytajDane(temp, "data1.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 3:
		{
			WczytajDane(temp, "data2.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 4:
		{
			WczytajDane(temp, "data3.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 5:
		{
			WczytajDane(temp, "data4.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 6:
		{
			WczytajDane(temp, "data5.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 7:
		{
			WczytajDane(temp, "data6.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 8:
		{
			WczytajDane(temp, "data7.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 9:
		{
			WczytajDane(temp, "data8.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 10:
		{
			WczytajDane(temp, "in50.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 11:
		{
			WczytajDane(temp, "in100.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 12:
		{
			WczytajDane(temp, "in200.txt");
			cout << Cmax(CARLIER(temp)) << endl;
			cout << "----------------" << endl << endl;
			break;
		}
		case 13:
			break;
		default:
			cout << "Nieznany wybór\n" << endl;
		}
	} while (wybor != 13);


	return 0;
}
