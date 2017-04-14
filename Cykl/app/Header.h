
#ifndef Header_h
#define Header_h

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;
struct krawedz;


//lista jednokierunkowa do przechowywania wierzcholkow grafu
struct  wierzcholek
{
	int numer;						//numer wierzcholka
	bool odwiedziny;				//informacja czy w trakcie przeszukiwania grafu, wierzcholek zostal odwiedzony
	wierzcholek *pNext;				//wskaznik na nastepny element listy
	krawedz *pKrawedzie;			//wskaznik na pierwszy element podlisty przechowujacej krawedzie
};


//lista jednokierunkowa do przechowywania krawedzi grafu
struct krawedz
{
	wierzcholek * pEdge;			//wskaznik na wierzcholek koncowy krawedzi
	krawedz * pDown;				//wskaznik na nastepny element listy
};


//stos do przechowywania odwiedzonych wierzcholkow trakcie wyszukiwania cyklu
struct stos
{
	int number;						//numer wierzcholka
	stos *pNext;					//wskaznik na nastepny element stosu
};


/*
Funkcja wczytuje parametry wywo³ania programu i sprawdza ich poprawnoœæ, s¹ one podawane w linii poleceñ jako prze³¹czniki w dowolnej kolejnoœci
*/
bool odczyt(int ile, char ** argumenty, string &szinput, string &szoutput);

//procedura wypisujaca w konsoli menu pomocy
void help();

//funkcja zwracajaca adres wierzcholka podanego jako parametr funkcji
wierzcholek * zwroc_adres_wierzcholka(wierzcholek *&pHead, int szukana);

//Procedura dodajaca do podlisty krawedz
void dodaj_krawedz(wierzcholek *& pHead, int pierwszy, int drugi);

//funkcja usuwajaca z dostarczonego lancucha znak myslnika i wiekszosci, pozostawiajac tylko numery wierzcholkow
string usun_strzalke(string &linia);

//procedura usuwajaca pierwszy element z dostarczonego stosu
void usun_szczyt_stosu(stos *&pHead);

//procedura usuwajaca caly dostarczony stos
void usun_caly_stos(stos *&pHead);

//procedura dodajaca element na szczyt stosu
void dodaj_na_stos(stos *&pHead, int numer);

//funkcja wyszukujaca cykl w grafie
bool szukaj_cyklu(wierzcholek *glowa, int szukana, stos *&peak);

//procedura wypisujaca stos do pliku
void wypisz_stos(stos *pHead, ofstream &plik);

//procedura oznaczajace wszystkie wierzcholki jako nieodwiedzone
void zeruj_odwiedziny(wierzcholek *&pHead);

//procedura usuwajaca liste wierzcholkow
void usun_liste_wierzcholkow(wierzcholek *&pHead);

//procedura usuwajaca podliste wierzcholka
void usun_liste_krawedzi(krawedz *&pHead);

#endif
