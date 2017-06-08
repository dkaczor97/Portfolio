#include "mozg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void addOrCreateLayer(layer **pHead, int number)
{

	if (!*pHead)
	{
		(*pHead) = (layer*)malloc(sizeof(layer));
		(*pHead)->layerNumber = number;
		(*pHead)->listOfNeurons = NULL;
		(*pHead)->pNextLayer = NULL;
	}
	else
	{
		layer *newLayer = (layer*)malloc(sizeof(layer));
		layer *temp = *pHead;
		while (temp->pNextLayer != NULL)
		{
			temp = temp->pNextLayer;
		}
		newLayer->layerNumber = number;
		newLayer->listOfNeurons = NULL;
		newLayer->pNextLayer = NULL;
		temp->pNextLayer = newLayer;
	}
}


neurone* addOrCreateNeurone(layer **pHead)
{
	neurone *neuroneHead = (*pHead)->listOfNeurons;
	if (!neuroneHead)
	{
		neuroneHead = (neurone*)malloc(sizeof(neurone));
		neuroneHead->outputValue = 0;
		neuroneHead->listOfDendrites = NULL;
		neuroneHead->pNextNeurone = NULL;
		return (*pHead)->listOfNeurons = neuroneHead;

	}
	else
	{
		neurone *newNeurone = (neurone*)malloc(sizeof(neurone));
		neurone *temp = neuroneHead;
		while (neuroneHead->pNextNeurone)
		{
			neuroneHead = neuroneHead->pNextNeurone;
		}
		newNeurone->outputValue = 0;
		newNeurone->listOfDendrites = NULL;
		newNeurone->pNextNeurone = NULL;
		return neuroneHead->pNextNeurone = newNeurone;

	}

}

void addOrCreateDendrite(neurone **pHead, float weight)
{
	dendrite *dendriteHead = (*pHead)->listOfDendrites;
	if (!dendriteHead)
	{
		dendriteHead = (dendrite*)malloc(sizeof(dendrite));
		dendriteHead->dendriteWeight = weight;
		dendriteHead->pNextDendrite = NULL;
		(*pHead)->listOfDendrites = dendriteHead;
	}
	else
	{
		dendrite *newDendrite = (dendrite*)malloc(sizeof(dendrite));
		while (dendriteHead->pNextDendrite)
		{
			dendriteHead = dendriteHead->pNextDendrite;
		}
		newDendrite->dendriteWeight = weight;
		newDendrite->pNextDendrite = NULL;
		dendriteHead->pNextDendrite = newDendrite;
	}

}

layer* findLastLayer(layer *pHead)
{
	while ((pHead)->pNextLayer)
	{
		(pHead) = (pHead)->pNextLayer;
	}
	return (pHead);
}



void createNet(layer **pHead, char *modelFileName)
{
	int layerCount = 0;
	float readWeight;
	char readLine[100];
	char *tempLine;
	FILE *inputFile;
	layer *lastLayer;
	neurone *lastNeurone;

	addOrCreateLayer(*&pHead, layerCount);
	lastLayer = findLastLayer(*pHead);

	if (inputFile = fopen(modelFileName, "r"))
	{
		while (fgets(readLine, sizeof(readLine), inputFile))
		{
			if (readLine[0] == '\n')
			{
				layerCount++;
				addOrCreateLayer(*&pHead, layerCount);
				lastLayer = findLastLayer(*pHead);
			}
			else
			{
				lastNeurone = addOrCreateNeurone(&lastLayer);
				tempLine = strtok(readLine, " ");
				while (tempLine)
				{
					sscanf(tempLine, "%f", &readWeight);
					addOrCreateDendrite(&lastNeurone, readWeight);
					tempLine = strtok(NULL, " ");

				}
			}
		}
		fclose(inputFile);
	}
	else
    {
        printf("ERROR! Nie mozna otworzyc pliku z modelem sieci!");
        freeMemory(&*pHead);
        exit(0);
    }
}


