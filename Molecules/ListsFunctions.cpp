#include "Headers.h"
#include "ListData.h"

template <typename T>
ListData<T>::ListData()
{
	header = NULL;
	//cout << "------->  ListData initialized successfully!" << endl << endl;
}


template <typename T>
ListData<T>::~ListData()
{
	if (header != NULL)
	{
		delete header;
		header = NULL;
		//cout << "------->  Finished processing the ListData." << endl;
	}
	else
	{
		//cout << "------->  Finished processing the ListData." << endl;
	}
}


template <typename T, size_t N>
inline
size_t sizeOfArray( const T(&)[ N ] )
{
	return N;
}


template <typename T>
Node<T>* ListData<T>::getNode()
{
	return header;
}


template <typename T>
void ListData<T>::PrintData()
{
	Node<T>* node;
	node = header;
	int counter = 1;
	while (node->getNext() != NULL)
	{
		cout << "Item " << counter << " in list : " << node->getKey()[0] << endl;
		node = node->getNext();
		++counter;
	}

	cout << "Item " << counter << " in list : " << node->getKey()[0] << endl;
}


template <typename T>
void ListData<T>::Insert(T newItem, int itemno, string itemName, double** ratings)
{
 	Node<T>* node = new Node<T>(newItem, 0, itemno, itemName, ratings);
	if (header == NULL)
    {
		header = node;
    }
	else
	{
		Node<T>* tail = header;
		while (tail->getNext() != NULL)
		{
			tail = tail->getNext();
		}
		tail->setNext(node);
	}

	//cout << "Item inserted" << endl;
}

template <typename T>
int ListData<T>::Distance(T item1, T item2)
{
	int distance = 0;
	int i = 0, j = 0;

	while ((i < item1.size()) && (j < item2.size()))
	{
		if (item1[i] == item2[j])
		{
			//cout << "Same bit" << endl;
			++i;
			++j;
		}
		else
		{
			//cout << "Different bit" << endl;
			++distance;
			++i;
			++j;
		}
	}

	return distance;
}


template <typename T>
double ListData<T>::DistanceB2BHamming(T item1, T item2)
{
	return (item1 ^= item2).count();
}


template <typename T>
double ListData<T>::CosineDistance(double item1[], double item2[], int size)      //Cosine
{
	double norm1, norm2;
	double cosine;

	norm1 = euclid_norm(item1, size);
	norm2 = euclid_norm(item2, size);
    cosine = dot_product(item1, item2, size) / (norm1 * norm2);

    return (1 - cosine);
}


template <typename T>
Node<T>* ListData<T>::ReturnHead() {
    return header;
}


template <typename T>
bool ListData<T>::HammingDuplicate(T item) {
	Node<T>* currentNode = header;
	while(currentNode != NULL) {
		if (item.compare(currentNode->getKey()) == 0) {
			return true;
		}
		currentNode = currentNode->getNext();
	}
	return false;
}

template <typename T>
bool ListData<T>::HammingB2BDuplicate(T item) {
	Node<T>* currentNode = header;
	while(currentNode != NULL) {
		if ((item ^= currentNode->getKey()) == 0) {
			return true;
		}
		currentNode = currentNode->getNext();
	}
	return false;
}


template <typename T>
bool ListData<T>::EuclideanDuplicate(T item, int size) {
	Node<T>* currentNode = header;
	while(currentNode != NULL) {
		if (SameArray(currentNode->getKey(), item, size)) {
			return true;
		}
		currentNode = currentNode->getNext();
	}
	return false;
}

