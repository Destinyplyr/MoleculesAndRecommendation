#include "mainSample.h"

void CLI(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, bool complete_printing) 
{
	int total;
	string GARBAGE;
	int point_number, size_of_cluster;
	int* items_in_cluster_itemNo;
	string*	items_in_cluster_itemName;
	bool first_time_lsh = 1; 	
	//int hashCreationDone = 0;

	/*if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
	{

	}
	if (strcmp(myMetric->metric_space.c_str(), "vector") == 0)
	{
		if (strcmp(myMetric->metric.c_str(), "euclidean") == 0)
		{

		}
		
		if (strcmp(myMetric->metric.c_str(), "cosine") == 0)
		{

		}
	}
	if (strcmp(myMetric->metric_space.c_str(), "matrix") == 0)
	{


	}*/

	//myMetric->metric_space = "hamming";
	cout << "test" <<endl;
	Init_Metrics(myMetric, inputFile);

	double*** all_conformation_table;

	ListData<double**>* vectorList = new ListData<double**>();
	vectorList->ListInsertionVector(inputFile, myMetric);

	all_conformation_table = vectorList->AllConformationTableCreation(myMetric);
	vectorList->ConformationTableMove(myMetric, all_conformation_table);
	//cout << "all_conformation_table[0][0][1] : " <<all_conformation_table[0][0][1] <<endl;
	//myMetric->metric = "euclidean";

	ListData<double**>* euclideanList = vectorList;//= new ListData<double*>();
	//euclideanList->ListInsertionVector(inputFile, myMetric);
	//cin >> GARBAGE;

	// //DELETE SEGMENT			//MANDATORY USAGE AFTER OTHER 
	// for (int i = 0; i < myMetric->point_number; i++) {
	//     delete[] distance_matrix[i];
	// }
	// delete[] distance_matrix;       //distance matrix deletion
	// //cout << "ekana to distance" << endl;
	// delete[] centroids;
	// //cout << "ekana to insertion" << endl;

	// /*delete clusterTable;
	// //cout << "list: " << (*clusterTable)->getArray() <<endl;
	// for (int i = 0; i < myMetric->point_number; ++i)
	// {
	//     delete[] clusterAssign[i];
	// }
	// delete[] clusterAssign;*/


	clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);
	// SetClaransFraction(myConf, myMetric);

	euclideanList->DistanceMatrixComputationVector(myMetric, distance_matrix, all_conformation_table);

	euclideanList->ClusterHandleExercise3( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);

	int hashCreationDone = 0;
	Hash<double**>* hashTableList = new Hash<double**>[L]();
	//euclideanList->initEuclideanList(myConf ,myMetric, inputFile, distance_matrix, k, L, &(myMetric->point_dimension), &(myMetric->point_number), &hashCreationDone, hashTableList, centroids, clusterAssign);
	// euclideanList->Printer( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);
	// delete euclideanList;


	/*if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
	{
		ListData<boost::dynamic_bitset<> >* hammingList = new ListData<boost::dynamic_bitset<> >();
		//ListData<bitset<64> >* hammingList = new ListData<bitset<64> >();
		hammingList->ListInsertionHamming(inputFile, myMetric);
		//cout << "ended" <<endl;
		clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);
		// SetClaransFraction(myConf, myMetric);

		hammingList->DistanceMatrixComputationHamming(myMetric, distance_matrix);
		cout << "omorfi" <<endl;

		hammingList->ClusterHandleExercise3( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);
		int hashCreationDone = 0;
		Hash<boost::dynamic_bitset<> >* hashTableList = new Hash<boost::dynamic_bitset<> >[L]();
		hammingList->initHammingLSHManagement(myConf, myMetric, inputFile, distance_matrix,  k,  L, &myMetric->point_number, &hashCreationDone, hashTableList, centroids, clusterAssign);

		// hammingList->Printer( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);
		// delete hammingList;
	}*/
	
	//exit(1);

	// myMetric->metric_space = "vector";


	// if (strcmp(myMetric->metric_space.c_str(), "vector") == 0)
	// {
	// 	ListData<double*>* vectorList = new ListData<double*>();
	// 	vectorList->ListInsertionVector(inputFile, myMetric);
	// 	//myMetric->metric = "euclidean";
	// 	if (strcmp(myMetric->metric.c_str(), "euclidean") == 0)
	// 	{

	// 		ListData<double*>* euclideanList = vectorList;//= new ListData<double*>();
	// 		//euclideanList->ListInsertionVector(inputFile, myMetric);
	// 		//cin >> GARBAGE;

	// 		// //DELETE SEGMENT			//MANDATORY USAGE AFTER OTHER 
	// 		// for (int i = 0; i < myMetric->point_number; i++) {
	// 		//     delete[] distance_matrix[i];
	// 		// }
	// 		// delete[] distance_matrix;       //distance matrix deletion
	// 		// //cout << "ekana to distance" << endl;
	// 		// delete[] centroids;
	// 		// //cout << "ekana to insertion" << endl;

	// 		// /*delete clusterTable;
	// 		// //cout << "list: " << (*clusterTable)->getArray() <<endl;
	// 		// for (int i = 0; i < myMetric->point_number; ++i)
	// 		// {
	// 		//     delete[] clusterAssign[i];
	// 		// }
	// 		// delete[] clusterAssign;*/


	// 		clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);
	// 		// SetClaransFraction(myConf, myMetric);

	// 		euclideanList->DistanceMatrixComputationVector(myMetric, distance_matrix);

	// 		euclideanList->ClusterHandleExercise3( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);

	// 		int hashCreationDone = 0;
	// 		Hash<double*>* hashTableList = new Hash<double*>[L]();
	// 		euclideanList->initEuclideanList(myConf ,myMetric, inputFile, distance_matrix, k, L, &(myMetric->point_dimension), &(myMetric->point_number), &hashCreationDone, hashTableList, centroids, clusterAssign);
	// 		// euclideanList->Printer( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);
	// 		// delete euclideanList;
	// 	}
	// 	myMetric->metric = "cosine";
	// 	if (strcmp(myMetric->metric.c_str(), "cosine") == 0)
	// 	{
	// 		ListData<double*>* cosineList =  vectorList;//= new ListData<double*>();
	// 		//cosineList->ListInsertionVector(inputFile, myMetric);

	// 		cout << "new list" <<endl;

	// 		//DELETE SEGMENT			//MANDATORY USAGE AFTER OTHER 
	// 		/*for (int i = 0; i < myMetric->point_number; i++) {
	// 		    delete[] distance_matrix[i];
	// 		}
	// 		delete[] distance_matrix;       //distance matrix deletion
	// 		//cout << "ekana to distance" << endl;
	// 		delete[] centroids;
	// 		//cout << "ekana to insertion" << endl;*/

	// 		//delete clusterTable;
	// 		//cout << "list: " << (*clusterTable)->getArray() <<endl;
	// 		/*for (int i = 0; i < myMetric->point_number; ++i)
	// 		{
	// 		    delete[] clusterAssign[i];
	// 		}
	// 		delete[] clusterAssign;*/

	// 		cout << "deleted" <<endl;


	// 		clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);
	// 		// SetClaransFraction(myConf, myMetric);

	// 		cosineList->DistanceMatrixComputationVector(myMetric, distance_matrix);

	// 		//cosineList->ClusterHandleExercise3( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);


	// 		int hashCreationDone = 0;
	// 		Hash<double*>* hashTableList = new Hash<double*>[L]();
	// 		cosineList->initCosineList(myConf, myMetric, inputFile,distance_matrix, k, L, &(myMetric->point_dimension), &(myMetric->point_number), &hashCreationDone, hashTableList, centroids, clusterAssign);
	// 		// cosineList->Printer( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);
	// 		// delete cosineList;
	// 	}
	// }

	/*if (strcmp(myMetric->metric_space.c_str(), "matrix") == 0)
	{
		ListData<double*>* DBHList = new ListData<double*>();
		DBHList->ListInsertionDB(inputFile, myMetric);


		clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);
		SetClaransFraction(myConf, myMetric);


		DBHList->DistanceMatrixComputationDB(inputFile, myMetric, distance_matrix);
		DBHList->Printer( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);
		delete DBHList;
	}*/
	//DELETE SEGMENT
	/*for (int i = 0; i < myMetric->point_number; i++) {
	    delete [] distance_matrix[i];
	}
	delete[] distance_matrix;       //distance matrix deletion
	//cout << "ekana to distance" << endl;
	delete[] centroids;*/
	//cout << "ekana to insertion" << endl;

	//delete clusterTable;
	//cout << "list: " << (*clusterTable)->getArray() <<endl;
	/*for (int i = 0; i < myMetric->point_number; ++i)
	{
	    delete[] clusterAssign[i];
	}
	delete[] clusterAssign;*/

}
