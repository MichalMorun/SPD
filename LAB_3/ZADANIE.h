#ifndef ZADANIE_H
#define ZADANIE_H


#define TEMP_P 500
#define TEMP_K 100
#define MI 0.95

class ZADANIE
{
public:
	int liczba_zadan;
	int liczba_maszyn;
	int Cmax;
	int tymczasowa_1, tymczasowa_2; // zmienne pomocnicze 
	float degree = TEMP_P; 
	float p; // prawdopodobieñstwo (wspó³czynnik)
	int **dane; // przechowywane dane
	int *kolejnosc_1, *kolejnosc_2; 
	int **TABLICA; 
	void zamien_kolejnosc(); // sortowanie
	int C_max(int *);
	void pobierz_dane(char *nazwa);
	void wyswietl();
	void zmien();
	void sprawdz();
	void porownaj();
	void wyzarzanie();

};

#endif 

