#include <bitset>
#include "NodeFunctions.cpp"
#include "ListsFunctions.cpp"
#include "VectorHammingListManagement.cpp"
#include "HammingListManagement.cpp"
#include "HammingLSHManagement.cpp"
#include "HashFunctions.cpp"
#include "EuclideanList.cpp"
#include "CosineList.cpp"
#include "ConformationFunctions.cpp"
// #include "DBListManagement.cpp"
// #include "DBHManagement.cpp"
#include "Headers.h"
#include "UtilsH.h"
#include "Algorithms.h"

void CLI(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, bool complete_printing);
//void CLI(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric);
// void ReadFiles(ifstream& inputFile, Metrics* myMetric, int* N, double** distanceMatrix);