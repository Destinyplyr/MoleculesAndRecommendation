#include "MathFunctions.h"
using namespace std;

//double dot_product(double* vector1, double* vector2, int vector_size) {     //T will always be double*
template <typename T>
double dot_product(T vector1, double* vector2, int vector_size) {
	double dot_product = 0;
	for (int i =0; i < vector_size; i++) {
        //cout << "vector 1 - vector 2: " << vector1[i] << " - " << vector2[i] <<endl;
		dot_product += vector1[i] * vector2[i];
	}
	return dot_product;
}

//double euclid_norm(double* vector1, int vector_size) {        //actually this
template <typename T>
double euclid_norm(T vector1, int vector_size) {
    double norm = 0;
    for (int i = 0; i < vector_size; i++) {
        norm += (pow(vector1[i], 2));
    }
    return sqrt(norm);
}
template <typename T>
bool SameArray(T array1, T array2, int size) {
    for (int i = 0; i < size; i++) {
        //cout << array1[i]<< "-" << array2[i] <<endl;
        if (array1[i] != array2[i]) {
            return false;
        }
    }
    return true;
}


template <typename T>
double DistanceEuclid(T item1[], T item2[], int size)
{
    double sum = 0.0f, square;
    double x, y;
    int i = 0, j = 0;

    while ((i < size) && (j < size))
    {

        x = item1[i];
        y = item2[j];
        square = pow((x - y), 2);
        sum += square;
        ++i;
        ++j;
    }
    return sqrt(sum);
}
