#include "stdafx.h"
#include "Header.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

bool odczyt(int ile, char ** argumenty, string &szinput, string &szoutput)
{
	const string etykieta_input("-g");
	const string etykieta_output("-c");
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

void help()
{
	cout << "Witam w programie wyszukujacym cykle w grafie skierowanym!" << endl;
	cout << "Jak uruchomic program?" << endl;
	cout << "W wierszu polecen nalezy wpisac nazwe programu z rozszerzeniem .exe," << endl;
	cout << "nastepnie po przelaczniku -g nazwe pliku tekstowego z danymi wejsciowymi" << endl;
	cout << "i po przelaczniku -c nazwe pliku tekstowego w ktorym maja zapisane zostac dane wyjsciowe" << endl;
	cout << "Jak maja wygladac dane wejsciowe?" << endl;
	cout << "Nalezy podac krawedzie w nastepujacy sposob" << endl;
	cout << "<wierzcholek poczatkowy> -> <wierzcholek koncowy>," << endl;
	cout << "Gdzie wierzcholki sa liczbami naturalnymi a kazda krawedz jest zakonczona przecinkiem, na przyklad:" << endl;
	cout << "1->2, 2->3, 3->1," << endl;

}


wierzcholek * zwroc_adres_wierzcholka(wierzcholek *&pHead, int szukana)
{

	if (!pHead)																	//jesli lista nie istnieje
	{
		return pHead = new wierzcholek{ szukana,0,pHead,0 };					//stworz pierwszy element listy i zwroc jego adres
	}

	auto p = pHead;																//utworzenie zmiennej pomocniczej
	while (p->pNext && p->numer != szukana)										//petla sprawdzajaca czy dany element jest juz na liscie
	{
		p = p->pNext;
	}
	if (p->numer == szukana)													//jesli dany element juz istnieje
	{
		return p;																//zwroc jego adres
	}
	p = new wierzcholek{ szukana,0,pHead,0 };									//w przeciwnym wypadku stworz nowy element o zadanej wartosci
	pHead = p;
	return p;
}

void dodaj_krawedz(wierzcholek *& pHead, int pierwszy, int drugi)
{
	wierzcholek	*peak = zwroc_adres_wierzcholka(pHead, pierwszy);				//przypisz do zmiennej adres wierzcholka poczatkowego
	wierzcholek *edge = zwroc_adres_wierzcholka(pHead, drugi);					//przypisz do zmiennej adres wierzcholka koncowego
	auto glowa_podlisty = peak->pKrawedzie;
	krawedz *p = new krawedz{ edge,glowa_podlisty };							//utworz wskaznik na wierzcholek koncowy i dodaj go do podlisty wierzcholka poczatkowego
	glowa_podlisty = p;
	peak->pKrawedzie = p;
}

string usun_strzalke(string &linia)
{
	for (int i = 0; i < linia.size(); i++)										//przejdz po wszystkich znakach w lancuchu
	{
		if (linia[i] == '-' || linia[i] == '>')									//jesli napotkasz podane znaki
		{
			linia[i] = ' ';														//zastap je znakiem bialym
		}
	}
	return linia;																//zwroc edytowany lancuch
}

void usun_szczyt_stosu(stos *&pHead)
{
	if (pHead)																	//jesli stos istnieje
	{
		auto p = pHead;
		pHead = pHead->pNext;													//przypisz glowe stosu nastepnemu elementowi
		delete p;																//usun szczyt stosu
	}
}

void usun_caly_stos(stos *&pHead)
{
	while (pHead)																//dopoki na stosie znajduja sie elementy
	{
		auto p = pHead;
		pHead = pHead->pNext;													//przypisz glowe stosu nastepnemu elementowi
		delete p;																//usun szczyt stosu
	}
}

void dodaj_na_stos(stos *&pHead, int numer)
{
	stos *nowy = new stos{ numer,pHead };										//utorz nowy element stosu 
	pHead = nowy;																//dodaj go na szczyt
}

bool szukaj_cyklu(wierzcholek *glowa, int szukana, stos *&peak)
{
	int u;
	glowa->odwiedziny = true;													//oznacz wierzcholek jako odwiedzony
	auto d = glowa->pKrawedzie;													//przypisz glowe podlisty do zmiennej pomocniczej d
	dodaj_na_stos(peak, glowa->numer);											//dodaj aktualny wierzcholek na stos
	while (d)																	//dopoki w podliscie sa elementy
	{
		u = d->pEdge->numer;													//przypisz do zmiennej u numer wierzcholka (wierzcholek koncowy) na ktory wskazuje element podlisty 
		if (u == szukana)														//jesli numer wierzcholka jest rowny szukanej
		{
			return true;														//zwroc wartosc true - cykl zostal znaleziony
		}
		if (!(d->pEdge->odwiedziny) && szukaj_cyklu(d->pEdge, szukana, peak))	//jesli wierzcholek nie zostal odwiedzony, powtorz szukanie dla jego krawedzi
		{
			return true;														//jesli znaleziono cykl, zwroc wartosc true
		}
		d = d->pDown;															//przejdz do nastepnego elementu podlisty
	}
	usun_szczyt_stosu(peak);													//jesli nie znaleziono cyklu, usun szczyt stosu
	return false;																//zwroc wartosc false - cykl nie zostal znaleziony
}


void wypisz_stos(stos *pHead, ofstream &plik)
{
	if (pHead)																	//jesli wskaznik nie jest koncem listy
	{
		wypisz_stos(pHead->pNext, plik);										//przejdz do kolejnego elementu i wykonaj funkcje az dojdziesz do szczytu stosu
		plik << pHead->number << "\t";											//wypisz element stosu
	}
}

void zeruj_odwiedziny(wierzcholek *&pHead)
{
	auto p = pHead;
	while (p)																	//dopoki nie dojdziesz do konca listy
	{
		p->odwiedziny = 0;														//oznacz element jako nieodwiedzony
		p = p->pNext;															//przejdz do nastepnego elementu
	}
}


void usun_liste_wierzcholkow(wierzcholek *&pHead)
{
	while (pHead)																//dopoki nie dojdziesz do konca listy
	{
		auto p = pHead;
		pHead = pHead->pNext;													//ustaw nastepny element jako glowe listy
		delete p;																//usun biezacy element
	}
}

void usun_liste_krawedzi(krawedz *&pHead)
{
	while (pHead)																//dopoki nie dojdziesz do konca listy
	{
		auto p = pHead;
		pHead = pHead->pDown;													//ustaw nastepny element jako glowe listy
		delete p;																//usun biezacy element
	}
}
