#include "Headers.h"
#include "ListData.h"

template <typename T>
void ListData<T>::ListInsertionDB(std::ifstream& inputFile, Metrics* myMetric)
{
	string GARBAGE;
	string genericStr;
	string pointStr;
	string itemNos;
	int point_number = 0;
	int index;
	double* point;

	inputFile >> GARBAGE;  					//Read "@metric space"
	inputFile >> GARBAGE;					//Read etc, "matrix"

	inputFile >> itemNos;  				//Read "@items"
    getline(inputFile, genericStr);
    stringstream linestream2(genericStr);
    while (getline(linestream2, itemNos, ',')) {       //Calculate dimension of points
    	this->Insert(0, point_number, itemNos);
        point_number++;
    }
	myMetric->point_number = point_number;

	inputFile.clear();              //Restart
	inputFile.seekg(0, ios::beg);   //Data file back from start
}


template <typename T>
void ListData<T>::DistanceMatrixComputationDB(std::ifstream& inputFile, Metrics* myMetric, double** distance_matrix)
{
	string GARBAGE;
	string line;
	string pointStr;

	inputFile >> GARBAGE;       //Read "@metric space"      
    inputFile >> GARBAGE;       //Read "matrix"
    inputFile >> GARBAGE;	    //Read @item

    getline(inputFile, GARBAGE);
    for (int i = 0; i < myMetric->point_number; i++) {
    	//cout << "item " << i << "\t";
        getline(inputFile, line);
        stringstream linestream(line);
        for (int j= 0 ; j< myMetric->point_number; j++ ) {
                getline(linestream, pointStr, '\t');
                distance_matrix[i][j] = strtod(pointStr.c_str(), NULL);
                //cout << distance_matrix[i][j] << "\t";
        }
       // cout <<endl;
    }
}

