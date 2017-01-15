#include "mainSample.h"

void CLI(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, bool complete_printing) 
{
	int total;
	string GARBAGE;
	int point_number, size_of_cluster;
	int* items_in_cluster_itemNo;
	string*	items_in_cluster_itemName;
	bool first_time_lsh = 1; 	
	int hashCreationDone = 0;
	clock_t clustering_start, clustering_finish, LSH_start, LSH_finish;
	double time_taken;

	myMetric->metric_space = "hamming";
	Init_Metrics(myMetric, inputFile);



	if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
	{
		ListData<boost::dynamic_bitset<> >* hammingList = new ListData<boost::dynamic_bitset<> >();
		hammingList->ListInsertionHamming(inputFile, myMetric);
		clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);

		hammingList->DistanceMatrixComputationHamming(myMetric, distance_matrix);

		outputFile << "##############Algorithm: Hamming Clustering" <<endl;
		cout << "##############Algorithm: Hamming Clustering" <<endl;

		clustering_start = clock();

		hammingList->ClusterHandleExercise3( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);
		clustering_finish = clock();
		time_taken = (double)(clustering_finish - clustering_start )/CLOCKS_PER_SEC;	//total clustering time

		outputFile << "Time taken: " << time_taken <<endl;

		clusterAssign= new int*[myMetric->point_number];
		for (int i = 0; i < myMetric->point_number; ++i)
		{
		    clusterAssign[i] = new int[3];
		    clusterAssign[i][0] = -1;
		    clusterAssign[i][1] = -1;
		    clusterAssign[i][2] = -1;
		}
		int hashCreationDone = 0;
		Hash<boost::dynamic_bitset<> >* hashTableList = new Hash<boost::dynamic_bitset<> >[L]();

		outputFile << "##############Algorithm: Hamming LSH" <<endl;
		cout << "##############Algorithm: Hamming LSH" <<endl;

		LSH_start = clock();
		hammingList->initHammingLSHManagement(myConf, myMetric, inputFile, outputFile, distance_matrix,  k,  L, &myMetric->point_number, &hashCreationDone, hashTableList, centroids, clusterAssign, complete_printing);
		LSH_finish = clock();
		time_taken = (double)(LSH_finish - LSH_start )/CLOCKS_PER_SEC;	//total clustering time
		outputFile << "Time taken: " << time_taken <<endl;


		delete[] hashTableList;

		delete hammingList;
	}



	myMetric->metric_space = "vector";


	if (strcmp(myMetric->metric_space.c_str(), "vector") == 0)
	{
		ListData<double*>* vectorList = new ListData<double*>();
		vectorList->ListInsertionVector(inputFile, myMetric);
		myMetric->metric = "euclidean";
		if (strcmp(myMetric->metric.c_str(), "euclidean") == 0)
		{

			ListData<double*>* euclideanList = vectorList;//= new ListData<double*>();


			clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);
			

			euclideanList->DistanceMatrixComputationVector(myMetric, distance_matrix);
			outputFile << "##############Algorithm: Euclidean Clustering" <<endl;
			cout << "##############Algorithm: Euclidean Clustering" <<endl;

			clustering_start = clock();
			euclideanList->ClusterHandleExercise3( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);
			clustering_finish = clock();
			time_taken = (double)(clustering_finish - clustering_start )/CLOCKS_PER_SEC;	//total clustering time

			outputFile << "Time taken: " << time_taken <<endl;

			clusterAssign= new int*[myMetric->point_number];
			for (int i = 0; i < myMetric->point_number; ++i)
			{
			    clusterAssign[i] = new int[3];
			    clusterAssign[i][0] = -1;
			    clusterAssign[i][1] = -1;
			    clusterAssign[i][2] = -1;
			}

			int hashCreationDone = 0;
			Hash<double*>* hashTableList = new Hash<double*>[L]();
			outputFile << "##############Algorithm: Euclidean LSH" <<endl;
			cout << "##############Algorithm: Euclidean LSH" <<endl;

			LSH_start = clock();
			euclideanList->initEuclideanList(myConf ,myMetric, inputFile, outputFile, distance_matrix, k, L, &(myMetric->point_dimension), &(myMetric->point_number), &hashCreationDone, hashTableList, centroids, clusterAssign, complete_printing);
			LSH_finish = clock();
			time_taken = (double)(LSH_finish - LSH_start )/CLOCKS_PER_SEC;	//total clustering time
			outputFile << "Time taken: " << time_taken <<endl;

			delete[] hashTableList;
		}
		myMetric->metric = "cosine";
		if (strcmp(myMetric->metric.c_str(), "cosine") == 0)
		{
			ListData<double*>* cosineList =  vectorList;//= new ListData<double*>();


			clusterTable->Init_Tables(&distance_matrix, myMetric, myConf, &centroids, &clusterTable, &clusterAssign);

			cosineList->DistanceMatrixComputationVector(myMetric, distance_matrix);

			clustering_start = clock();

			outputFile << "##############Algorithm: Cosine Clustering" <<endl;
			cout << "##############Algorithm: Cosine Clustering" <<endl;
			cosineList->ClusterHandleExercise3( inputFile, outputFile, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, complete_printing);

			clustering_finish = clock();
			time_taken = (double)(clustering_finish - clustering_start )/CLOCKS_PER_SEC;	//total clustering time

			outputFile << "Time taken: " << time_taken <<endl;

			clusterAssign= new int*[myMetric->point_number];
			for (int i = 0; i < myMetric->point_number; ++i)
			{
			    clusterAssign[i] = new int[3];
			    clusterAssign[i][0] = -1;
			    clusterAssign[i][1] = -1;
			    clusterAssign[i][2] = -1;
			}

			int hashCreationDone = 0;
			Hash<double*>* hashTableList = new Hash<double*>[L]();

			outputFile << "##############Algorithm: Cosine LSH" <<endl;
			cout << "##############Algorithm: Cosine LSH" <<endl;
			LSH_start = clock();
			cosineList->initCosineList(myConf, myMetric, inputFile, outputFile, distance_matrix, k, L, &(myMetric->point_dimension), &(myMetric->point_number), &hashCreationDone, hashTableList, centroids, clusterAssign, complete_printing);
			LSH_finish = clock();
			time_taken = (double)(LSH_finish - LSH_start )/CLOCKS_PER_SEC;	//total clustering time
			outputFile << "Time taken: " << time_taken <<endl;
		}
		delete vectorList;
	}
	
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
	for (int i = 0; i < myMetric->point_number; ++i)
	{
	    delete[] clusterAssign[i];
	}
	delete[] clusterAssign;

	for (int i = 0; i < myMetric->point_number; i++) {
	    delete[] distance_matrix[i];
	}

	delete[] distance_matrix;

	delete[] centroids;
	

}
