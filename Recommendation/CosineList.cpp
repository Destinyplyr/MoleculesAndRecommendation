#include "Headers.h"
#include "ListData.h"
#include "Hash.h"
#include "MathFunctions.h"

using namespace std;


template <typename T>
void ListData<T>::initCosineList(Conf* myConf, ifstream& inputFile, double** distanceMatrix, int k, int L, int* dataLengthPointNumber, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign)
{
	string genericStr;
	string itemNos;
	string genericQuery;
	string pointStr;
	string metric;
	string GARBAGE;
	string metric_space; 
	string filename;
	string choice;
	Node<double*>* lshENN;
	Node<double*>* realENN;
	double*** v;
	double** t;
	double minOutsideDistance;
	double edis;
	double y_1, y_2, r, ID, phi;
	double Radius = 0;
	double minEBruteDistance= 99999;
	double elapsed_secs_lshe, elapsed_secs_euclidList, elapsed_secs_hashing, end_h_creation;
	double elapsed_secs_ebrute, elapsed_secs_query;
	double distance_bucketpoint_from_centroid;
	double minimumDistance = INT_MAX;
	double minDistance = INT_MAX;
	double secondBestDistance = INT_MAX;
	double*** h;
	int** r_k;
	int* dataLength;                    //dataLength is point dimension here
	int itemNumber = 0;
	int w = 4;
	int inputFileSize = 0;
	int queryCounter = 1;
	int hashResult;
	int assigned_in_this_radius = 0;
	int minimumCentroid = -1;
	int secondBestCentroid = -1;
	clock_t begin, begin_brute, end_ebrute, begin_euclidList;
	clock_t begin_lshe_query, end_lshe_query;
	clock_t end_euclidList, begin_lsh_hashing, end_lsh_hashing;

	Node<T>* nodePtr;
	Node<T>* centroidPtr;

	std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
	std::cout.precision(20);

	dataLength = new int;
	*dataLength = 0;

	double** point_to_centroid_assignment = new double*[*dataLengthPointNumber];           //[0] holds current centroid  [1] holds distance from it [2] holds second best centroid [3]  holds distance from it
	for (int i = 0; i < *dataLengthPointNumber; ++i)
	{
		point_to_centroid_assignment[i] = new double[4];
		point_to_centroid_assignment[i][0] = -1;            //init   
		point_to_centroid_assignment[i][1] = INT_MAX;
		point_to_centroid_assignment[i][2] = -1;
		point_to_centroid_assignment[i][3] = INT_MAX;
	}

	inputFile.clear();              //Restart
	inputFile.seekg(0, ios::beg);   //Data file back from start
	begin = clock();

	inputFile >> metric_space;      //Read "@metric space"      
	inputFile >> metric_space;      //Read "euclidean"
	inputFile >> metric;            //Read etc, "@metric"       
	inputFile >> metric;            //Read euclidean
	inputFile >> itemNos;           //Read itemno

	getline(inputFile, genericStr);
	stringstream linestream(genericStr);
	getline(linestream, pointStr, '\t');
	while (getline(linestream, pointStr, '\t')) {           //Calculate dimension of points
		(*dataLength)++;
	}
	
	h = new double**[L];
	for (int o = 0; o < L; ++o)
	{
		h[o] = new double*[k];
		for (int j = 0; j < k; j++) {
			h[o][j] = new double[*dataLength];
		}
	}

	for (int o = 0; o < L; o++) {		    //for every hash table
		for (int j = 0; j < k; j++) {	    //for every h
			for (int i = 0; i < *dataLength; i++) {		//[-1,1]
				//generate uniform  [M, N]: we want v numbers from -1 to 1
				r = ((double)rand() / (double)RAND_MAX);		//radius smaller than 1
				//cout <<  " R  : " << r << endl;
				y_1 = -1 + ((double)rand() / (double)RAND_MAX)*(2);
				//cout <<  " y_1  : " << y_1 << endl;
				y_2 = sqrt(abs((int)(r - pow(y_1, 2))));			    // r = y_1^2 + y_2^2
				//cout <<  " y_2  : " << y_2 << endl;
				h[o][j][i] = y_1 * sqrt((-2 * log(r))/r);       //every coordinate follows a N(0,1) distribution
				//cout << "voji = " << v[o][j][i] <<endl;
			}
			//t[o][j] = 0 + ((double)rand() / (double)RAND_MAX)*(w);	//[0,w)
			//r_k[o][j]  = rand();
			//cout << "r_k : " << r_k[o][j] << endl;
		}
	}
	end_h_creation = clock();

	if (k > *dataLength) 
	{
		cout << "Warning: LSH does not support so many mini-hashing functions. Switching to highest number available" << endl;
		k = *dataLength;
	}

	end_euclidList = clock();
	elapsed_secs_euclidList = (double) (end_euclidList - begin_euclidList) / CLOCKS_PER_SEC;

	begin_lsh_hashing = clock();
	long long  tableSize = (*dataLengthPointNumber) / 8;        //n/8 for LSH
	//cout << "tableSize :" << tableSize << endl;
	long long M = pow(2, 32) - 5;
	//cout << "M :" << M << endl;
	//Hash<double*>* hashTableList = new Hash<double*>[L];

	if (*hashCreationDone == 0)     //create the hashtable
	{
		for (int o = 0; o < L; ++o)
		{
			hashTableList[o].initHash(tableSize);
		}
		nodePtr = this->getNode();
		while (nodePtr != NULL) {               //For every node in the euclidList
			for (int o = 0; o < L; ++o){        //For every hashtable
				//ID = 0;
				for (int j = 0; j < k; ++j)     //For every h
				{
					//cout << "dot product :key:" << nodePtr->getKey()[0] << " voj: " << v[o][j][0] <<endl;
					//cout << "toj: " << t[o][j] <<endl;
					if (dot_product(nodePtr->getKey(), h[o][j], *dataLength) >= 0) 
					{
						hashResult += pow (2, j);
					}
					//h =  (int)floor((dot_product(nodePtr->getKey(), v[o][j], *dataLength) + t[o][j]) / w);
					//cout << "h :" << h << endl;
					//ID += (r_k[o][j] * h) % M;
					//cout << "ID :" << ID << endl;
				}
				//phi = abs((long)ID % tableSize);
				//cout << "phi :" << phi << endl;
				hashResult = hashResult % tableSize;
				//cout << "nodenodePtr->getItemNo() in Insert " << nodePtr->getItemNo() <<endl;
				hashTableList[o].Insert(hashResult, nodePtr->getKey(), hashResult, nodePtr->getItemNo(), nodePtr->getItemName());
			}
			nodePtr = nodePtr->getNext();
		}
	}

	nodePtr = this->getNode();

	end_lsh_hashing = clock();
	elapsed_secs_hashing = (double) (end_lsh_hashing - begin_lsh_hashing) / CLOCKS_PER_SEC;


	for (int o = 0; o < L; ++o) //re-initialize hashTable //for every hash table
	{
		hashTableList[o].ReInitializeHashTable(L, tableSize);
	}

	Radius = FindRadiusForAssignment(myConf, distanceMatrix, centroids);


	TrickList<double*>* trickList = new TrickList<double*>();       //The first item of the TrickList is the info head

	for (int o = 0; o < L; ++o)     //for every hashtable
	{
		//REMERMBER TO clear current hashtable off assignments
		hashResult = 0;
		do
		{
			assigned_in_this_radius = false;
			for (int q = 0; q < myConf->number_of_clusters; q++)    //for every centroid 
			{
				hashResult = 0;

				nodePtr = this->getNode();      //nodePtr holds current centroid
				while(nodePtr->getItemNo() != centroids[q])
				{
					nodePtr = nodePtr->getNext();
				}
				//euclidean hashing for centroid
				for (int j = 0; j < k; ++j)     //For every h
				{
					if (dot_product(nodePtr->getKey(), h[o][j], *dataLength) >= 0) {
						hashResult += pow (2, j);
					}
				}

				//cout << "The hash result : " << hashResult << endl;

				centroidPtr = nodePtr;      //centroidPtr now holds the centroid
				nodePtr = hashTableList[o].getHashTable()[hashResult].getBucket();        //nodePtr holds bucket items now
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
						if (distance_bucketpoint_from_centroid < nodePtr->getDistanceFromCentroid())        //getDistance initialized as INT_MAX
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
			for (int hash_bucket = 0; hash_bucket < tableSize; hash_bucket++)   //from assign in hashtable to legit assigned
			{
				nodePtr = hashTableList[o].getHashTable()[hash_bucket].getBucket();     //nodePtr now holds current bucket
				while(nodePtr != NULL)
				{
					if (nodePtr->getFlagForAssignment() == 1) 
					{

						nodePtr->setFlagAsAssigned(1);
						hashTableList[o].MoveToBack(nodePtr->getItemNo(), hash_bucket);
						//cout << "nodePtr->getItemNo() : " << nodePtr->getItemNo() << endl;
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
	for (int point_iter = 0; point_iter < *dataLengthPointNumber; point_iter++) //leftover assignment
	{
		//cout << "Leftover : " << point_iter << endl;
		minimumDistance = numeric_limits<double>::max() ;       //make it DOUBLE
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
	for (int point_iter = 0; point_iter < *dataLengthPointNumber; point_iter++) 
	{
		//cout << "changing to best : " << point_to_centroid_assignment[point_iter][0] <<endl;
		clusterAssign[point_iter][2] = point_to_centroid_assignment[point_iter][0];     //give best centroid chosen
		//cout << "changing to 2nd best : " << point_to_centroid_assignment[point_iter][2] <<endl;
		if (point_to_centroid_assignment[point_iter][2] == -1) 
		{
			clusterAssign[point_iter][1] = clusterAssign[point_iter][2];
		}
		else
		{
			clusterAssign[point_iter][1] = point_to_centroid_assignment[point_iter][2];     //give 2nd best centroid
		}
		
	}
	for (int o = 0; o < L; ++o)
	{
		for (int j = 0; j < k; j++) {
			delete[] h[o][j];
		}
		delete[] h[o];
	}
	delete h;
	
	delete dataLength;

	for (int i = 0; i < *dataLength; ++i)
	{
		delete point_to_centroid_assignment[i];
	}
	delete[] point_to_centroid_assignment;
	return;
}