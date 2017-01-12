#include "Headers.h"
#include "ListData.h"
#include <cblas.h>
#include <lapacke.h>
//#include <lapack.h>


template <typename T>
double*** ListData<T>::AllConformationTableCreation(Metrics* myMetric)
{
	string GARBAGE;
	Node<T>* currentNode = header;
	int conformation_no = 0;
	double*** all_conformation_table = new double**[myMetric->point_number];	//new conformation table
	while(currentNode != NULL)
	{
		all_conformation_table[conformation_no] = new double*[myMetric->point_dimension];
		for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
		{
			all_conformation_table[conformation_no][current_backbone_atom] = new double[3];
			all_conformation_table[conformation_no][current_backbone_atom][0] = (currentNode->getKey())[current_backbone_atom][0];
			all_conformation_table[conformation_no][current_backbone_atom][1] = (currentNode->getKey())[current_backbone_atom][1];
			all_conformation_table[conformation_no][current_backbone_atom][2] = (currentNode->getKey())[current_backbone_atom][2];
			//cout << all_conformation_table[conformation_no][current_backbone_atom][0] << " - " << all_conformation_table[conformation_no][current_backbone_atom][1] << " - " << all_conformation_table[conformation_no][current_backbone_atom][2] <<endl;
			//cin >> GARBAGE;
		}
		//all_conformation_table[conformation_no] = currentNode->getKey();
		conformation_no++;
		currentNode = currentNode->getNext();
		//cin >>GARBAGE;
	}
	return all_conformation_table;
}

template <typename T>
void ListData<T>::ConformationTableMove(Metrics* myMetric, double*** all_conformation_table)
{
	string GARBAGE;
	Node<T>* currentNode = header;
	double* x_c = new double[3];				//xc is the conformation move factor
	for (int conformation_no = 0; conformation_no < myMetric->point_number; conformation_no++)
	{
		for (int vector_iter = 0; vector_iter <3; vector_iter++)
		{
			x_c[vector_iter] = 0;
		}

		for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
		{
			//cout << "current_backbone_atom : " << current_backbone_atom <<endl;
			for (int vector_iter = 0; vector_iter <3; vector_iter++)
			{
				x_c[vector_iter] += all_conformation_table[conformation_no][current_backbone_atom][vector_iter];
				//cout << " all_conformation_table[conformation_no][current_backbone_atom][vector_iter] : " << all_conformation_table[conformation_no][current_backbone_atom][vector_iter] <<endl;
			}
			/*all_conformation_table[conformation_no][current_backbone_atom] = new double[3];
			all_conformation_table[conformation_no][current_backbone_atom][0] = (currentNode->getKey())[current_backbone_atom][0];
			all_conformation_table[conformation_no][current_backbone_atom][1] = (currentNode->getKey())[current_backbone_atom][1];
			all_conformation_table[conformation_no][current_backbone_atom][2] = (currentNode->getKey())[current_backbone_atom][2];*/
			//cout << all_conformation_table[conformation_no][current_backbone_atom][0] << " - " << all_conformation_table[conformation_no][current_backbone_atom][1] << " - " << all_conformation_table[conformation_no][current_backbone_atom][2] <<endl;
			//cin >> GARBAGE;
		}

		for (int vector_iter = 0; vector_iter <3; vector_iter++)
		{
			x_c[vector_iter] /= myMetric->point_dimension;		//now x_c is ready to be subtracted from all_conformation_table
			//cout << "x_c[" << vector_iter << "] : " << x_c[vector_iter] <<endl;
			//cin >> GARBAGE;
		}

		for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
		{
			for (int vector_iter = 0; vector_iter <3; vector_iter++)
			{
				all_conformation_table[conformation_no][current_backbone_atom][vector_iter] -= x_c[vector_iter];
			}
			/*all_conformation_table[conformation_no][current_backbone_atom] = new double[3];
			all_conformation_table[conformation_no][current_backbone_atom][0] = (currentNode->getKey())[current_backbone_atom][0];
			all_conformation_table[conformation_no][current_backbone_atom][1] = (currentNode->getKey())[current_backbone_atom][1];
			all_conformation_table[conformation_no][current_backbone_atom][2] = (currentNode->getKey())[current_backbone_atom][2];*/
			//cout << all_conformation_table[conformation_no][current_backbone_atom][0] << " - " << all_conformation_table[conformation_no][current_backbone_atom][1] << " - " << all_conformation_table[conformation_no][current_backbone_atom][2] <<endl;
			//cin >> GARBAGE;
		}


		//all_conformation_table[conformation_no] = currentNode->getKey();
	}
}

template <typename T>
double ListData<T>::cRMSD(Metrics* myMetric, double** conformation_X, double** conformation_Y)
{
	double* comformation_X_consec = NULL;
	comformation_X_consec = new double[3*myMetric->point_dimension];
	for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
	{
		for (int vector_iter = 0; vector_iter <3; vector_iter++)
		{
			comformation_X_consec[current_backbone_atom * vector_iter] = conformation_X[current_backbone_atom][vector_iter];
		}
	}

	double* comformation_Y_consec = NULL;
	comformation_Y_consec = new double[3*myMetric->point_dimension];
	for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
	{
		for (int vector_iter = 0; vector_iter <3; vector_iter++)
		{
			comformation_Y_consec[current_backbone_atom * vector_iter] = conformation_Y[current_backbone_atom][vector_iter];
		}
	}

	double* returned_table = NULL;
	returned_table = new double[3*myMetric->point_dimension];

//	cblas_dgemm(row/col major, 	A/A^T, 		B/B^T, 		  A/C rows,	B cols, A cols/B rows, scalar alpha, , conformation_X, A rows, conformation_Y, B rows, scalar beta,		C return table, C rows 						 )
	cblas_dgemm (CblasRowMajor, CblasTrans, CblasNoTrans, 3, 3, myMetric->point_number , 1, comformation_X_consec, 3 , comformation_Y_consec, myMetric->point_dimension, 0, returned_table, 3);
}