template <typename T>
bool ListData<T>::ExistsOnce(int itemno, Node<T>* header_out)
{
	Node<T>* currentNode = header_out;
	//cout << endl;
	while (currentNode != NULL)
	{
		// cout << "once - current node key: " << currentNode->getKey() << endl;
		// cout << "once -current node itemno: " << currentNode->getItemNo() << endl;
		if (currentNode->getItemNo() == itemno) 
		{
			if (Exists(itemno, currentNode->getNext()) == false) 
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
		currentNode = currentNode->getNext();
	}
	return false;
}

template <typename T>
bool ListData<T>::Exists(int itemno, Node<T>* header_out)
{
	Node<T>* currentNode = header_out;
	while (currentNode != NULL)
	{
		// cout << "simple - current node key: " << currentNode->getKey() << endl;
		// cout << "simple -current node itemno: " << currentNode->getItemNo() << endl;
		// cout << "simple: " << currentNode->getItemNo() << " " << itemno <<endl;
		if (currentNode->getItemNo() == itemno) 
		{
			return true;
		}
		currentNode = currentNode->getNext();
	}
	return false;
}

template <typename T>
string* ListData<T>::ItemNamesFromItemNos(int* items_in_cluster_itemNo, int size_of_cluster)
{
	Node<T>* currentNode;
	string* items_in_cluster_itemName = new string[size_of_cluster+1];
	for (int items_in_cluster_iter = 0; items_in_cluster_iter <= size_of_cluster; items_in_cluster_iter++)
	{
		currentNode = this->ReturnHead();
		while (currentNode != NULL) 
		{
			//cout << "currentNode->getItemNo()" << currentNode->getItemNo() << " items_in_cluster_itemNo[items_in_cluster_iter] " <<items_in_cluster_itemNo[items_in_cluster_iter] <<endl; 
			if (currentNode->getItemNo() == items_in_cluster_itemNo[items_in_cluster_iter])
			{
				//cout << "problem in if" <<endl;
				//cout << "currentNode->getItemName() " << currentNode->getItemName() <<endl;
				items_in_cluster_itemName[items_in_cluster_iter] = currentNode->getItemName();
			}
			currentNode = currentNode->getNext();
			//cout << "stuck on you" <<endl;
		}
		//cout << "and managed to get out" <<endl;
	}
	return items_in_cluster_itemName;
}

template <typename T>
void ListData<T>::Printer(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, bool complete_printing)
{
	int total;
	string GARBAGE;
	int point_number, size_of_cluster;
	int* items_in_cluster_itemNo;
	string*	items_in_cluster_itemName;
	bool first_time_lsh = 1; 	
	int hashCreationDone = 0;
	clock_t KMPP_start, KMPP_finish;
	clock_t Concentrate_start, Concentrate_finish;
	double Initialization_elapsed;
	clock_t Assign_Update_start, Assign_Update_finish;
	double Assign_Update_elapsed;
	clock_t CLARA_start, CLARA_finish;
	double CLARA_elapsed;

	for(int init_iter = 0; init_iter < 2; init_iter++)
	{
		if (init_iter == 0) //case KMedoids++
		{
			KMPP_start = clock();
			KMedoidsPP(myConf, myMetric, distance_matrix, centroids);
			KMPP_finish = clock();
			Initialization_elapsed = (double)(KMPP_finish - KMPP_start)/CLOCKS_PER_SEC;
		}
		else if (init_iter == 1)//case Concentrate
		{
			Concentrate_start = clock();
			Concentrate(myConf, myMetric, distance_matrix, centroids);
			Concentrate_finish = clock();
			Initialization_elapsed = (double)(Concentrate_finish - Concentrate_start)/CLOCKS_PER_SEC;
		}
		/*cout << "==================" << endl << "PRINTING CLUSTERS IN mainSample BEFORE CLARANS : " <<endl;
		for (int w = 0; w <myConf->number_of_clusters; w++) {
			cout << centroids[w] << " ";
		}*/


		for (int assign_iter = 0; assign_iter < 2; assign_iter++)
		{
			if (assign_iter == 0)		//case PAM
			{
				delete clusterTable;
				clusterTable = new ClusterTable(myConf->number_of_clusters);
				//cout << "starting pam - alaloyds" <<endl;
				Assign_Update_start = clock();
				for (int assign_update_times =0; assign_update_times < 5; assign_update_times++)
				{
					PAM(myConf, myMetric, distance_matrix, centroids, clusterTable, clusterAssign);		//assignment
					if (!ALaLoyds(myConf, myMetric, distance_matrix, centroids, clusterTable, clusterAssign)) {		//update
					 	//cout << "done!" << endl;
					}	
				}
				Assign_Update_finish = clock();
				Assign_Update_elapsed = (double)(Assign_Update_finish - Assign_Update_start)/CLOCKS_PER_SEC;
				/*for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
				{
					clusterTable->PrintClusterNo(cluster_iter);
				}*/

				//PRINTING SEGMENT
				outputFile << "Algorithm: I" << init_iter+1 << "x" << assign_iter+1 << "x" << 1 <<endl;
				for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
				{
					clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);		//used to get size
					outputFile << "CLUSTER-" << cluster_iter << " {"<<size_of_cluster << ", " << clusterTable->ClusterDistance( myMetric, distance_matrix, cluster_iter, clusterAssign) <<"}" <<endl;
				}
				outputFile << "Clustering Time: " << Assign_Update_elapsed + Initialization_elapsed <<endl;
				clusterTable->PrintingSilhouette(outputFile, myConf, distance_matrix, centroids, clusterAssign);		//printing Silhouette

				if (complete_printing == 1)
				{
					for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
					{
						//ClusterTable::PrintClusterDataForList(int cluster_no int* size_of_cluster)
						items_in_cluster_itemNo = clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);
						if (items_in_cluster_itemNo == NULL)
						{
							continue;
						}
						items_in_cluster_itemName = ItemNamesFromItemNos(items_in_cluster_itemNo, size_of_cluster);
						clusterTable->PrintClusterUsingNames(outputFile, items_in_cluster_itemName, size_of_cluster, cluster_iter);
						delete[] items_in_cluster_itemNo;		//initialized inside PrintClusterDataForList
						delete[] items_in_cluster_itemName;		//initilaized inside ItemNamesFromItemNos
					}
				}
				//cout << "finished pam - alaloyds" <<endl;

			}
			else if (assign_iter == 1)	//case reverse LSH
			{
				//cout << "starting lsh - alaloyds" <<endl;
				Hash<T>* hashTableList = new Hash<T>[L]();      //Na min orizetai se kathe iteration tou update, giati xanaorizetai
				Assign_Update_start = clock();
				for (int assign_update_times =0; assign_update_times < 5; assign_update_times++)
				{
					if (first_time_lsh == true) 
					{
						first_time_lsh = false;
						hashCreationDone = 0;
						point_number = myMetric->point_number;							//assignment
						if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
						{
							initHammingLSHManagement(myConf, inputFile, distance_matrix, k , L, &(point_number), &hashCreationDone, hashTableList, centroids, clusterAssign);
						}
						if (strcmp(myMetric->metric_space.c_str(), "vector") == 0)
						{
							if (strcmp(myMetric->metric.c_str(), "euclidean") == 0)
							{
								initEuclideanList(myConf, inputFile, distance_matrix, k , L, &(point_number), &hashCreationDone, hashTableList, centroids, clusterAssign);
							}
							
							if (strcmp(myMetric->metric.c_str(), "cosine") == 0)
							{
								initCosineList(myConf, inputFile, distance_matrix, k , L, &(point_number), &hashCreationDone, hashTableList, centroids, clusterAssign);
							}
						}
						if (strcmp(myMetric->metric_space.c_str(), "matrix") == 0)
						{
							initDBHManagement(myConf, inputFile, distance_matrix, k , L, &(point_number), &hashCreationDone, hashTableList, centroids, clusterAssign);
						}
					}
					delete clusterTable;
					clusterTable = new ClusterTable(myConf->number_of_clusters);
					clusterTable->CreateClusterTableFromClusterAssign(myConf, myMetric,clusterAssign, centroids);
					if (!ALaLoyds(myConf, myMetric, distance_matrix, centroids, clusterTable, clusterAssign)) {		//update
					 	//cout << "done!" << endl;
					}
				}
				Assign_Update_finish = clock();
				Assign_Update_elapsed = (double)(Assign_Update_finish - Assign_Update_start)/CLOCKS_PER_SEC;

				//PRINTING SEGMENT
				outputFile << "Algorithm: I" << init_iter+1 << "x" << assign_iter+1 << "x" << 1 <<endl;
				for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
				{
					clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);		//used to get size
					outputFile << "CLUSTER-" << cluster_iter << " {"<<size_of_cluster << ", " << clusterTable->ClusterDistance( myMetric, distance_matrix, cluster_iter, clusterAssign) <<"}" <<endl;
				}
				outputFile << "Clustering Time: " << Assign_Update_elapsed + Initialization_elapsed <<endl;
				clusterTable->PrintingSilhouette(outputFile, myConf, distance_matrix, centroids, clusterAssign);		//printing Silhouette

				if (complete_printing == 1)
				{
					for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
					{
						items_in_cluster_itemNo = clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);
						if (items_in_cluster_itemNo == NULL)
						{
							continue;
						}
						items_in_cluster_itemName = ItemNamesFromItemNos(items_in_cluster_itemNo, size_of_cluster);
						//cout << "PrintClusterUsingNames" << endl;
						clusterTable->PrintClusterUsingNames(outputFile, items_in_cluster_itemName, size_of_cluster, cluster_iter);
						delete[] items_in_cluster_itemNo;		//initialized inside PrintClusterDataForList
						delete[] items_in_cluster_itemName;		//initilaized inside ItemNamesFromItemNos
					}
				}
/*				for (int delete_iter = 0; delete_iter < L; delete_iter++)
				{
					delete hashTableList[delete_iter];
				}*/
				delete[] hashTableList;
				//cout << "finished lsh - alaloyds" <<endl;
			}
			
		}
		Assign_Update_start = clock();
		for (int i = 0; i < myMetric->point_number; ++i)
		{
			delete clusterAssign[i];
		}
		delete[] clusterAssign;
		clusterAssign= new int*[myMetric->point_number];
		for (int i = 0; i < myMetric->point_number; ++i)
		{
		    clusterAssign[i] = new int[3];
		    clusterAssign[i][0] = -1;
		    clusterAssign[i][1] = -1;
		    clusterAssign[i][2] = -1;
		}

		delete clusterTable;
		clusterTable = new ClusterTable(myConf->number_of_clusters);
		CLARANS(myConf, myMetric, distance_matrix, centroids, clusterTable, clusterAssign);		//full CLARANS algorithm uses only PAM
		Assign_Update_finish = clock();
		Assign_Update_elapsed = (double)(Assign_Update_finish - Assign_Update_start)/CLOCKS_PER_SEC;

		outputFile << "Algorithm: I" << init_iter+1 << "x" << 1 << "x" << 2 <<endl;
		for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
		{
			clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);		//used to get size
			outputFile << "CLUSTER-" << cluster_iter << " {"<<size_of_cluster << ", " << clusterTable->ClusterDistance( myMetric, distance_matrix, cluster_iter, clusterAssign) <<"}" <<endl;
		}
		outputFile << "Clustering Time: " << Assign_Update_elapsed + Initialization_elapsed <<endl;
		clusterTable->PrintingSilhouette(outputFile, myConf, distance_matrix, centroids, clusterAssign);		//printing Silhouette

		if (complete_printing == 1)
		{
			for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
			{
				items_in_cluster_itemNo = clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);
				if (items_in_cluster_itemNo == NULL)
				{
					continue;
				}
				items_in_cluster_itemName = ItemNamesFromItemNos(items_in_cluster_itemNo, size_of_cluster);
				clusterTable->PrintClusterUsingNames(outputFile, items_in_cluster_itemName, size_of_cluster, cluster_iter);
				delete[] items_in_cluster_itemNo;		//initialized inside PrintClusterDataForList
				delete[] items_in_cluster_itemName;		//initilaized inside ItemNamesFromItemNos
			}
		}
		for (int i = 0; i < myMetric->point_number; ++i)
		{
		    clusterAssign[i][0] = -1;
		    clusterAssign[i][1] = -1;
		    clusterAssign[i][2] = -1;
		}
		//cout << "finished CLARANS" <<endl;
	}
	CLARA_start = clock();
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		delete clusterAssign[i];
	}
	delete[] clusterAssign;
	clusterAssign= new int*[myMetric->point_number];
	for (int i = 0; i < myMetric->point_number; ++i)
	{
	    clusterAssign[i] = new int[3];
	    clusterAssign[i][0] = -1;
	    clusterAssign[i][1] = -1;
	    clusterAssign[i][2] = -1;
	}
	delete clusterTable;
	clusterTable = new ClusterTable(myConf->number_of_clusters);
	//cout << "starting CLARA" <<endl;
	CLARA( myConf,  myMetric, distance_matrix, centroids, clusterTable, clusterAssign);
	CLARA_finish = clock();
	CLARA_elapsed = (double)(CLARA_finish - CLARA_start)/CLOCKS_PER_SEC;
	//PRINTING SEGMENT
	outputFile << "CLARA" <<endl;
	total = 0;
	for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
	{
		clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);		//used to get size
		outputFile << "CLUSTER-" << cluster_iter << " {" << size_of_cluster << ", " << clusterTable->ClusterDistance( myMetric, distance_matrix, cluster_iter, clusterAssign) <<"}" <<endl;
		total += size_of_cluster;
	}
	//outputFile << "total on clusters: " <<total<<endl;
	outputFile << "Clustering Time: " << CLARA_elapsed <<endl;
	clusterTable->PrintingSilhouette(outputFile, myConf, distance_matrix, centroids, clusterAssign);		//printing Silhouette
	if (complete_printing == 1)
	{
		for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
		{
			items_in_cluster_itemNo = clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);
			if (items_in_cluster_itemNo == NULL)
			{
				continue;
			}
			items_in_cluster_itemName = ItemNamesFromItemNos(items_in_cluster_itemNo, size_of_cluster);
			clusterTable->PrintClusterUsingNames(outputFile, items_in_cluster_itemName, size_of_cluster, cluster_iter);
			delete[] items_in_cluster_itemNo;		//initialized inside PrintClusterDataForList
			delete[] items_in_cluster_itemName;		//initilaized inside ItemNamesFromItemNos
		}
	}
	//cout << "finished CLARA" <<endl;
	delete clusterTable;
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		delete clusterAssign[i];
	}
	delete[] clusterAssign;
}

