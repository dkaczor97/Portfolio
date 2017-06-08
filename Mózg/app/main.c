#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mozg.h"


int main(int argc, char *argv[])
{
	layer *head;
	head = NULL;
	char *modelFileName;
	char *dataFileName;
	char *outputFileName;
	if (openProgram(argc, argv, &modelFileName, &dataFileName, &outputFileName))
	{
		createNet(&head, modelFileName);
		generateOutputValues(&head, dataFileName, outputFileName);
		freeMemory(&head);
	}
	else
	{
		printf("ERROR! Sprawdz poprawnosc wprowadzonych danych i uruchom program jeszcze raz\n");
		printf("Nalezy podac dwa pliki wejsciowe i jeden wyjsciowy po przelacznikach:\n");
		printf("-i plik_z_danymi.txt -m plik_z_modelem_sieci.txt -o plik_wyjsciowy.txt\n");
		printf("kolejnosc podawania przelacznikow z plikami jest dowolna");
	}

	return 0;
}



