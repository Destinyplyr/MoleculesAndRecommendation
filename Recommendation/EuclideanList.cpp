#include "ListData.h"
#include "DistanceMatrixFunctions.h"
#include "TrickFunctions.cpp"

using namespace std;


template <typename T>																						//dataLength is point dimension
void ListData<T>::initEuclideanList(Conf* myConf, Metrics* myMetric, ifstream& inputFile, ofstream& outputFile, double** distanceMatrix, int k, int L, int* dataLength, int* dataLengthPointNumber, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign, bool complete_printing) 
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
	int** r_k;
	//int* dataLength;					//dataLength is point dimension here
	int itemNumber = 0;
	int h;
	int w = 4;
	int inputFileSize = 0;
	int queryCounter = 1;
	int hashResult;
	//int assigned_in_this_radius = 0;
	int minimumCentroid = -1;
	int secondBestCentroid = -1;
	clock_t begin, begin_brute, end_ebrute, begin_euclidList;
	clock_t begin_lshe_query, end_lshe_query;
	clock_t end_euclidList, begin_lsh_hashing, end_lsh_hashing;

	Node<T>* nodePtr = NULL;
	Node<T>* centroidPtr;

	std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
	std::cout.precision(20);


	double** point_to_centroid_assignment = new double*[*dataLengthPointNumber];           //[0] holds current centroid  [1] holds distance from it [2] holds second best centroid [3]  holds distance from it
	for (int i = 0; i < *dataLengthPointNumber; ++i)
	{
		point_to_centroid_assignment[i] = new double[4];
		point_to_centroid_assignment[i][0] = -1;            //init   
		point_to_centroid_assignment[i][1] = INT_MAX;
		point_to_centroid_assignment[i][2] = -1;
		point_to_centroid_assignment[i][3] = INT_MAX;
	}

	begin = clock();

	v = new double**[L];
	t = new double*[L];
	r_k = new int*[L];
	for (int o = 0; o < L; ++o)
	{
		v[o] = new double*[k];
		t[o] = new double[k];
		r_k[o] = new int[k];
		for (int j = 0; j < k; j++) {
			v[o][j] = new double[*dataLength];
		}
	}

	for (int o = 0; o < L; o++) {		//For every hash table
		for (int j = 0; j < k; j++) {	//For every h
   		for (int i = 0; i < *dataLength; i++) {		//[-1,1]
   			//int r = M + (rand() / RAND_MAX + 1.0)*(N - M+1);        //generate uniform  [M, N]: we want v numbers from -1 to 1
   			r = ((double)rand() / (double)RAND_MAX);		//Radius smaller than 1
   			//cout <<  " R  : " << r << endl;
   			y_1 = -1 + ((double)rand() / (double)RAND_MAX)*(2);
   			//cout <<  " y_1  : " << y_1 << endl;
   			y_2 = sqrt(abs((int)(r - pow(y_1, 2))));			// r = y_1^2 + y_2^2
   			//cout <<  " y_2  : " << y_2 << endl;
   			v[o][j][i] = y_1 * sqrt((-2 * log(r))/r);	//Every coordinate follows a N(0,1) distribution
   			//cout << "voji = " << v[o][j][i] <<endl;
   		}
   		t[o][j] = 0 + ((double)rand() / (double)RAND_MAX)*(w);	//[0,w)
   		r_k[o][j]  = rand();
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
	long long  tableSize = (*dataLengthPointNumber) / 8;		//n/8 for LSH
	//cout << "tableSize :" << tableSize << endl;
	long long M = pow(2, 32) - 5;
	//cout << "M :" << M << endl;

	if (*hashCreationDone == 0)		//create the hashtable if it wasn't created (assign - update cycle)
	{
		for (int o = 0; o < L; ++o)
		{
			hashTableList[o].initHash(tableSize);
		}
		nodePtr = this->getNode();
		while (nodePtr != NULL) {				//For every node in the euclidList
			for (int o = 0; o < L; ++o){		//For every hashtable
				ID = 0;
				for (int j = 0; j < k; ++j)		//For every h
				{
					//cout << "dot product :key:" << nodePtr->getKey()[0] << " voj: " << v[o][j][0] <<endl;
					//cout << "toj: " << t[o][j] <<endl;
 					h =  (int)floor((dot_product(nodePtr->getKey(), v[o][j], *dataLength) + t[o][j]) / w);
					//cout << "h :" << h << endl;
					ID += (r_k[o][j] * h) % M;
					//cout << "ID :" << ID << endl;
				}
				phi = abs((long)ID % tableSize);
				//cout << "phi :" << phi << endl;
				hashTableList[o].Insert(phi, nodePtr->getKey(), ID, nodePtr->getItemNo(), nodePtr->getItemName(), nodePtr->getRatings());
			}
				nodePtr = nodePtr->getNext();
		}
	}


	end_lsh_hashing = clock();
	elapsed_secs_hashing = (double) (end_lsh_hashing - begin_lsh_hashing) / CLOCKS_PER_SEC;


	for (int o = 0; o < L; ++o) //re-initialize hashTable //for every hash table
    {
    	hashTableList[o].ReInitializeHashTable(L, tableSize);
    }


    Radius = -1;

    double** min_max_thresh = new double*[3];
    for (int i = 0; i < 3; ++i)
    {
    	min_max_thresh[i] = new double[2];
    	//min_max_thresh[i][1] = -1;
    }

    //Node<T> *nodePtr = NULL;
    Node<T> *otherBucketNodePtr = NULL;
    //minimumNode = NULL;

    int assigned_in_this_radius = 0;
    int neighborhood_size = 35;
    int times_radius_changed = 0;

    double* NN_table = new double[neighborhood_size];

    double current_node_point_no = 0;
    double other_node_point_no = 0;
    double current_user_general_rating = 0;
    double item_rating_from_other_user = 0;
    double similarity_sum = 0;
    double normalizing_factor = 0;
    double R_u_i = 0;
    double added_similarity = 0;

    int other_point_no_in_bucket = -1;

    double* driver_user_ratings = NULL;
    double* other_user_ratings = NULL;

    double* user_general_rating_table = NULL;
    double** user_rating_table = NULL;

    int* all_user_table = NULL;

    int* random_users_for_driver_user = NULL;
    random_users_for_driver_user = new int[neighborhood_size];

    double** current_ratings = new double*[myMetric->point_dimension];	//current_ratings[0] holds item_no [1] holds its rating
    for(int current_item_rated = 0; current_item_rated < myMetric->point_dimension; current_item_rated++)
    {
    	current_ratings[current_item_rated] = new double[2];
    	current_ratings[current_item_rated][0] = current_item_rated;
    }

    user_rating_table = ReturnUserRatingTable(myMetric);
    user_general_rating_table = ReturnUserGeneralRatingTable(myMetric);

	for (int o = 0; o < L; ++o) 	//for every hashtable
	{
		hashResult = 0;
		for (int hashResult = 0; hashResult < tableSize; hashResult++)		//for every bucket
		{
			//cout << "for bucket " << hashResult <<endl;
			nodePtr = hashTableList[o].getHashTable()[hashResult].getBucket();
			while(nodePtr != NULL)		//for every bucket item
			{
				//cout << "for user " << nodePtr->getItemNo() <<endl;
				current_node_point_no = nodePtr->getItemNo();		//this point
				times_radius_changed = 0;
				

				for(int current_item_rated = 0; current_item_rated < myMetric->point_dimension; current_item_rated++)
				{
					current_ratings[current_item_rated][1] = 0;
				}

				current_user_general_rating = this->ReturnUserGeneralRating(myMetric, current_node_point_no);

				for (int i = 0; i < 3; ++i)
				{
					min_max_thresh[i][1] = -1;
				}
				min_max_thresh[0][0] = 0;
				min_max_thresh[2][0] = 100; //FindRadiusForAssignment(myConf, distanceMatrix, centroids);

				Radius = FindNextRadius(min_max_thresh, -1, -1, neighborhood_size);
				//cout << "for Radius : " << Radius <<endl;

				do 		//do until we have as many points as we want or we have changed radius a number of times
				{
					assigned_in_this_radius = 0;
					otherBucketNodePtr = hashTableList[o].getHashTable()[hashResult].getBucket();
					while (otherBucketNodePtr != NULL)		//take all other items of the bucket
					{
						other_node_point_no = otherBucketNodePtr->getItemNo();
						if (otherBucketNodePtr == nodePtr)
						{
							otherBucketNodePtr = otherBucketNodePtr->getNext();
							continue;
						}
						else 
						{		//its inside the radius
							//cout << "current_node_point_no : " << current_node_point_no << " other_node_point_no : " << endl;
							if (DistanceMatrixDistance(distanceMatrix, current_node_point_no, other_node_point_no) <= Radius)
							{
								assigned_in_this_radius++;
								if (assigned_in_this_radius == neighborhood_size + 1)
								{
									break;
								}
								NN_table[assigned_in_this_radius-1] = other_node_point_no;
							}
						}
						otherBucketNodePtr = otherBucketNodePtr->getNext();
					}
					if (assigned_in_this_radius != neighborhood_size)		//more or less from neighborhood size
					{
						Radius = FindNextRadius(min_max_thresh, Radius, assigned_in_this_radius, neighborhood_size);
						//cout << "for Radius : " << Radius <<endl;
						if (Radius == -1)
						{
							break;
						}
						times_radius_changed++;
					}
					else
					{
						break;
					}
				}while(assigned_in_this_radius != neighborhood_size && times_radius_changed < 5);
				
				/*cout << "We found for user " << nodePtr->getItemNo() <<endl;
				for (int neighbor_no = 0; neighbor_no < neighborhood_size; neighbor_no++)
				{
					cout << "##user " << NN_table[neighbor_no] <<endl;
				}*/

				//RATING FROM OTHER USERS SEGMENT

				if (assigned_in_this_radius == neighborhood_size + 1)
				{
					assigned_in_this_radius = neighborhood_size;
				}

				for(int current_item_rated = 0; current_item_rated < myMetric->point_dimension; current_item_rated++)
				{
					similarity_sum = 0;
					normalizing_factor = 1;
					other_point_no_in_bucket = -1;
					for(int other_bucket_item = 0; other_bucket_item < assigned_in_this_radius; other_bucket_item++)
					{
						other_point_no_in_bucket = NN_table[other_bucket_item];
						other_user_ratings = this->ReturnUserRatings(other_point_no_in_bucket, user_rating_table);
						if (other_point_no_in_bucket != current_node_point_no)		//if not same item in bucket
						{
							//item_rating_from_other_user = ReturnUserSpecificRating (other_point_no_in_bucket, current_item_rated);
							//below more efficient
							item_rating_from_other_user = user_rating_table[other_point_no_in_bucket][current_item_rated];
							if (item_rating_from_other_user != 0)
							{
								if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
								{
									added_similarity = this->SimilarityHamming(myMetric, distanceMatrix, current_node_point_no, other_point_no_in_bucket);
								}
								if (strcmp(myMetric->metric_space.c_str(), "vector") == 0)
								{
									if (strcmp(myMetric->metric.c_str(), "euclidean") == 0)
									{
										added_similarity = this->SimilarityEuclidean(myMetric, distanceMatrix, current_node_point_no, other_point_no_in_bucket);
									}
									else if (strcmp(myMetric->metric.c_str(), "cosine") == 0)
									{
										added_similarity = this->SimilarityCosine(myMetric, driver_user_ratings, other_user_ratings);
									}

								}
								//similarity_sum += ( this->SimilarityHamming(myMetric, distance_matrix, current_node_point_no, other_point_no_in_bucket)) * (item_rating_from_other_user - this->ReturnUserGeneralRating(myMetric, other_point_no_in_bucket)) ;
								// similarity_sum += (added_similarity) * (item_rating_from_other_user - this->ReturnUserGeneralRating(myMetric, other_point_no_in_bucket)) ;
								//below more efficient
								similarity_sum += (added_similarity) * (item_rating_from_other_user - user_general_rating_table[other_point_no_in_bucket]);
								normalizing_factor += added_similarity;
							}
						}
					}
					if (normalizing_factor == 0) 
					{
						normalizing_factor = 1;		//but similarity sum is 0 so it will go to 0
					}
					//R(u,i) = R(u) + (1/(Σsim(u,v))) * (Σ(sum(u,v) *(R(v,i) - R(v)))
					// same as
					//R(u,i) = R(u) + (1/normalizing_factor) * (Σ(sum(u,v) *(R(v,i) - R(v)))
					current_ratings[current_item_rated][1] = current_user_general_rating + ((1/normalizing_factor) * (similarity_sum));
				}

				quickSort_twolist(current_ratings, 0, myMetric->point_dimension-1);

				outputFile << "We found for user " << nodePtr->getItemNo() <<endl;
				outputFile << "R(u) : " <<current_user_general_rating <<endl;

				for (int best_recommendation = myMetric->point_dimension-1; best_recommendation > myMetric->point_dimension-6; best_recommendation-- )
				{
					outputFile << "Best item " << current_ratings[best_recommendation][0] << " - Rating: " << current_ratings[best_recommendation][1] <<endl;
				}

				nodePtr = nodePtr->getNext();
				
			}

		}
		if (complete_printing == true)
		{
			outputFile << "10-fold-cross validation on Euclidean LSH" <<endl;
			outputFile << ListData<T>::TenFoldCrossValidation(myMetric, distanceMatrix, user_rating_table, user_general_rating_table) <<endl;
		}
		break;			//not using multiple tables
	}


	for (int o = 0; o < L; ++o)
	{
		for (int j = 0; j < k; j++) {
			delete[] v[o][j];
		}
		delete[] v[o];
		delete[] t[o];
		delete[] r_k[o];

	}
	delete[] v;
	delete[] t;
	delete[] r_k;

	/*v = new double***[L];
	t = new double**[L];
	r_k = new int**[L];*/

	delete[] user_general_rating_table;

	for (int i = 0; i < myMetric->point_number; ++i)
	{
		delete[] user_rating_table[i];
	}
	delete[] user_rating_table;


	delete[] random_users_for_driver_user;

	delete[] NN_table;



	for (int i = 0; i < 3; ++i)
	{
		delete[] min_max_thresh[i];
		//min_max_thresh[i][1] = -1;
	}
	delete[] min_max_thresh;

	for (int i = 0; i < *dataLengthPointNumber; ++i)
	{
		delete[] point_to_centroid_assignment[i];
	}
	delete[] point_to_centroid_assignment;

	return;

}