template <typename T>
void ListData<T>::ClusterHandleExercise3(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, bool complete_printing, double* currentData, bool takeData)
{
	int total;
	string GARBAGE;
	int point_number, size_of_cluster;
	int* items_in_cluster_itemNo;
	string*	items_in_cluster_itemName;
	bool first_time_lsh = 1; 	
	int hashCreationDone = 0;
	clock_t total_start, total_finish;
	double total_elapsed;
	clock_t KMPP_start, KMPP_finish;
	clock_t Concentrate_start, Concentrate_finish;
	double Initialization_elapsed;
	clock_t Assign_Update_start, Assign_Update_finish;
	double Assign_Update_elapsed;
	clock_t CLARA_start, CLARA_finish;
	double CLARA_elapsed;
	double best_silh = -INT_MAX;
	double current_silh;
	int best_cluster_num;
	int current_point_no_in_cluster = 0;
	int other_point_no_in_cluster = 0;
	int neighborhood_size = 35;
	double current_user_general_rating = 0;
	double item_rating_from_other_user = 0;
	double similarity_sum = 0;
	double normalizing_factor = 0;
	double R_u_i = 0;
	double added_similarity = 0;

	double* driver_user_ratings = NULL;
	double* other_user_ratings = NULL;


	double** current_ratings = NULL;
	double* user_general_rating_table = NULL;
	double** user_rating_table = NULL;

	int* NN_table = NULL;
	int* all_user_table = NULL;

	int* random_users_for_driver_user = NULL;
	random_users_for_driver_user = new int[neighborhood_size];

	total_start = clock();

	for (int cluster_num = 4; cluster_num < 10; cluster_num++)		//test cluster size
	{
		if (cluster_num > myMetric->point_number)		//if more clusters than points
		{
			continue;
		}
		myConf->number_of_clusters = cluster_num;
			KMPP_start = clock();
			KMedoidsPP(myConf, myMetric, distance_matrix, centroids);
			KMPP_finish = clock();
			Initialization_elapsed = (double)(KMPP_finish - KMPP_start)/CLOCKS_PER_SEC;

		cout << "==================" << endl << "PRINTING CENTROIDS for " << cluster_num <<" clusters: " <<endl;
		for (int w = 0; w <myConf->number_of_clusters; w++) {
			cout << centroids[w] << " ";
		}
		cout <<endl;

		for (int assign_iter = 0; assign_iter < 2; assign_iter++)
		{
			if (assign_iter == 0)		//case PAM
			{
				//clear clusterassign
				for (int i = 0; i < myMetric->point_number; ++i)
				{
					delete[] clusterAssign[i];
				}
				delete[] clusterAssign;
				clusterAssign= new int*[myMetric->point_number];
				for (int i = 0; i < myMetric->point_number; ++i)
				{
				    clusterAssign[i] = new int[3];
				    clusterAssign[i][0] = -1;
				    clusterAssign[i][1] = -1;
				    clusterAssign[i][2] = -1;
				}
				delete clusterTable;
				clusterTable = new ClusterTable(myConf->number_of_clusters);
				//cout << "starting pam - alaloyds" <<endl;
				Assign_Update_start = clock();
				for (int assign_update_times =0; assign_update_times < 3; assign_update_times++)
				{
					PAM(myConf, myMetric, distance_matrix, centroids, clusterTable, clusterAssign);		//assignment
					if (!ALaLoyds(myConf, myMetric, distance_matrix, centroids, clusterTable, clusterAssign)) {		//update
					 	//cout << "done!" << endl;
					}	
				}
				Assign_Update_finish = clock();
				Assign_Update_elapsed = (double)(Assign_Update_finish - Assign_Update_start)/CLOCKS_PER_SEC;

				//PRINTING SEGMENT
				//outputFile << "Algorithm: I" << init_iter+1 << "x" << assign_iter+1 << "x" << 1 <<endl;
				// for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
				// {
				// 	clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);		//used to get size
				// 	outputFile << "CLUSTER-" << cluster_iter << " {"<<size_of_cluster << ", " << clusterTable->ClusterDistance( myMetric, distance_matrix, cluster_iter, clusterAssign) <<"}" <<endl;
				// }
				//outputFile << "Clustering Time: " << Assign_Update_elapsed + Initialization_elapsed <<endl;





				current_silh = clusterTable->ReturnSilhouette(outputFile, myConf, distance_matrix, centroids, clusterAssign);		//returning Silhouette
				cout << "RET SILH " << cluster_num << " : " << current_silh <<endl;
				if (current_silh > best_silh)
				{
					best_silh = current_silh;
					best_cluster_num = cluster_num;
				}

			}
			
		}
		
	}

	myConf->number_of_clusters = best_cluster_num;

	//CLUSTER AGAIN with optimized k
	KMPP_start = clock();
	KMedoidsPP(myConf, myMetric, distance_matrix, centroids);
	KMPP_finish = clock();
	Initialization_elapsed = (double)(KMPP_finish - KMPP_start)/CLOCKS_PER_SEC;

	/*cout << "==================" << endl << "PRINTING CLUSTERS IN mainSample BEFORE CLARANS : " <<endl;
	for (int w = 0; w <myConf->number_of_clusters; w++) {
		cout << centroids[w] << " ";
	}
	cout << endl <<endl;*/

	//clear clusterassign
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		delete[] clusterAssign[i];
	}
	delete[] clusterAssign;
	clusterAssign= new int*[myMetric->point_number];
	for (int i = 0; i < myMetric->point_number; ++i)
	{
	    clusterAssign[i] = new int[3];
	    clusterAssign[i][0] = -1;
	    clusterAssign[i][1] = -1;
	    clusterAssign[i][2] = -1;
	}
	delete clusterTable;
	clusterTable = new ClusterTable(myConf->number_of_clusters);
	//cout << "starting pam - alaloyds" <<endl;
	Assign_Update_start = clock();
	cout << "==================" << endl << "PRINTING CENTROIDS with best number of centroids : " <<endl;
	for (int w = 0; w <myConf->number_of_clusters; w++) {
		cout << centroids[w] << " ";
	}
	cout << endl;


	for (int assign_update_times =0; assign_update_times < 3; assign_update_times++)
	{
		PAM(myConf, myMetric, distance_matrix, centroids, clusterTable, clusterAssign);		//assignment
		if (!ALaLoyds(myConf, myMetric, distance_matrix, centroids, clusterTable, clusterAssign)) {		//update
		 	//cout << "done!" << endl;
		}	
	}
	Assign_Update_finish = clock();
	Assign_Update_elapsed = (double)(Assign_Update_finish - Assign_Update_start)/CLOCKS_PER_SEC;
	/*for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
	{
		clusterTable->PrintClusterNo(cluster_iter);
	}*/

	current_silh = clusterTable->ReturnSilhouette(outputFile, myConf, distance_matrix, centroids, clusterAssign);		//returning Silhouette


	int total_on_clusters = 0;
	for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
	{
		delete[] clusterTable->PrintClusterDataForList(cluster_iter, &size_of_cluster);		//used to get size
		total_on_clusters += size_of_cluster;
		outputFile << "CLUSTER-" << cluster_iter << " {"<<size_of_cluster << ", " << clusterTable->ClusterDistance( myMetric, distance_matrix, cluster_iter, clusterAssign) <<"}" <<endl;
	}
	outputFile << "Total on clusters: " <<total_on_clusters <<endl;

	outputFile << "END SILH : " << current_silh <<endl<<endl;

	for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
	{
		clusterTable->PrintClusterItemPointNo(outputFile, cluster_iter);
	}

	total_finish = clock();
	total_elapsed = (double)(total_finish - total_start )/CLOCKS_PER_SEC;	//total clustering time

	if (takeData == true)
	{
		currentData[2] = myConf->number_of_clusters;	//best k
		currentData[3] = current_silh;					//best silh
		currentData[4] = total_elapsed;					//total clustering time
	}


		
	delete clusterTable;
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		delete[] clusterAssign[i];
	}
	delete[] clusterAssign;
	delete[] random_users_for_driver_user;
}

