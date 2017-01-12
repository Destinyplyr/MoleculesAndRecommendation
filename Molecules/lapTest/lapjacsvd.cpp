#include <iostream>
#include <fstream>
#include <lapacke.h>

using namespace std;

extern "C"
{
	#include <stdio.h>
	#include <math.h>
}

int main(int argc, char const *argv[])
{
	
	double input[3][3] = {1, 1, 0, 0, 1, 1, 1, 2, 1};
	int i, j;
	lapack_int info, n, ldinput, ldv;
	double *singular, *v, *stat;

	n = ldv = ldinput = 3;
	singular = (double* ) LAPACKE_malloc(sizeof(double) * n);
	stat = (double* ) LAPACKE_malloc(sizeof(double) * 2 * n);

	info = LAPACKE_dgesvj(LAPACK_ROW_MAJOR, 'G', 'N', 'N', n, n, *input, ldinput, singular, 0, v, ldv, stat);
	for (int i = 0; i < n; ++i)
	{
		cout << "Singular Value : " << singular[i] << endl;
	}

	LAPACKE_free(singular);
	return(info);
}