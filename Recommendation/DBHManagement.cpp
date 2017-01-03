#include "ListData.h"
#include "MathFunctions.h"
#include "DistanceMatrixFunctions.h"

using namespace std;


template <typename T>                                                                                           //dataLength is number of points
void ListData<T>::initDBHManagement(Conf* myConf, ifstream& inputFile, double** distanceMatrix, int k, int L, int* dataLength, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign) 
{
	string genericStr;
	string line;
	string genericQuery;
	string pointStr;
	string metric;
	string GARBAGE;
	string metric_space;   
	string* itemName;
	string filename;
	string choice;
	double*** h;
	double* h_x1_x2;
	double* point;
	double cdis;
	double Radius = 0;
	double elapsed_secs_lshc;
	double elapsed_secs_brutec;
	double d_x1_x2, h_x1_x2_x;
	double minCBruteDistance= 99999;
	double minLSHDistance = 999999;
	double minDistance;
	double minimumDistance;
	double secondBestDistance;
	double distance_bucketpoint_from_centroid = 0;
	int lshCNN = 0;
	int realCNN = 0;
	int ind = 0;
	int hashResult = 0;
	int index = 0;
	int queryCounter = 1;
	int tableSize;
	int minimumCentroid;
	int secondBestCentroid;
	Node<T>* nodePtr;
	Node<T>* minimumNode = NULL;

	bool assigned_in_this_radius = false;

	clock_t begin, begin_brute, end_brute, end_Matrix_insert, end_h_creation;
	clock_t begin_lsh_hashing, end_lsh_hashing, begin_h_creation;
	clock_t begin_lsh_query, end_lsh_query;

	double** point_to_centroid_assignment = new double*[*dataLength];           //[0] holds current centroid  [1] holds distance from it [2] holds second best centroid [3]  holds distance from it
	for (int i = 0; i < *dataLength; ++i)
	{
		point_to_centroid_assignment[i] = new double[4];
		point_to_centroid_assignment[i][0] = -1;            //init   
		point_to_centroid_assignment[i][1] = INT_MAX;
		point_to_centroid_assignment[i][2] = -1;
		point_to_centroid_assignment[i][3] = INT_MAX;
	}

	std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
	std::cout.precision(20);
	
	inputFile.clear();              //Restart
	inputFile.seekg(0, ios::beg);   //Data file back from start
	itemName = new string[*dataLength];
	inputFile >> metric_space;      //Read "@metric space"     
	inputFile >> metric_space;      //Read "matrix"
	inputFile >> genericStr;        //Read items
	getline(inputFile, genericStr);
	stringstream linestream2(genericStr);
	while (getline(linestream2, pointStr, ',')) //Calculate dimension of points
	{       
		itemName[ind] = pointStr;
		ind++;
	}

	tableSize = (*dataLength) / 8;
	end_Matrix_insert = clock();
	begin_h_creation = clock();

	h = new double**[L];
	h_x1_x2 = new double[*dataLength];
	for (int o = 0; o < L; ++o)
	{
		h[o] = new double*[k];
		for (int j = 0; j < k; j++) 
		{
			h[o][j] = new double[4];             //x1,x2,t1,t2
		}
	}

	for (int o = 0; o < L; o++)         //for every hash table
	{   
		for (int j = 0; j < k; j++) 		//for every h
		{ 
			h[o][j][0] = floor(((double)rand() / (double)RAND_MAX)*((double)(*dataLength))); //x_1
			h[o][j][1] = floor(((double)rand() / (double)RAND_MAX)*((double)(*dataLength))); //x_2
			d_x1_x2 = pow(DistanceMatrixDistance(distanceMatrix, h[o][j][0], h[o][j][1]), 2);
			for (int u = 0; u < *dataLength; u++) {
				h_x1_x2[u] = (pow(DistanceMatrixDistance(distanceMatrix, u, h[o][j][0]),2) + pow(DistanceMatrixDistance(distanceMatrix, u, h[o][j][1]),2) - d_x1_x2)/(2*d_x1_x2);
			}
			quickSort(h_x1_x2, 0, (*dataLength)-1);
			if ((*dataLength) %2 == 0) {
				h[o][j][2] = h_x1_x2[(*dataLength) / 2];//t1
			}
			else {
				h[o][j][2] = 0.5* (h_x1_x2[(*dataLength) / 2] + h_x1_x2[((*dataLength) / 2)-1]) ;//t1
			}
			h[o][j][3] = numeric_limits<double>::max() ; //t_2
		}
	}

 	inputFile.clear();              //Restart
  	inputFile.seekg(0, ios::beg);   //Data file back from start

	end_h_creation = clock();
  	begin_lsh_hashing = clock();
	if (*hashCreationDone == 0)		//if we haven't created hashtable (assign-update cycle)
	{
		nodePtr = this->ReturnHead();
		for (int o = 0; o < L; ++o)
		{
			hashTableList[o].initHash(tableSize);
		}
		for (int u = 0; u <*dataLength; u++) {  //For every point
			for (int o = 0; o < L; ++o){        //For every hashtable
				hashResult = 0;
				for (int i=0; i < k; i++) {
					d_x1_x2 = pow(DistanceMatrixDistance(distanceMatrix, h[o][i][0], h[o][i][1]), 2);
					h_x1_x2_x = (pow(DistanceMatrixDistance(distanceMatrix, u, h[o][i][0]),2) + pow(DistanceMatrixDistance(distanceMatrix, u, h[o][i][1]),2) - d_x1_x2)/(2*d_x1_x2);
					if ((h_x1_x2_x >= h[o][i][2])&& (h_x1_x2_x <= h[o][i][3]) ) {
						hashResult += pow (2, i);
						if (tableSize < pow(2,k)) 		//VERY CAREFUL - REVIEW THIS!!! //pow(2, k) number of buckets on normal dbh
						{
							hashResult = hashResult % 2;
						}
					}
				}
				//cout << "Point " << u << " Hashtable "<< o <<" Building hashTable: Inserting in bucket: " <<hashResult <<endl;
				hashTableList[o].Insert(hashResult, nodePtr->getKey(), hashResult, u, itemName[u]);
				//cout << " ----> Checking insert: " << hashTableList[o].getHashTable()[hashResult].getBucket()->getItemNo() <<endl;
			}
			nodePtr = nodePtr->getNext();
		}
		*hashCreationDone = 1;
	}
	for (int o = 0; o < L; ++o) //re-initialize hashTable //for every hash table
	{
		hashTableList[o].ReInitializeHashTable(L, tableSize);
	}
	  	//end_lsh_hashing = clock();

			Radius = FindRadiusForAssignment(myConf, distanceMatrix, centroids); 
			nodePtr = NULL;
			minimumNode = NULL; 	
			for (int o = 0; o < L; ++o) 	//for every hashtable
			{
				do
				{
					assigned_in_this_radius = false;
					for (int q = 0; q < myConf->number_of_clusters; q++) 	//for every centroid
					{
						hashResult = 0;		
						for (int i=0; i < k; i++) 
						{      //for evry h
							d_x1_x2 = pow(DistanceMatrixDistance(distanceMatrix, h[o][i][0], h[o][i][1]), 2);        //distance between x1_x2 recalculate       //below distance between centroid and x1, x2
							h_x1_x2_x = (pow(DistanceMatrixDistance(distanceMatrix, centroids[q], h[o][i][0]),2) + pow(DistanceMatrixDistance(distanceMatrix, centroids[q], h[o][i][1]),2) - d_x1_x2)/(2*d_x1_x2);
							//h_x1_x2_x = (pow(point[(int)h[o][i][0]],2) + pow(point[(int)h[o][i][1]],2) - d_x1_x2)/(2*d_x1_x2);
							if ((h_x1_x2_x >= h[o][i][2])&& (h_x1_x2_x <= h[o][i][3]) ) 
							{
								hashResult += pow (2, i);
							}
						}
						//cout << "The hash result : " << hashResult << endl;
						nodePtr = hashTableList[o].getHashTable()[hashResult].getBucket();
						if (nodePtr == NULL)
						{
							//cout << "nodePtr after hashing is NULL";
							//exit(15);
						}
						while (nodePtr != NULL && nodePtr->getFlagAsAssigned() != 1)
						{
							distance_bucketpoint_from_centroid = DistanceMatrixDistance(distanceMatrix, nodePtr->getItemNo(), centroids[q]);
							//cout << "Distance bucket from centroid : " << distance_bucketpoint_from_centroid << endl;
							//cout << "Radius : " << Radius << endl;
							if (distance_bucketpoint_from_centroid <= Radius)       //if inside radius
							{
								// if(point_to_centroid_assignment[nodePtr->getItemNo()] != -1)        //case that current point has been assigned on ANOTHER HASHTABLE but not yet on this one
								// {	
								//cout << "It's better than Radius" << endl;
								if (distance_bucketpoint_from_centroid < nodePtr->getDistanceFromCentroid()) 		//getDistance initialized as INT_MAX
								{
									assigned_in_this_radius = true;
									nodePtr->setSecondBestCentroid(nodePtr->getCentroid());
									nodePtr->setSecondBestDistance(nodePtr->getDistanceFromCentroid());
									nodePtr->setFlagForAssignment(1);
									nodePtr->setCentroid(centroids[q]);
									nodePtr->setDistanceFromCentroid(distance_bucketpoint_from_centroid);
								}
							}
							nodePtr = nodePtr->getNext();
						}

					}
					for (int hash_bucket = 0; hash_bucket < tableSize; hash_bucket++)	//from assign to legit assigned
					{
						nodePtr = hashTableList[o].getHashTable()[hash_bucket].getBucket();
						while(nodePtr != NULL)
						{
							if (nodePtr->getFlagForAssignment() == 1) 
							{

								nodePtr->setFlagAsAssigned(1);
								hashTableList[o].MoveToBack(nodePtr->getItemNo(), hash_bucket);
								if(point_to_centroid_assignment[nodePtr->getItemNo()][0] == -1)
								{
									//cout << "It was -1 (centroid - distance) and now updated to our local array : old :" << point_to_centroid_assignment[nodePtr->getItemNo()][0] << " : " << point_to_centroid_assignment[nodePtr->getItemNo()][1] << endl;
									point_to_centroid_assignment[nodePtr->getItemNo()][0] = nodePtr->getCentroid();
									point_to_centroid_assignment[nodePtr->getItemNo()][1] = nodePtr->getDistanceFromCentroid();
									//cout << "It was -1 (centroid - distance) and now updated to our local array : new :" << point_to_centroid_assignment[nodePtr->getItemNo()][0] << " : " << point_to_centroid_assignment[nodePtr->getItemNo()][1] << endl;
								}
								else 
								{
									//cout << "We have a previous assignment in our local array" <<endl;
									if(point_to_centroid_assignment[nodePtr->getItemNo()][1] > nodePtr->getDistanceFromCentroid())
									{
										//cout << "It had values before (centroid - distance) and now updated to our local array : old " << point_to_centroid_assignment[nodePtr->getItemNo()][0] << " : " << point_to_centroid_assignment[nodePtr->getItemNo()][1] << endl;
										point_to_centroid_assignment[nodePtr->getItemNo()][0] = nodePtr->getCentroid();
										point_to_centroid_assignment[nodePtr->getItemNo()][1] = nodePtr->getDistanceFromCentroid();
										if (point_to_centroid_assignment[nodePtr->getItemNo()][3] > nodePtr->getSecondBestDistance())
										{
											point_to_centroid_assignment[nodePtr->getItemNo()][2] = nodePtr->getSecondBestCentroid();
											point_to_centroid_assignment[nodePtr->getItemNo()][3] = nodePtr->getSecondBestDistance();
										}
										//cout << "It had values before (centroid - distance) and now updated to our local array : new " << point_to_centroid_assignment[nodePtr->getItemNo()][0] << " : " << point_to_centroid_assignment[nodePtr->getItemNo()][1] << endl;
									}
								}
							}
							nodePtr = nodePtr->getNext();
						}
					}
					//cout << "Multiplying Radius" <<endl;
					Radius = Radius * 2;
				}while(assigned_in_this_radius);
			}
			for (int point_iter = 0; point_iter < *dataLength; point_iter++)	//leftover assignment
			{
				minimumDistance = numeric_limits<double>::max() ; 		
				minimumCentroid = -1;
				if (point_to_centroid_assignment[point_iter][0] == -1 || point_to_centroid_assignment[point_iter][1] == -1)
				{
					for (int centroid_iter = 0; centroid_iter < myConf->number_of_clusters; centroid_iter++)
					{
						// distance_bucketpoint_from_centroid = DistanceMatrixDistance(distanceMatrix, nodePtr->getItemNo(), centroids[centroid_iter]);
						distance_bucketpoint_from_centroid = DistanceMatrixDistance(distanceMatrix, point_iter, centroids[centroid_iter]);
						if (distance_bucketpoint_from_centroid < minimumDistance)
						{
							secondBestCentroid = minimumCentroid;
							secondBestDistance = minimumDistance;
							minimumDistance = distance_bucketpoint_from_centroid;
							minimumCentroid = centroids[centroid_iter];
						}
					}
					point_to_centroid_assignment[point_iter][0] = minimumCentroid;
					point_to_centroid_assignment[point_iter][1] = minDistance;
					point_to_centroid_assignment[point_iter][2] = secondBestCentroid;
					point_to_centroid_assignment[point_iter][3] = secondBestDistance;
				}
				//cout << "Leftover updated" << endl;
			}
			//cout << "Exiting..." << endl;
			for (int point_iter = 0; point_iter < *dataLength; point_iter++)		//moving changes to clusterAssign	
			{
				//cout << "changing to best : " << point_to_centroid_assignment[point_iter][0] <<endl;
				clusterAssign[point_iter][2] = point_to_centroid_assignment[point_iter][0];		//give best centroid chosen
				//cout << "changing to 2nd best : " << point_to_centroid_assignment[point_iter][2] <<endl;
				if (point_to_centroid_assignment[point_iter][2] == -1) 
				{
					clusterAssign[point_iter][1] = clusterAssign[point_iter][2];
				}
				else
				{
					clusterAssign[point_iter][1] = point_to_centroid_assignment[point_iter][2];		//give 2nd best centroid
				}
				
			}
			delete[] itemName;
			for (int o = 0; o < L; ++o)
			{
				for (int j = 0; j < k; j++) {
					delete[] h[o][j];
				}
				delete[] h[o];
			}
			delete[] h;
			delete[] h_x1_x2;
			for (int i = 0; i < *dataLength; ++i)
			{
				delete point_to_centroid_assignment[i];
			}
			delete[] point_to_centroid_assignment;
			return;
}