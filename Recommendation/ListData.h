#ifndef LIST_DATA
#define LIST_DATA

#include "Headers.h"
#include "Node.h"
#include "MathFunctions.h"
//#include "Returns.h"
#include "Hash.h"
#include "ClusterTable.h"		//includes Returns
#include "Algorithms.h"
#include "MathFunctions.cpp"

template <typename T>
class ListData
{
	public:
		ListData();
		~ListData();
        Node<T>* ReturnHead();
        Node<T>* getNode();
		void PrintData();
		void Insert(T item, int itemno, std::string itemName, double* ratings);
		void initEuclideanList(Conf* myConf,Metrics* myMetric, ifstream& inputFile, ofstream& outputFile, double** distanceMatrix, int k, int L, int* dataLength, int* dataLengthPointNumber, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign, bool complete_printing);
		void initCosineList(Conf* myConf,Metrics* myMetric,  ifstream& inputFile, ofstream& outputFile, double** distanceMatrix, int k, int L, int* dataLength, int* dataLengthPointNumber, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign, bool complete_printing);
		void initDBHManagement(Conf* myConf, ifstream& inputFile, double** distanceMatrix, int k, int L, int* dataLength, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign);
		void initHammingLSHManagement(Conf* myConf,Metrics* myMetric, ifstream& inputFile, ofstream& outputFile, double** distanceMatrix, int k, int L, int* dataLength, int* hashCreationDone, Hash<T>* hashTableList, int* centroids, int** clusterAssign, bool complete_printing);

		int Distance(T item1, T item2);
		bool ExistsOnce(int itemno, Node<T>* header);
		bool Exists(int itemno, Node<T>* header_out);
		double CosineDistance(double p1[], double p2[], int size);
		bool HammingDuplicate(T item);
		bool HammingB2BDuplicate(T item);
		bool EuclideanDuplicate(T item, int size);
		void ListInsertionVector(std::ifstream& inputFile, Metrics* myMetric);
		void DistanceMatrixComputationVector(Metrics* myMetric, double** distance_matrix);
		void ListInsertionDB(std::ifstream& inputFile, Metrics* myMetric);
		void DistanceMatrixComputationDB(std::ifstream& inputFile, Metrics* myMetric, double** distance_matrix);
		void ListInsertionHamming(std::ifstream& inputFile, Metrics* myMetric);
		double DistanceB2BHamming(T item1, T item2);
		void DistanceMatrixComputationHamming(Metrics* myMetric, double** distance_matrix);
		string* ItemNamesFromItemNos(int* items_in_cluster_itemNo, int size_of_cluster);
		void Printer(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, bool complete_printing);
		void ClusterHandleExercise3(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, bool complete_printing);
		double ReturnUserGeneralRating(Metrics* myMetric, int itemno);
		double SimilarityHamming(Metrics* myMetric, double** distance_matrix, double item1, double item2);
		double SimilarityEuclidean(Metrics* myMetric, double** distance_matrix, double item1, double item2);
		double SimilarityCosine(Metrics* myMetric, double* item1, double* item2);
		double ReturnUserSpecificRating (int itemno, int item_no_rating);
		double** ReturnUserRatingTable (Metrics* myMetric);
		double* ReturnUserGeneralRatingTable (Metrics* myMetric);
		double* ReturnUserRatings(int itemno, double** ratings_table);

		double TenFoldCrossValidation(Metrics* myMetric,double** distanceMatrix,  double** user_rating_table, double* user_general_rating_table);
	private:
		Node<T>* header;
};


#endif // !LIST_DATA_HAMMING