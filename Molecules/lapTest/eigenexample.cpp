#include <iostream>
#include <fstream>
#include <lapacke.h>

extern "C"
{
	#include <stdio.h>
	#include <math.h>
}


using namespace std;

int main(int argc, char const *argv[])
{
	
	double a[3][3] = {6, 3, -8, 0, -2, 0, 1, 0, -3};
	int i, j;
	lapack_int info, n, lda, ldvl, ldvr;
	double *real, *imaginary, *rvectors, *lvectors;
	n = lda = ldvl = ldvr = 3;
	real =  (double*) LAPACKE_malloc(sizeof(double) * n);
	imaginary = (double*) LAPACKE_malloc(sizeof(double) * n);
	rvectors = (double*) LAPACKE_malloc(sizeof(double) * n * n);

	info = LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', n, *a, lda, real, imaginary, lvectors, ldvl, rvectors, ldvr);
	for (int i = 0; i < n; ++i)
	{
		cout << "Eigenvalue : " << real[i] << " and " << imaginary[i] << endl;
		cout << "Eigenvector Transposed : [ ";
		if (fabs(imaginary[i]) < 10e-7)
		{
			for (int j = 0; j < n; ++j)
			{
				cout << " | " << rvectors[j * 3+i];
			}
			cout << " ]" << endl;
		}
	}

	LAPACKE_free(real);
	LAPACKE_free(imaginary);
	LAPACKE_free(rvectors);
	return(info);
}