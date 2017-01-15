#include "Headers.h"
#include "UtilsH.h"


void Init_Conf(Conf* myConf, ifstream& conFile)
{

 	myConf->number_of_clusters;
	myConf->number_of_hash_functions = 4;
	myConf->number_of_hash_tables = 5;
	myConf->clarans_set_fraction = 0;
	myConf->clarans_iterations = 2;

	string GARBAGE;
	for (int i =0; i < 5; i ++) {
		if (conFile >> GARBAGE) {
			if (strcmp(GARBAGE.c_str(), "number_of_clusters:") == 0) {
				conFile >> myConf->number_of_clusters;
			}
			else if (strcmp(GARBAGE.c_str(), "number_of_hash_functions:") == 0) {
				conFile >> myConf->number_of_hash_functions;
			}
			else if (strcmp(GARBAGE.c_str(), "number_of_hash_tables:") == 0) {
				conFile >> myConf->number_of_hash_tables;
			}
			else if (strcmp(GARBAGE.c_str(), "clarans_set_fraction:") == 0) {
				conFile >> myConf->clarans_set_fraction;
			}
			else if (strcmp(GARBAGE.c_str(), "clarans_iterations:") == 0) {
				conFile >> myConf->clarans_iterations;
			}
		}
		else {
			break;
		}
	}
	//cout << "Information read from configuration file." << endl;
}


void Init_Metrics(Metrics* myMetric, std::ifstream& inputFile)
{
	string GARBAGE;
	string genericStr;
	string pointStr;
	int current_user = -1;
	int point_dimension = 0;
	int point_number = 0;
	int item_max = 0;

	getline(inputFile, genericStr);
	do {

   		stringstream linestream(genericStr);
   		getline(linestream, pointStr, '\t');
   		//cout << pointStr <<endl;
   		if (stoi(pointStr) != current_user)
   		{
   			point_number++;
   			current_user = stoi(pointStr);
   		}
   		getline(linestream, pointStr, '\t');
   		if (stoi(pointStr) > item_max)
   		{
   			item_max = stoi(pointStr);
   		}
   		getline(linestream, pointStr, '\t');

	}while(getline(inputFile, genericStr));

    myMetric->point_dimension = item_max;
    myMetric->point_number = point_number;

 	inputFile.clear();      		//Restart
 	inputFile.seekg(0, ios::beg);   //Data file back from start

}

void SetClaransFraction(Conf* myConf, Metrics* myMetric)
{
	if (myConf->clarans_set_fraction == 0)
	{
		if ((0.12 * myConf->number_of_clusters * (myMetric->point_number - myConf->number_of_clusters)) >= 250)
		{
			myConf->clarans_set_fraction = (0.12 * myConf->number_of_clusters * (myMetric->point_number - myConf->number_of_clusters));
		}
		else
		{
			if (myConf->number_of_clusters * (myMetric->point_number - myConf->number_of_clusters) < 250)
			{
				myConf->clarans_set_fraction = myConf->number_of_clusters * (myMetric->point_number - myConf->number_of_clusters);
			}
			else
			{
				myConf->clarans_set_fraction = 250;
			}
		}
	}
}



bool Exists(int* items, int pos, int newItem)
{
	for (int i = 0; i < pos; ++i)
	{
		if (items[i] == newItem)
		{
			return true;
		}
	}
	return false;
}





double ObjectiveCost(int** clusterAssign, double** distanceMatrix, Metrics* myMetric)		//returns the cost of all
{
	int column, row, j;
	double theCost = 0.0f;

	for (int i = 0; i < myMetric->point_number; ++i)
	{
		theCost += DistanceMatrixDistance( distanceMatrix, clusterAssign[i][0], i);
	}

	return theCost;
}







int ReturnCluster(Conf* myConf, int* centroids, int centroid) {
	for (int i = 0; i < myConf->number_of_clusters; ++i)
	{
		if (centroids[i] == centroid)
		{
			return i;
		}
	}
}