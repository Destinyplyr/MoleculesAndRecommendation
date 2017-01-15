#include "ListData.h"
#include "MathFunctions.h"
#include "DistanceMatrixFunctions.h"

//using namespace std;
//Hash<bitset<64> >* hashTableList
template <typename T>                                                                                           //dataLength is number of points
void ListData<T>::initHammingLSHManagement(Conf* myConf, Metrics* myMetric, ifstream& inputFile, ofstream& outputFile, double** distanceMatrix, int k, int L, int* dataLength, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign, bool complete_printing)
{
	string GARBAGE;
	int hammingSize = 0;     //Used for hamming size, or number of vector attributes
	int itemNo = 0;         //How many items we have?
	int hdis;
	int lshdis;
	int queryCounter = 1;
	int minBruteDistance = 9999;
	int minLSHDistance = 9999;
	//int tableSize = (*dataLength) / 8;			// n/8
	int tableSize = pow(2, k);
	int minimumCentroid = -1;
	int secondBestCentroid = -1;
	double elapsed_secs_lsh, elapsed_secs_brute;
	double elapsed_secs_hashing, elapsed_secs_query;
	double distance_bucketpoint_from_centroid;
	double minimumDistance = INT_MAX;
	double minDistance = INT_MAX;
	double secondBestDistance = INT_MAX;
	double Radius = 0;

	double current_node_point_no = 0;
	double other_node_point_no = 0;

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
	int assigned_in_this_radius = 0;
	clock_t begin, begin_brute, end_brute, end_List_creation;
	clock_t begin_lsh_hashing, end_lsh_hashing;
	clock_t begin_lsh_query, end_lsh_query;
	Node<T>* minimumNode;
	Node<T>* listNode;

	boost::dynamic_bitset<> currentPoint;

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

	hammingSize = *dataLength;
	if (k > hammingSize)
	{
		cout << "Warning: LSH does not support so many mini-hashing functions. Switching to highest number available." << endl;
		k = hammingSize;
	}

	
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
				}//int hashResult, T newItem, int g, int itemno, string itemName, double* ratings
				//cout << "Inserting: hashResult: " << hashResult << " listNode->getKey() : " << listNode->getKey() << " g: " << hashResult << " itemno : " << listNode->getItemNo() << " itemName : " << " ratings: " <<listNode->getRatings() <<endl;
				hashTableList[l].Insert(hashResult, listNode->getKey(), hashResult, listNode->getItemNo(), itemName, listNode->getRatings());
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

	//Radius = FindRadiusForAssignment(myConf, distanceMatrix, centroids);
	Radius = -1;
	//cout << "Radius: " << Radius <<endl;

	double** min_max_thresh = new double*[3];
	for (int i = 0; i < 3; ++i)
	{
		min_max_thresh[i] = new double[2];
		//min_max_thresh[i][1] = -1;
	}
	//min_max_thresh[0][0] = 0;
	//min_max_thresh[2][0] = FindRadiusForAssignment(myConf, distanceMatrix, centroids);

	//Radius = FindNextRadius(min_max_thresh, -1, -1, neighborhood_size);

	Node<T> *nodePtr = NULL;
	Node<T> *otherBucketNodePtr = NULL;
	minimumNode = NULL;

	int neighborhood_size = 35;
	int times_radius_changed = 0;

	double* NN_table = new double[neighborhood_size];

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

	//standard assignment 
	for (int o = 0; o < L; ++o) 	//for every hashtable
	{
		//cout << "For hash table " << o << " : " << endl;
		//REMERMBER TO clear current hashtable off assignments
		hashResult = 0;
		for (int hashResult = 0; hashResult < pow(2, k); hashResult++)		//for every bucket
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
				min_max_thresh[2][0] = 10;//FindRadiusForAssignment(myConf, distanceMatrix, centroids);

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
						/*cout << "NN_table holds : " << assigned_in_this_radius <<endl;
						for (int nn_table_iter = 0; nn_table_iter < assigned_in_this_radius; nn_table_iter++)
						{
							cout << "########################################" << NN_table[nn_table_iter] <<endl;
						}*/

						other_point_no_in_bucket = NN_table[other_bucket_item];
						other_user_ratings = this->ReturnUserRatings(other_point_no_in_bucket, user_rating_table);
						//cout << 1 <<endl;
						if (other_point_no_in_bucket != current_node_point_no)		//if not same item in bucket
						{
							//item_rating_from_other_user = ReturnUserSpecificRating (other_point_no_in_bucket, current_item_rated);
							//below is more efficient
							item_rating_from_other_user = user_rating_table[other_point_no_in_bucket][current_item_rated];
							//cout << 2 <<endl;
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
								//below is more efficient
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

				for (int best_recommendation = myMetric->point_dimension-1; best_recommendation > myMetric->point_dimension-6; best_recommendation-- )
				{
					outputFile << "Best item " << current_ratings[best_recommendation][0] << " - Rating: " << current_ratings[best_recommendation][1] <<endl;
				}

				nodePtr = nodePtr->getNext();
				
			}

		}
		outputFile << "10-fold-cross validation on Hamming LSH" <<endl;
		outputFile << ListData<T>::TenFoldCrossValidation(myMetric, distanceMatrix, user_rating_table, user_general_rating_table) <<endl;
		break;			//not using multiple tables
	}

	for (int i = 0; i < myMetric->point_number; ++i)
	{
		delete[] user_rating_table[i];
	}
	delete[] user_rating_table;

	for(int current_item_rated = 0; current_item_rated < myMetric->point_dimension; current_item_rated++)
	{
		delete[] current_ratings[current_item_rated];
	}
	delete[] current_ratings;

	delete[] user_general_rating_table;

	delete[] NN_table;

	delete[] random_users_for_driver_user;

	for (int i = 0; i < 3; ++i)
	{
		delete[] min_max_thresh[i];
		//min_max_thresh[i][1] = -1;
	}
	delete[] min_max_thresh;
	
	for (int i = 0; i < *dataLength; ++i)
	{
		delete[] point_to_centroid_assignment[i];
	}
	delete[] point_to_centroid_assignment;
	
	for (int i = 0; i < L; ++i)
	{
		delete[] miniHashIndexList[i];
	}
	delete[] miniHashIndexList;
	return;
}