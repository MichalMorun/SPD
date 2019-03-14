#include "pch.h"
#include <iostream>
#include <time.h>
#include "Algorytm_johnsona.h"
#include <algorithm>

using namespace std;




int main()
{
	setlocale(LC_ALL, "");
	srand(time(NULL)); // ustawienie początkowe generatora liczb losowych



	Pobierz_dane();
	Utworz_dane();
	Algorytm_Johnsona();
	Wyswietl();
	Przeglad_zupelny();


	return 0;
}

