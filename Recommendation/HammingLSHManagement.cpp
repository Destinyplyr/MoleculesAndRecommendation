#include "ListData.h"
#include "MathFunctions.h"
#include "DistanceMatrixFunctions.h"

using namespace std;
//Hash<bitset<64> >* hashTableList
template <typename T>                                                                                           //dataLength is number of points
void ListData<T>::initHammingLSHManagement(Conf* myConf, ifstream& inputFile, double** distanceMatrix, int k, int L, int* dataLength, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign)
{
	int hammingSize = 0;     //Used for hamming size, or number of vector attributes
	int itemNo = 0;         //How many items we have?
	int hdis;
	int lshdis;
	int queryCounter = 1;
	int minBruteDistance = 9999;
	int minLSHDistance = 9999;
	int tableSize = (*dataLength) / 8;			// n/8
	int minimumCentroid = -1;
	int secondBestCentroid = -1;
	double elapsed_secs_lsh, elapsed_secs_brute;
	double elapsed_secs_hashing, elapsed_secs_query;
	double distance_bucketpoint_from_centroid;
	double minimumDistance = INT_MAX;
	double minDistance = INT_MAX;
	double secondBestDistance = INT_MAX;
	double Radius = 0;
	string metric_space;
	string filename;
	string choice;
	string metric;
	string genericStr;
	string itemName;
	string queryCode;
	string myString;
	bool turn = false;
	bool outParameter = false, inParameter = false, queryParameter = false;
	bool assigned_in_this_radius = false;
	clock_t begin, begin_brute, end_brute, end_List_creation;
	clock_t begin_lsh_hashing, end_lsh_hashing;
	clock_t begin_lsh_query, end_lsh_query;
	Node<T>* minimumNode;
	Node<T>* listNode; //=  new Node<string>();

	std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
	std::cout.precision(20);

	double** point_to_centroid_assignment = new double*[*dataLength];           //[0] holds current centroid  [1] holds distance from it [2] holds second best centroid [3]  holds distance from it
	for (int i = 0; i < *dataLength; ++i)
	{
		point_to_centroid_assignment[i] = new double[4];
		point_to_centroid_assignment[i][0] = -1;            //init   
		point_to_centroid_assignment[i][1] = INT_MAX;
		point_to_centroid_assignment[i][2] = -1;
		point_to_centroid_assignment[i][3] = INT_MAX;
	}


	inputFile.clear();      		//Restart
	inputFile.seekg(0, ios::beg);   //Data file back from start


	inputFile >> genericStr;    	//Read "@metric space" 
	inputFile >> genericStr;		//Read etc, "hamming"       
	inputFile >> itemName;	    	//Read itemno
	inputFile >> genericStr;		//Read data size
	hammingSize = genericStr.length();		//dataLength holds hamming size

	if (k > hammingSize)
	{
		cout << "Warning: LSH does not support so many mini-hashing functions. Switching to highest number available." << endl;
		k = hammingSize;
	}

	inputFile.clear();     			//Restart
	inputFile.seekg(0, ios::beg);   //Data file back from start
	inputFile >> genericStr;    	//Read "@metric space"
	inputFile >> metric_space;		//Read etc, "hamming"
	
	begin = clock();

	//HASTABLE CREATION
	//miniHashIndexList is used to store the miniHashIndex for every hashTable
	int** miniHashIndexList = new int*[L];			
	int currentIndex = 0;
	int hashResult = 0;

	int* miniHashIndex;
	for (int l = 0; l < L; l++) 	//every hash table
	{
		miniHashIndex = new int[k];		
		//which mini-hashing functions should I choose?
		for (int i=0; i < k; i++)
		{
			//int r = Μ + (rand() / RAND_MAX + 1.0)*(N - M+1);        
			//generate uniform  [M, N]: we want k numbers from 0 to size of Hamming1-1
			miniHashIndex[i] = (int)(((double)rand() / (double)RAND_MAX)*((double)hammingSize-1));
			//cout << "miniHashIndex[" << i << "]: " << miniHashIndex[i] <<endl;
		}
		miniHashIndexList[l] = miniHashIndex;
	}

	if (*hashCreationDone == 0) {
		for (int l = 0; l < L; l++) 	//every hash table
		{	//new hash table list
			hashTableList[l].initHash(tableSize);
			listNode = getNode();
			while(listNode != NULL)
			{
				hashResult = 0;
				for (int i=0; i < k; i++) 	//for every mini hashing function
				{
				    currentIndex = miniHashIndex[i];        //current index regarding the Hamming string;
				    hashResult += pow (2, i) * (listNode->getKey()[currentIndex] != 0);    //creates the binary as an int
				    //cout << "Inserting in hamming hashtable: curr Hash result: " << hashResult <<endl;
				}
				hashTableList[l].Insert(hashResult, listNode->getKey(), hashResult, itemNo, itemName);
				listNode = listNode->getNext();
			}
		}
		*hashCreationDone = 1;
	}

	end_lsh_hashing = clock();
	elapsed_secs_hashing = (double) (end_lsh_hashing - begin_lsh_hashing) / CLOCKS_PER_SEC;

	for (int o = 0; o < L; ++o) //re-initialize hashTable //for every hash table
    {
    	hashTableList[o].ReInitializeHashTable(L, tableSize);
    }


	Radius = FindRadiusForAssignment(myConf, distanceMatrix, centroids);
	Node<T> *nodePtr = NULL;
	minimumNode = NULL; 
	//standard assignment 
	for (int o = 0; o < L; ++o) 	//for every hashtable
	{
		//cout << "For hash table " << o << " : " << endl;
		//REMERMBER TO clear current hashtable off assignments
		hashResult = 0;
		do
		{
			assigned_in_this_radius = false;
			for (int q = 0; q < myConf->number_of_clusters; q++) 	//for every centroid 
			{
				//cout << "For centroid : " << q << " : " << endl;
				hashResult = 0;


				//hamming hashing for centroid
				for (int i=0; i < k; i++)
				{
					//Current index regarding the Hamming string - using the miniHash that was used before
					currentIndex = miniHashIndexList[o][i];
					nodePtr = this->getNode();
					while (nodePtr->getItemNo() != centroids[q])
					{
						nodePtr = nodePtr->getNext();
					}   
					hashResult += pow (2, i) * (nodePtr->getKey()[currentIndex] != 0);    //Creates the binary as an int
					//cout << "Inserting in hamming hashtable: curr Hash result: " << hashResult <<endl;
				}

				//cout << "The hash result : " << hashResult << endl;
				nodePtr = hashTableList[o].getHashTable()[hashResult].getBucket();
				if (nodePtr == NULL)
				{

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
			for (int hash_bucket = 0; hash_bucket < tableSize; hash_bucket++)	//from assign in hashtable to legit assigned
			{
				//cout << "This hash bucket : " << hash_bucket << endl;
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
				//cout << "Centroid : " << centroid_iter << endl;
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
	for (int point_iter = 0; point_iter < *dataLength; point_iter++)	
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
	
	for (int i = 0; i < *dataLength; ++i)
	{
		delete point_to_centroid_assignment[i];
	}
	delete[] point_to_centroid_assignment;
	
	for (int i = 0; i < L; ++i)
	{
		delete miniHashIndexList[i];
	}
	delete[] miniHashIndexList;
	return;
}