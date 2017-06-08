#ifndef MOZG_H_INCLUDED
#define MOZG_H_INCLUDED

typedef struct Dendrites
{
	float dendriteWeight;
	struct Dendrites *pNextDendrite;
}dendrite;


typedef struct Neurons
{
	float          outputValue;
	dendrite      *listOfDendrites;
	struct Neurons  *pNextNeurone;
}neurone;


typedef struct Layers
{
	int             layerNumber;
	neurone         *listOfNeurons;
	struct Layers   *pNextLayer;
}layer;







void deleteDendrites(dendrite **pHead);
void deleteNeurons(neurone **pHead);
void freeMemory(layer **pHead);
void addOrCreateLayer(layer **pHead, int number);
neurone* addOrCreateNeurone(layer **pHead);
void addOrCreateDendrite(neurone **pHead, float weight);
layer* findLastLayer(layer *pHead);
void createNet(layer **pHead, char *modelFileName);
void generateOutputValues(layer **pHead, char *dataFileName, char *outputFileName);
int openProgram(int numberOfParameters, char *parameters[], char **modelFileName, char **inputFileName, char **outputFileName);


#endif // MOZG_H_INCLUDED