void generateOutputValues(layer **pHead, char *dataFileName, char *outputFileName)
{
	layer *firstLayer;
	layer *secondLayer;
	neurone *firstNeuronesList;
	neurone *secondNeuronesList;
	dendrite *dendritesList;
	float atanParameter;

	neurone *tempNeuroneHead;
	FILE *dataFile;
	float readValue;
	char readLine[100];
	char *tempLine;

	FILE *outputFile;
	layer *lastLayer;

	if (dataFile = fopen(dataFileName, "r"))
	{
	    if(outputFile = fopen(outputFileName, "w"))
        {
            while (fgets(readLine, sizeof(readLine), dataFile))
            {
                tempNeuroneHead = (*pHead)->listOfNeurons;
                tempLine = strtok(readLine, " ");
                while (tempLine && tempNeuroneHead)
                {
                    sscanf(tempLine, "%f", &readValue);
                    tempLine = strtok(NULL, " ");
                    tempNeuroneHead->outputValue = atan(readValue * (tempNeuroneHead->listOfDendrites->dendriteWeight));
                    tempNeuroneHead = tempNeuroneHead->pNextNeurone;
                }
                //ustawiono pierwsze wartosci, mozna obliczac kolejne
                firstLayer = (*pHead);
                secondLayer = (*pHead)->pNextLayer;
                while (secondLayer)
                {
                    secondNeuronesList = secondLayer->listOfNeurons;
                    while (secondNeuronesList)
                    {
                        atanParameter = 0;
                        firstNeuronesList = firstLayer->listOfNeurons;
                        dendritesList = secondNeuronesList->listOfDendrites;
                        while (dendritesList)
                        {
                            atanParameter += ((dendritesList->dendriteWeight)*(firstNeuronesList->outputValue));
                            dendritesList = dendritesList->pNextDendrite;
                            firstNeuronesList = firstNeuronesList->pNextNeurone;
                        }
                        secondNeuronesList->outputValue = atan(atanParameter);
                        secondNeuronesList = secondNeuronesList->pNextNeurone;
                    }
                    secondLayer = secondLayer->pNextLayer;
                    firstLayer = firstLayer->pNextLayer;
                }

			//ustwiono reszte wartosci, mozna tworzyc plik wynikowy
			lastLayer = findLastLayer(*pHead);
			fprintf(outputFile, "%f\n", lastLayer->listOfNeurons->outputValue);

            }

        }

		fclose(dataFile);
		fclose(outputFile);
	}
	else
    {
        printf("ERROR! Nie mozna otworzyc pliku z danymi");
        freeMemory(&*pHead);
        exit(0);
    }
}

int openProgram(int numberOfParameters, char *parameters[], char **modelFileName, char **inputFileName, char **outputFileName)
{
	char *modelSwitch = "-m";
	char *inputSwitch = "-i";
	char *outputSwitch = "-o";
	int i;
	int correctModelParameter = 0;
	int correctInputParameter = 0;
	int correctOutputParameter = 0;
	for (i = 0; i<numberOfParameters; i++)
	{
		if (strcmp(parameters[i], modelSwitch)==0)
		{
			*modelFileName = parameters[i + 1];
			correctModelParameter = 1;

		}
		if (strcmp(parameters[i], inputSwitch)==0)
		{
			*inputFileName = parameters[i + 1];
			correctInputParameter = 1;
		}
		if (strcmp(parameters[i], outputSwitch)==0)
		{
			*outputFileName = parameters[i + 1];
			correctOutputParameter = 1;
		}
	}
	if (correctInputParameter && correctModelParameter && correctOutputParameter)
		return 1;
	else
		return 0;
}


void deleteDendrites(dendrite **pHead)
{
	dendrite *temp;
	while (*pHead)
	{
		temp = *pHead;
		(*pHead) = (*pHead)->pNextDendrite;
		free(temp);
	}
}

void deleteNeurons(neurone **pHead)
{
	neurone *temp;
	while (*pHead)
	{
		deleteDendrites(&(*pHead)->listOfDendrites);
		temp = (*pHead);
		(*pHead) = (*pHead)->pNextNeurone;
		free(temp);
	}
}
void freeMemory(layer **pHead)
{
	layer *temp;
	while (*pHead)
	{
		deleteNeurons(&(*pHead)->listOfNeurons);
		temp = (*pHead);
		(*pHead) = (*pHead)->pNextLayer;
		free(temp);
	}

}

