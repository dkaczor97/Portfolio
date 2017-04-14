// Cykl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "Header.h"

#ifdef _WIN32
#  define _CRTDBG_MAP_ALLOC  
#  include <stdlib.h>
#  include <crtdbg.h>
#endif

using namespace std;

int main(int argc, char* argv[])
{
	{
		string wejscie, wyjscie;													//lancuchy przechowujace nazwy plikow wejsciowych i wyjsciowych
		if (odczyt(argc, argv, wejscie, wyjscie))									//jezeli program zostal uruchomiony poprawnie
		{
			wierzcholek *glowa = 0;													//glowa listy wierzcholkow
			stos *szczyt = 0;														//glowa stosu
			ifstream plik_wejsciowy;												//plik z danymi wejsciowymi
			ofstream plik_wyjsciowy;												//plik z danymi wyjsciowymi
			plik_wejsciowy.open(wejscie);											//otwarcie pliku wejsciowego
			plik_wyjsciowy.open(wyjscie);											//otwarcie pliku wyjsciowego
			string linia;															//lancuch przechowujacy aktualnie wczytana linie tekstu z pliku wejsciowego
			int wierzcholek_poczatkowy, wierzcholek_koncowy;						//zmienne przechowujace wierzcholek poczatkowy i koncowy aktualnie wczytywanej krawedzi
			bool wystepowanie_cyklu;												//przechowuje informacje czy cykl zostal znaleziony

			//tworzenie grafu
			while (getline(plik_wejsciowy, linia, ','))								//wczytuj kolejne linie tekstu az do napotkania przecinka lub konca pliku wejsciowego
			{
				istringstream iss(usun_strzalke(linia));							//przypisz do zmiennej iss linie zawierajaca same numery wierzcholka poczatkowego i koncowego
				if (iss >> wierzcholek_poczatkowy >> wierzcholek_koncowy)			//przypisanie wierzcholkow do osobnych zmiennych
					dodaj_krawedz(glowa, wierzcholek_poczatkowy, wierzcholek_koncowy);	//dodanie krawedzi do grafu
			}

			//szukanie cyklu
			auto p = glowa;															//zmienna pomocnicza
			while (p)																//petla przechodzaca przez cala liste wierzcholkow
			{
				zeruj_odwiedziny(glowa);											//oznaczenie wszystkich wierzcholkow jako nieodwiedzone

				if (szukaj_cyklu(p, p->numer, szczyt))								//jesli znaleziono cykl dla danego wierzcholka
				{
					wystepowanie_cyklu = true;										//zapisz informacje ze znaleziono cykl
					dodaj_na_stos(szczyt, p->numer);								//dodanie na stos ostatniego elementu cyklu
					wypisz_stos(szczyt, plik_wyjsciowy);							//wypisanie stosu do pliku wyjsciowego
					plik_wyjsciowy << endl;											//przejscie do nastepnej linii w pliku wyjsciowym
					usun_caly_stos(szczyt);											//usuniecie stosu i zwolnienie pamieci
				}
				p = p->pNext;														//przejscie do nastepnego wierzcholka
			}
			if (!wystepowanie_cyklu)												//jesli w calym grafie nie zostal znaleziony zaden cykl
				plik_wyjsciowy << "Brak cyklu w grafie";							//wypisanie stosownej informacji


			//zwalnianie pamieci
			p = glowa;																
			while (p)																//przejscie po calej liscie wierzcholkow
			{
				usun_liste_krawedzi(p->pKrawedzie);									//usuniecie podlisty aktualnego wierzcholka
				p = p->pNext;														//przejscie do nastepnego wierzcholka
			}
			usun_liste_wierzcholkow(glowa);											//usuniecie listy wierzcholkow
			plik_wejsciowy.close();													//zamkniecie pliku wejsciowego
			plik_wyjsciowy.close();													//zamkniecie pliku wyjsciowego


		}

		else																		//jesli nie udalo sie poprawnie uruchomic programu
		{
			help();																	//wypisanie menu pomocy
		}
	}
#ifdef _WIN32
	_CrtDumpMemoryLeaks();															//sprawdzenie wyciekow pamieci
#endif
	return 0;
}