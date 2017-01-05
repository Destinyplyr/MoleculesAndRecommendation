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
	//bitset<64> currentPoint;
	int count_ins = 0;
	int point_number = 0;
	int index;
	double* point;


	if (strcmp(myMetric->metric_space.c_str(), "hamming") == 0)
	{
		//inputFile >> itemNos;  				//Read "item1"
		cout << 1 <<endl;
		getline(inputFile, genericStr);
		do {
			//index = 0;

			stringstream linestream(genericStr);
			getline(linestream, pointStr, '\t');
			//cout << pointStr <<endl;
			if (stoi(pointStr) != point_number)	//if we are changing user
			{
				if (point_number != 0)		//we are changing user
				{
					//cout << "Inserting: " << currentPoint <<endl;
					//cin >> GARBAGE;
					this->Insert(currentPoint, point_number, to_string(point_number));
					count_ins++;
				}
				point_number = stoi(pointStr);
				currentPoint.reset();		//reset the bitset
			}
			getline(linestream, pointStr, '\t');
			index = stoi(pointStr);
			/*if (stoi(pointStr) > item_max)
			{
				item_max = stoi(pointStr);
			}*/
			getline(linestream, pointStr, '\t');
			if (stoi(pointStr) > 2)			//if it has a positive rating mark it
			{
				currentPoint[index-1] = 1;
			}

	   		/*stringstream linestream(genericStr);
	   		getline(linestream, pointStr, '\t');
	   		getline(linestream, pointStr, '\t');*/

	   		//currentPoint = bitset<64>(string(pointStr));
    		/*if (!this->HammingB2BDuplicate(currentPoint)) {
    			cout << "point inserted: " << currentPoint << " - " << point_number << " - " << itemNos <<endl;
    			this->Insert(currentPoint, point_number, itemNos);
    		}
    		inputFile >> itemNos;		//next itemno
    		point_number++;*/

   		}while(getline(inputFile, genericStr));
   		this->Insert(currentPoint, point_number, to_string(point_number));
   		count_ins++;
   		//cout << "inserted: " << count_ins <<endl;
   		//cin >> GARBAGE;
   		//myMetric->point_number = point_number;
	}

}

template <typename T>
void ListData<T>::DistanceMatrixComputationHamming(Metrics* myMetric, double** distance_matrix) {
	Node<T>* driver_node;
	Node<T>* current_node;
	string GARBAGE;

	driver_node = this->header;
	// bitset<64> a(string("1010111010100100001010110000100100001001000100011101100011100000"));
	// bitset<64> b(string("0010111010100100011011110001100010011001000000011101100001100000"));
	//cout << myMetric->point_number <<endl;

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
					//cin >> GARBAGE;
					//cout << "item: " << current_node->getKey() <<endl;
					distance_matrix[i][j] = this->DistanceB2BHamming(current_node->getKey(), driver_node->getKey());
					//cout << "dham: " <<distance_matrix[i][j] <<endl;					//cout << distance_matrix[i][j] << "\t";
					current_node = current_node->getNext();
				}
			}
		}
		driver_node = driver_node->getNext();
	}
}