template <typename T>
double ListData<T>::ReturnUserGeneralRating(Metrics* myMetric, int itemno)
{
	double** ratings;
	int items_rated = 0;
	double total_rating = 0;
	Node<T>* currentNode = header;
	while (currentNode != NULL && currentNode->getItemNo() != itemno)
	{
		currentNode = currentNode->getNext();
	}

	if (currentNode!= NULL)
	{
		ratings = currentNode->getRatings();
		for (int i = 0; i < myMetric->point_dimension; ++i)
		{
			if (ratings[i] != 0)
			{
				items_rated++;
				//total_rating += ratings[i][0];	//unworkable with [0] - just for compilation
			}
		}
	}
	else 
	{
		cout << itemno << " haven't found this item on listdata" <<endl;
		return -1;
	}

	return (double)(total_rating / items_rated);
}

template <typename T>			//itemno is the user number == ItemNo(), item_no_rating is the item_no of the item's rating that this user gave
double ListData<T>::ReturnUserSpecificRating (int itemno, int item_no_rating)
{
	Node<T>* currentNode = header;
	double* ratings;

	while (currentNode != NULL && currentNode->getItemNo() != itemno)
	{
		currentNode = currentNode->getNext();
	}
	if (currentNode!= NULL)
	{
		ratings = currentNode->getRatings();
		return ratings[item_no_rating];
	}
	else 
	{
		cout << "haven't found this item on listdata" <<endl;
		return -1;
	}
}

