// konwerter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>


using namespace std;

/*
Funkcja wczytuje parametry wywolania programu, sa one podawane w linii polecen jako przelaczniki w dowolnej kolejnosci:
-i input		nazwa pliku wejsciowego
-o output		nazwa pliku wyjsciowego
-h				wyswietlenie pomocy

paramery funkcji:
ile				liczba podanych parametrow przy uruchomieniu programu
argumenty		tablica wskaznikow na lancuchy uzytych przy uruchomieniu programu
szinput			parametr wyjsciowy, do ktorego zostanie	zapisana odczytana nazwa pliku wejsciowego
szoutput		parametr wyjsciowy, do ktorego zostanie zapisana odczytana nazwa pliku wyjsciowego

wartosc zwracana
true			podano prawidlowe parametry	wywolania programu, zostaly odczytane i zapisane do szinput i szoutput
false			nie udalo sie odczytac wartosci parametrow wywolania programu; wartosci zmiennych szinput i szoutput moga nie zawierac istotnych wartosci

*/
bool odczyt(int ile, char ** argumenty, string &szinput, string &szoutput)
{
	const string etykieta_input("-i");
	const string etykieta_output("-o");
	const string etykieta_help("-h");
	const int flaga_input = 1;
	const int flaga_output = flaga_input << 1;
	const int flaga_help = 0;
	const int poprawny_wynik = flaga_input | flaga_output;
	int wynik = 0;
	for (int i = 0; i < ile - 1; i++)
	{
		string arg(argumenty[i]);
		if (arg == etykieta_input)
		{
			szinput = argumenty[i + 1];
			wynik |= flaga_input;
		}
		if (arg == etykieta_output)
		{
			szoutput = argumenty[i + 1];
			wynik |= flaga_output;
		}


	}
	if (wynik == poprawny_wynik)
		return true;
	else
		return false;

}
/*
Funkcja wypisujaca w konsoli pomoc dla uzytkowanika 
*/
void help()
{
	cout << "Witam w programie konwerujacym liczby naturalne w roznych systemach liczbowych" << endl << endl;
	cout << "Jak uruchomic program?" << endl;
	cout << "W wierszu polecen nalezy wpisac nazwe programu (konwerter.exe), nastepnie po przelaczniku -i nazwe pliku tekstowego z danymi wejsciowymi i po przelaczniku -o nazwe pliku tekstowego w ktorym maja zapisane zostac dane wejsciowe" << endl << endl;
	cout << "Jak maja wygladac dane wejsciowe?" << endl;
	cout << "a ( p -> q ), gdzie:" << endl;
	cout << "a - liczba do konwersji" << endl;
	cout << "p - system liczbowy (od 2 do 36) w ktorym zapisana jest liczba \"a\"" << endl;
	cout << "q - system docelowy" << endl << endl;
	cout << "UWAGA! Dla systemow powyzej 10, nalezy wpisywac wielkie litery" << endl;
}


/*
Funkcja konwertujaca liczbe w systemie dziesietnym na dowolny system z zakresu od 2 do 36

Parametry funkcji:
liczba			liczba naturalna w systemie dziesietnym ktora nalezy przekonwetowac
sys				system, na ktory nalezy przekonwetowac podana liczbe

Wartosc zwracana:
lancuch zawierajacy przekonwertowana liczbe
*/
string DecToAll(int liczba, int sys)
{
	if (liczba < 0)												//Jesli podana liczba jest mniejsza od zera, co jest niezgodne z zalozeniami algorytmu, zwracana jest stosowna informacja
		return "Bledne dane wejsciowe";
	string wynik;
	int reszta;
	
	while (liczba != 0)
	{
		reszta = liczba%sys;									//dzielenie liczby wejsciowej z reszta, przez podstawe sytemu docelowego
		if (reszta<10)											//jesli reszta jest mniejsza od 10
		{
			wynik += reszta + '0';								//zapisanie reszty jako cyfry w pierwszej wolnej komorce lancucha
		}
		else													//jesli reszta jest rowna 10 lub wiecej
		{
			wynik += reszta + 'A' - 10;							//zapisanie reszty jako litery w pierwszej wolnej komorce lancucha
		}
		liczba /= sys;											//dzielenie calkowite (bez reszty i miejsc po przecinku) liczby wejsciowej przez podstawe systemu docelowego
	}
	reverse(wynik.begin(), wynik.end());						//odwrocenie kolejnosci komorek w lancuchu
	return wynik;												//zwrocenie lancucha zawierajacego wynik konwersji
}

