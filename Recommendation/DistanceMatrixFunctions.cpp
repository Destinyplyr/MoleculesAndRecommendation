#include "DistanceMatrixFunctions.h"

using namespace std;


double DistanceMatrixDistance(double** distanceMatrix, double item1, double item2) {

    int driver = 0;
    int left = 0;

    if (item1 >= item2) {   //Driver the smallest to also work with the instructions
        driver = item2;
        left = item1;
    }
    else {
        driver = item1;
        left = item2;
    }
    return distanceMatrix[driver][left];
}


void quickSort(double* myArray, int first, int last ){
    int pivot;
    if(first < last){
        pivot = parition(myArray, first, last);
        quickSort(myArray, first, pivot-1);
        quickSort(myArray, pivot+1, last);
    }
}


int parition(double* myArray, int first, int last){
    int  piv = first;
    int pivot = myArray[first];

    for(int i = first+1 ; i <= last ; i++){
        if(myArray[i] <= pivot)
        {
            piv++;
            swap(myArray[i], myArray[piv]);
            
        }
    }
    swap(myArray[piv], myArray[first]);
    return piv;
}


void swap(int& a, int& b){
    int temp = a;
    a = b;
    b = temp;
}



void quickSort_twolist(double** myArray, int first, int last ){
    int pivot;
    if(first < last){
        pivot = parition_twolist(myArray, first, last);
        quickSort_twolist(myArray, first, pivot-1);
        quickSort_twolist(myArray, pivot+1, last);
    }
}


int parition_twolist(double** myArray, int first, int last){
    int  piv = first;
    double pivot = myArray[first][1];

    for(int i = first+1 ; i <= last ; i++){
        if(myArray[i][1] <= pivot)
        {
            piv++;
            swap_twolist(myArray[i], myArray[piv]);
            
        }
    }
    swap_twolist(myArray[piv], myArray[first]);
    return piv;
}


void swap_twolist(double* a, double* b){
    double* temp = new double[2];
    temp[0] = a[0];
    temp[1] = a[1];
    a[0] = b[0];
    a[1] = b[1];
    b[0] = temp[0];
    b[1] = temp[1];
    delete[] temp;
}


double FindRadiusForAssignment(Conf* myConf,double** distanceMatrix, int* centroids)
{
    double minDistance = INT_MAX;
    for (int centroid_iter = 0; centroid_iter < myConf->number_of_clusters; centroid_iter++)
    {
        for (int second_centroid = centroid_iter+1; second_centroid < myConf->number_of_clusters; second_centroid++)
        {
            //cout << "Second centrod : " << centroids[second_centroid] << endl;
            //cout << "Distace Matrix Distace : " << DistanceMatrixDistance(distanceMatrix, centroids[centroid_iter], centroids[second_centroid]) << endl;
            //cout << "Min distance before calculations : " << minDistance << endl;
            if(DistanceMatrixDistance(distanceMatrix, centroids[centroid_iter], centroids[second_centroid]) < minDistance)
            {
                minDistance = DistanceMatrixDistance(distanceMatrix, centroids[centroid_iter], centroids[second_centroid]);
                //cout << "New min distance : " << minDistance << endl;
            }
        }
    }
    return minDistance / 2;
}

//return -1 if stop
double FindNextRadius(double** min_max_thresh, double oldRadius, double oldInserted, double neighborhood_size)        //min_max_thresh[0][0] - min radius, min_max_thresh[0][1] - min radius items inserted, min_max_thresh[1][0] - max radius, min_max_thresh[1][1] - max radius items inserted
{                                                                                           //min_max_thresh[1][0] - middle radius, min_max_thresh[1][1] - middle radius items inserted
    
    if (oldRadius == min_max_thresh[0][0])  //we have used it with no luck
    {
        min_max_thresh[0][1] = oldInserted;
        if (oldInserted > neighborhood_size)
        {
            return -1;
        }
    }
    else if (oldRadius == min_max_thresh[2][0])  //we have used max with no luck
    {
        min_max_thresh[2][1] = oldInserted;
        if (oldInserted < neighborhood_size)          
        {                                       //max becomes min
            min_max_thresh[0][0] = min_max_thresh[2][0];
            min_max_thresh[0][1] = min_max_thresh[2][1];
            min_max_thresh[2][0] *=2;               //max doubles
            min_max_thresh[2][1] = -1;
            return  min_max_thresh[2][0];
        }
        else
        {
            min_max_thresh[1][0] = (min_max_thresh[0][0] + min_max_thresh[2][0])/2;
            return min_max_thresh[1][0];
        }
        
    }

    if (min_max_thresh[0][1] == -1)     //haven't used min_max min
    {
        return min_max_thresh[0][0];
    }

    else if (min_max_thresh[2][1] == -1)     //haven't used min_max max
    {
        return min_max_thresh[2][0];
    }

    if (min_max_thresh[1][0] == oldRadius)      //middle tried
    {
        min_max_thresh[1][1] = oldInserted;
        if (oldInserted > neighborhood_size)    //middle as max    
        {
            min_max_thresh[2][0] = oldRadius;
            min_max_thresh[2][1] = oldInserted;
        }
        else if (oldInserted < neighborhood_size)   //middle as min
        {
            min_max_thresh[0][0] = oldRadius;
            min_max_thresh[0][1] = oldInserted;
        }
        min_max_thresh[1][0] = (min_max_thresh[0][0] + min_max_thresh[2][0])/2;
        min_max_thresh[1][1] = -1;
        return min_max_thresh[1][0];
    }



}