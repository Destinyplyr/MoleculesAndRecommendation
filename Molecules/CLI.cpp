#include "mainSample.h"

void CLI(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, bool complete_printing) 
{
	int total;
	string GARBAGE;
	int point_number, size_of_cluster;
	int* items_in_cluster_itemNo;
	string*	items_in_cluster_itemName;
	bool first_time_lsh = 1; 	




	Init_Metrics(myMetric, inputFile);

	double*** all_conformation_table;

	ListData<double**>* vectorList = new ListData<double**>();
	vectorList->ListInsertionVector(inputFile, myMetric);

	all_conformation_table = vectorList->AllConformationTableCreation(myMetric);
	vectorList->ConformationTableMove(myMetric, all_conformation_table);
	//cout << "all_conformation_table[0][0][1] : " <<all_conformation_table[0][0][1] <<endl;

	ListData<double**>* euclideanList = vectorList;


	clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);

	euclideanList->DistanceMatrixComputationVector(myMetric, distance_matrix, all_conformation_table);

	euclideanList->ClusterHandleExercise3( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing, NULL, false);

	clusterAssign= new int*[myMetric->point_number];	//deleting clusterAssign on first round inside clusterhandexercise3
	for (int i = 0; i < myMetric->point_number; ++i)
	{
	    clusterAssign[i] = new int[3];
	    clusterAssign[i][0] = -1;
	    clusterAssign[i][1] = -1;
	    clusterAssign[i][2] = -1;
	}
	clusterTable = new ClusterTable(myConf->number_of_clusters);

	euclideanList->DistanceConformationVectorHandle(inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L, k, all_conformation_table);

	//DELETE SEGMENT
	/*for (int i = 0; i < myMetric->point_number; i++) {
	    delete [] distance_matrix[i];
	}
	delete[] distance_matrix;       //distance matrix deletion
	//cout << "ekana to distance" << endl;
	delete[] centroids;*/
	//cout << "ekana to insertion" << endl;

	delete vectorList;

	for (int i = 0; i < myMetric->point_number; i++)
	{

		for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
		{
			delete[] all_conformation_table[i][current_backbone_atom];
		}
		delete[] all_conformation_table[i];
	}
	delete[] all_conformation_table;

	//delete clusterTable;

	
	//delete hashTableList;
	//cout << "list: " << (*clusterTable)->getArray() <<endl;
	/*for (int i = 0; i < myMetric->point_number; ++i)
	{
	    delete[] clusterAssign[i];
	}
	delete[] clusterAssign;*/
	for (int i = 0; i < myMetric->point_number; i++) {
	    delete[] distance_matrix[i];
	}
	delete[] distance_matrix;


	delete[] centroids;
	

}