template <typename T>			//itemno is the user number == ItemNo(), item_no_rating is the item_no of the item's rating that this user gave
double** ListData<T>::ReturnUserRatingTable (Metrics* myMetric)
{
	Node<T>* currentNode = header;
	double** ratings;

	int iterator = 0;

	ratings = new double*[myMetric->point_number];
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		ratings[i] = new double[myMetric->point_dimension];
	}

	/*while(currentNode != NULL)
	{
		for(int item_to_rate = 0; item_to_rate < myMetric->point_dimension ; item_to_rate++ )
		{
			//ratings[iterator][item_to_rate] = (currentNode->getRatings())[item_to_rate];
		}
		iterator++;
		currentNode = currentNode->getNext();
	}*/
	return ratings;
}

template <typename T>			//itemno is the user number == ItemNo(), item_no_rating is the item_no of the item's rating that this user gave
double* ListData<T>::ReturnUserGeneralRatingTable (Metrics* myMetric)
{
	Node<T>* currentNode = header;
	double** current_ratings;
	double* ratings;
	int items_rated = 0;
	double total_rating = 0;
	int iterator = 0;

	ratings = new double[myMetric->point_number];

	while(currentNode != NULL)
	{
		current_ratings = currentNode->getRatings();
		for (int i = 0; i < myMetric->point_dimension; ++i)
		{
			if (current_ratings[i] != 0)
			{
				items_rated++;
				//total_rating += current_ratings[i][0];		//unworkable with[0] -- just for compilation
			}
		}

		ratings[iterator] = (double)(total_rating / items_rated);
		iterator++;
		currentNode = currentNode->getNext();
	}
	return ratings;
}

