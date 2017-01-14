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

	std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
	std::cout.precision(20);
	srand (time(NULL));

	if (argc > 1)
	{
		if (argc % 2 == 0)
		{
			cout << "Missing the correct number of parameters" << endl;
			cout << "Suggested use: $./recommendation –d <input file> -ο <output file>" << endl;
			return -1;
		}
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-d") == 0)
			{
				inputFile.open(argv[i+1]);	//Input file comes next on argv
				if (inputFile == NULL)
				{
					cout << "You've given a wrong input file. " << endl;
					exit(1);
				}
				else
				{
					cout << "File : " << argv[i+1] << " opened successfully!" << endl;
					//inParameter = true;
				}
				
				//Init_Metrics(myMetric, inputFile);
				i++;
			}
			else if (strcmp(argv[i], "-o") == 0)
			{
				outputFile.open(argv[i+1]);		//Output file comes next on argv
				if (outputFile == NULL)
				{
					cout << "You've given a wrong output file. " << endl;
					exit(1);
				}
				else
				{
					cout << "File : " << argv[i+1] << " opened successfully!" << endl;
					//outParameter = true;
				}

				i++;
			}
			else if (strcmp(argv[i], "-validate") == 0)
			{
				validateFlag = true;
			}
			else
			{
				cout << "You've given wrong input" <<endl;
				return -1;
			}
		}
	}


	CLI( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids,clusterAssign, L, k, validateFlag) ;


	delete myConf;
	delete myMetric;

	return 0;
}