/*
Funkcja konwertujaca liczbe w dowolnym systemie z zakresu od 2 do 36, na liczbe w systemie dziesietnym

Parametry funkcji:
liczba			liczba naturalna w dowolnym systemie
sys				system, w ktorym podana jest liczba

Wartosc zwracana:
wynik			liczba naturalna w systemie dziesietnym
*/
int AllToDec(const string &liczba, int sys) 
{
	int potega = 0;
	int cyfra;
	int wynik = 0;
	for (int i = liczba.size() - 1; i >= 0; i--)				//petla przechodzaca po kazdym elemencie lancucha od tylu
	{

		if (liczba[i] >= '0' && liczba[i] <= '9')				//sprawdzanie czy dany element jest cyfra
		{
			if (liczba[i] - '0' < sys)							//sprawdzanie czy cyfra jest zgodna z podanym systemem
				cyfra = liczba[i] - '0';						//przypisanie cyfry do zmiennej liczbowej 
			else
			{
				return -1;										//jesli cyfra jest niezgodna z systemem, zwrocona zostaje wartosc oznaczajaca blad
			}
		}
		else if (liczba[i] >= 'A' && liczba[i] <= 'Z')			//sprawdzanie czy dany element jest litera
		{
			if (liczba[i] - 'A' + 10 < sys)						//sprawdzanie czy litera jest zgodna z podanym systemem
				cyfra = liczba[i] - 'A' + 10;					//przypisanie wartosci litery w danym systemie do zmiennej liczbowej
			else
			{
				return -1;										//jesli litera jest niezgodna z systemem, zwrocona zostaje wartosc oznaczajaca blad
			}
		}
		wynik += cyfra*pow(sys, potega);						//podniesienie systemu do odpowiedniej potegi, przemnozenie z cyfra i zsumowanie z poprzednimi wynikami
		potega++;												//podniesienie wartosci potegi
	}
	return wynik;												//zwrocenie liczby w systemie dziesietnym

}
/*
Funkcja zamieniajaca w lancuchu wszystkie znaki nie bedaca litera badz cyfra na znak bialy 

Parametry funkcji:
linia			lancuch na ktorym nalezy wykonac usuniecie znakow niebedacych alfanumerycznymi

Wartosc zwracana:
linia			lancuch bez znakow niebedacych alfanumerycznymi
*/
string uzs(string &linia)
{
	for (int i = 0; i < linia.size(); i++)						//petla przechodzaca po kazdej komorce lancucha 
	{
		if (!isalnum(linia[i]))									//sprawdzanie czy znak znajdujacy sie w komorce jest znakiem innym niz alfanumeryczny
			linia[i] = ' ';										//jesli powyzszy warunek jest prawdziwy, znak jest zamieniany na znak odstepu
	}
	return linia;												//zwrocenie lancucha bez znakow innych niz alfanumeryczne
}


int main(int argc, char* argv[])
{
	string wejscie, wyjscie;									//lancuchy przechowujace nazwy plikow wejsciowych i wyjsciowych
	
	if (odczyt(argc, argv, wejscie, wyjscie))					//sprawdzenie czy przy uruchamianiu programu zostaly podane wlasciwe parametry
	{
		ifstream plik;											//zmienna plikowa tylko do odczytu
		ofstream plik2;											//zmienna plikowa tylko do zapisu
		string linia;											//lancuch przechowujacy aktualnie wczytywana linie
		string liczba;											//lancuch przechowujacy liczbe ktora nalezy przekonwertowac  
		int syst;												//zmienna przechowujaca system w jakim zapisana jest podana liczba
		int sysdoc;												//zmienna przechowujaca system na ktory ma byc przekonwertowana liczba
		int temp;												//zmienna pomocnicza 
		plik.open(wejscie);										//otwarcie pliku z danymi wejsciowymi
		plik2.open(wyjscie);									//otwarcie pliku z danymi wyjsciowymi

		while (getline(plik, linia))							//wczytywanie linii tekstu dopoki nie zostanie napotkany koniec pliku z danymi wejsciowymi
		{
			if (linia.empty())									//sprawdzenie czy wczytana linia jest pusta
			{
				plik2 << endl;									//jesli powyzszy warunek jest prawdziwy, do pliku wyjsciowego rowniez wypisywana jest pusta linia

			}
			else												//jesli warunek jest nieprawdziwy, program wykonuje nastepujace polecenia
			{
				istringstream iss(uzs(linia));					//utworzenie lancucha przechowujacego wczytana wczesnieje linie, pozbawiona znakow niealfanumerycznych
				iss >> liczba >> syst >> sysdoc;				//wczytanie liczby, systemu w jakim sie znajduje i systemu docelowego do przeznaczonych dla nich lancuchow i zmiennych
				temp = AllToDec(liczba, syst);					//przekonwertowanie liczby na system dziesietny i zapisanie jej w zmiennej pomocniczej
				plik2 << DecToAll(temp, sysdoc) << endl;		//przekonwertowanie liczby z systemu dziesietnego na docelowy i wypisanie jej w pliku wyjsciowym
			}
		}
		plik.close();											//zamkniecie pliku wejsciowego
		plik2.close();											//zamkniecie pliku wyjsciowego
	}
	else														//instrukcje wykonujace sie jesli przy uruchomieniu programu zostaly podane nieprawidlowe dane
	{
		help();													//wypisanie w konsoli pomocy dla uzytkownika
	}
	return 0;
}

