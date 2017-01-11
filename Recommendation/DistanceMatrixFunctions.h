#include "Headers.h"
#include "Returns.h"

double DistanceMatrixDistance(double** inputFile, double item1, double item2);
void quickSort(double* myArray, int first, int last );
int parition(double* myArray, int first, int last);
void swap(int& a, int& b);
void quickSort_twolist(double** myArray, int first, int last );
int parition_twolist(double** myArray, int first, int last);
void swap_twolist(double* a, double* b);
double FindNextRadius(double** min_max_thresh, double oldRadius, double oldInserted, double neighborhood_size);

double FindRadiusForAssignment(Conf* myConf,double** distanceMatrix, int* centroids);