#include "Headers.h"
#include "ListData.h"

template <typename T>
void ListData<T>::ListInsertionHamming(std::ifstream& inputFile, Metrics* myMetric)
{
	string GARBAGE;
	string genericStr;
	string pointStr;
	string itemNos;
	boost::dynamic_bitset<> currentPoint(myMetric->point_dimension);
	int count_ins = 0;
	int point_number = 0;
	int index;
	double* point;
	double* ratings;


	if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
	{
		getline(inputFile, genericStr);
		ratings = new double[myMetric->point_dimension];
		for (int i = 0; i < myMetric->point_dimension; ++i)
		{
			ratings[i] = 0;
		}
		do {

			stringstream linestream(genericStr);
			getline(linestream, pointStr, '\t');
			//cout << pointStr <<endl;
			if (stoi(pointStr) != point_number)	//if we are changing user
			{
				
				if (point_number != 0)		//we are changing user
				{
					this->Insert(currentPoint, count_ins, to_string(point_number), ratings);
					count_ins++;
				}
				point_number = stoi(pointStr);
				currentPoint.reset();		//reset the bitset
				ratings = new double[myMetric->point_dimension];
				for (int i = 0; i < myMetric->point_dimension; ++i)
				{
					ratings[i] = 0;
				}
			}
			getline(linestream, pointStr, '\t');
			index = stoi(pointStr);
			getline(linestream, pointStr, '\t');
			ratings[index-1] = stoi(pointStr);
			if (stoi(pointStr) > 1)			//if it has a positive rating mark it
			{
				currentPoint[index-1] = 1;
			}

   		}while(getline(inputFile, genericStr));
   		this->Insert(currentPoint, count_ins, to_string(point_number), ratings);
   		count_ins++;
	}

}

template <typename T>
void ListData<T>::DistanceMatrixComputationHamming(Metrics* myMetric, double** distance_matrix) {
	Node<T>* driver_node;
	Node<T>* current_node;
	string GARBAGE;

	driver_node = this->header;

	for (int i = 0; i < myMetric->point_number; i++) {
		current_node = driver_node->getNext();
		cout << "item " << i << "\t";
		for (int j = 0; j < myMetric->point_number; j++) {
			if (j <= i) {
				distance_matrix[i][j] = 0;
			}
			else {
				if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
				{
					//cout << myMetric->point_number <<endl;
					//cout << "distance_matrix[" << i << "][" << j<<"]" <<endl;
					//cout << "item: " << current_node->getKey() <<endl;
					distance_matrix[i][j] = this->DistanceB2BHamming(current_node->getKey(), driver_node->getKey());
					/*if (i == 0)
					{
						cout <<distance_matrix[i][j] << " ";					//cout << distance_matrix[i][j] << "\t";
					}*/
					
					current_node = current_node->getNext();
				}
			}
		}
		//cout << endl;
		driver_node = driver_node->getNext();
	}
	cout << endl;
}

template <typename T>
double ListData<T>::SimilarityHamming(Metrics* myMetric, double** distance_matrix, double item1, double item2)
{
	return myMetric->point_dimension - DistanceMatrixDistance(distance_matrix, item1, item2);
}