template <typename T>			//itemno is the user number == ItemNo(), item_no_rating is the item_no of the item's rating that this user gave
double* ListData<T>::ReturnUserRatings(int itemno, double** ratings_table)
{
	return ratings_table[itemno];
}


template <typename T>			
double ListData<T>::TenFoldCrossValidation(Metrics* myMetric, double** distanceMatrix, double** user_rating_table, double* user_general_rating_table)
{
	string GARBAGE;
	int fold_size = 10;
	int fold_step = myMetric->point_number / fold_size;

	double others_rated = 0;

	double total_user_sum = 0;		//is Σ(Σ|R(j) - P(j)|) - the first sum for all test users
	double current_user_sum = 0;
	double similarity_sum = 0;
	double normalizing_factor = 0;
	double added_similarity = 0;
	double MAE_sum = 0;
	double item_rating_from_current_user_train_set = 0;

	double R_u = 0;

	double* other_user_ratings = NULL;
	double* current_user_test_set_ratings = NULL;


	//for every fold
	for (int first_user_test_set = 0; first_user_test_set < myMetric->point_number-1; first_user_test_set += fold_step)
	{
		//cout << "first_user_test_set : " <<first_user_test_set <<endl;
		total_user_sum = 0;
		//for every user on this fold
		for (int current_user_test_set = first_user_test_set; current_user_test_set < first_user_test_set + fold_step; current_user_test_set++)
		{
			current_user_sum = 0;
			//for every rating
			//cout << "current_user_test_set + fold_step : " <<current_user_test_set + fold_step <<endl;
			R_u = this->ReturnUserGeneralRating(myMetric, current_user_test_set);
			current_user_test_set_ratings = this->ReturnUserRatings(current_user_test_set, user_rating_table);
			
			for (int current_item = 0; current_item < myMetric->point_dimension; current_item++)
			{
				if (user_rating_table[current_user_test_set][current_item] != 0 )		//if current item is rated
				{
					//others_rated = 0;		//reset others rating of this item
					similarity_sum = 0;
					normalizing_factor = 0;
					added_similarity = 0;
					//for every user
					//others_rated += R_u;	//added R(u)

					for (int current_user_train_set = 0; current_user_train_set < myMetric->point_number; current_user_train_set++)
					{
						//if this user is not on test set - it belongs on train set
						if (current_user_train_set < first_user_test_set || current_user_train_set > first_user_test_set + fold_step)
						{
							//if other user has rated the item
							if (user_rating_table[current_user_train_set][current_item] != 0)
							{
								other_user_ratings = this->ReturnUserRatings(current_user_train_set, user_rating_table);
								item_rating_from_current_user_train_set = user_rating_table[current_user_train_set][current_item];
								if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
								{
									added_similarity = this->SimilarityHamming(myMetric, distanceMatrix, current_user_test_set, current_user_train_set);
								}
								if (strcmp(myMetric->metric_space.c_str(), "vector") == 0)
								{
									if (strcmp(myMetric->metric.c_str(), "euclidean") == 0)
									{
										added_similarity = this->SimilarityEuclidean(myMetric, distanceMatrix, current_user_test_set, current_user_train_set);
									}
									else if (strcmp(myMetric->metric.c_str(), "cosine") == 0)
									{
										added_similarity = this->SimilarityCosine(myMetric, current_user_test_set_ratings, other_user_ratings);
									}

								}
								//similarity_sum += ( this->SimilarityHamming(myMetric, distanceMatrix, current_user_test_set, current_user_train_set)) * (item_rating_from_other_user - this->ReturnUserGeneralRating(myMetric, current_user_test_set)) ;
								// similarity_sum += (added_similarity) * (item_rating_from_current_user_train_set - this->ReturnUserGeneralRating(myMetric, current_user_test_set)) ;
								similarity_sum += (added_similarity) * (item_rating_from_current_user_train_set - user_general_rating_table[current_user_train_set]);
								//cout << 3 <<endl;
								normalizing_factor += added_similarity;
								//cout << 4 <<endl;
							}
						}
					}
					if (normalizing_factor == 0) 
					{
						normalizing_factor = 1;		//but similarity sum is 0 so it will go to 0
					}
					//cout << "user_rating_table[current_user_test_set][current_item] : " << user_rating_table[current_user_test_set][current_item] << " 1/normalizing_factor : " << 1/normalizing_factor << " similarity_sum : " << similarity_sum <<endl;
					current_user_sum += abs(user_rating_table[current_user_test_set][current_item] - (R_u + ((1/normalizing_factor) * (similarity_sum))));
					//current_ratings[current_item_rated][1] = current_user_general_rating + ((1/normalizing_factor) * (similarity_sum));
				}
			}
			total_user_sum +=current_user_sum;
		}
		//cout << "total_user_sum before mul: " << total_user_sum <<endl;
		total_user_sum /= fold_step;
		//cout << "total_user_sum : " << total_user_sum <<endl;
		MAE_sum += total_user_sum;
	}
	cout << "MAE_sum/(double)fold_size : " << MAE_sum/(double)fold_size <<endl;
	return MAE_sum/(double)fold_size;
}


