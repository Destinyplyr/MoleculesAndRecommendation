#include "Headers.h"
#include "UtilsH.h"
#include "Algorithms.h"

void Concentrate(Conf* myConf, Metrics* myMetric, double** distanceMatrix, int* centroids)
{
	double Sum = 0;
	double* divisor;
	double maxProb = 0;
	double** V_s = new double*[myMetric->point_number];     //V_s[i][0] -> holds V for i centroid [1] holds which point is this centroid
	for (int i = 0; i < myMetric->point_number; ++i)		//init
	{
	    V_s[i] = new double[2];
		V_s[i][0] = 0;
	}

	int chosenCentroid;
	int column, row;
	string GARBAGE;

	divisor = new double [myMetric->point_number];
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		divisor[i] = 0;
		for (int j = 0; j < myMetric->point_number; ++j)
		{
			divisor[i] += DistanceMatrixDistance(distanceMatrix, i, j);
		}
	}



	for (int i =0; i <myMetric->point_number; ++i ) {
		for (int j = 0; j < myMetric->point_number; ++j)
		{
			if ( i != j) {
				V_s[i][0] += DistanceMatrixDistance(distanceMatrix, i, j) / divisor[j];
			}
		}
		V_s[i][1] = i;
	}

	quickSort_twolist(V_s, 0, myMetric->point_number-1);

/*	for (int i = 0; i < 4; ++i)
	{
		cout << newh[i][0] <<newh[i][1] <<endl;
	}*/
	
/*    for (int i = 0; i < myMetric->point_number ; ++i)
    {
    	cout << "V_s " << i << " - " << V_s[i][0] << " - " << V_s[i][1] <<endl;
    }*/
    for (int i = 0; i < myConf->number_of_clusters; i ++) {
        centroids[i] = V_s[i][1];
    }
    for (int i = 0; i < myMetric->point_number; ++i)		//init
    {
       delete V_s[i];
    }
    delete[] V_s;     //V_s[i][0] -> holds V for i centroid [1] holds which point is this centroid

    delete divisor;
}

void swap_twolist(double& a, double& b){
    double temp = a;
    a = b;
    b = temp;
}

void quickSort_twolist(double** myArray, int first, int last ){ //appied QS for twolist
    int pivot;
    if(first < last){
        pivot = parition_twolist(myArray, first, last);
        quickSort_twolist(myArray, first, pivot-1);
        quickSort_twolist(myArray, pivot+1, last);
    }
}


int parition_twolist(double** myArray, int first, int last){        //appied QS for twolist
    int  piv = first;
    double pivot = myArray[first][0];

    for(int i = first+1 ; i <= last ; i++){
        if(myArray[i][0] <= pivot)
        {
        	piv++;
            swap_twolist(myArray[i][0], myArray[piv][0]);
            swap_twolist(myArray[i][1], myArray[piv][1]);
           
        }
    }
    swap_twolist(myArray[piv][0], myArray[first][0]);
    swap_twolist(myArray[piv][1], myArray[first][1]);
    return piv;
}