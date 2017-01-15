#include "mainSample.h"

int main(int argc, char const *argv[])
{
	
	ifstream inputFile;
	ifstream conFile;
	ofstream outputFile;
	int initChoice;
	int assignChoice;
	int updateChoice;
	int* centroids;
	int point_number;
	ClusterTable* clusterTable;
	int** clusterAssign;
	double** distance_matrix;
	string GARBAGE;
	string choice;
	string filename;
	Conf* myConf = new Conf();
	Metrics* myMetric = new Metrics();
	bool validateFlag = false;
	bool first_time_lsh;
	int L, k;
	L = 5;
	k = 4;
	int hashCreationDone;
	bool outParameter = false, inParameter = false, confParameter = false;

	srand (time(NULL));

	inputFile.open("input.dat");
	if (inputFile == NULL)
	{
		cout << "You've given a wrong input file. " << endl;
		exit(1);
	}
	else
	{
		cout << "File : " << "input.dat" << " opened successfully!" << endl;
	}

	outputFile.open("conform.dat");		//Output file comes next on argv
	if (outputFile == NULL)
	{
		cout << "You've given a wrong output file. " << endl;
		exit(1);
	}
	else
	{
		cout << "File : " << "conform.dat" << " opened successfully!" << endl;
	}

	CLI( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids,clusterAssign, L, k, validateFlag) ;


	delete myConf;
	delete myMetric;

	return 0;
}