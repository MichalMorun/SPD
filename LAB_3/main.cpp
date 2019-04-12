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

using namespace std;


int main()
{
	srand(time(NULL));
	float czas;

	long long START = GetTickCount();

	ZADANIE N;
	N.wyzarzanie();

	long long KONIEC = GetTickCount();
	czas = (float)(KONIEC - START);
	cout << endl << "czas: " << czas/1000 << "s" << endl;
	//cout << endl << N.degree;
	return 0;
}