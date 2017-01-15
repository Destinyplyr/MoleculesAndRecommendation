#include "Headers.h"
#include "ListData.h"
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
		}
		conformation_no++;
		currentNode = currentNode->getNext();
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
		}

		for (int vector_iter = 0; vector_iter <3; vector_iter++)
		{
			x_c[vector_iter] /= myMetric->point_dimension;		//now x_c is ready to be subtracted from all_conformation_table
			//cout << "x_c[" << vector_iter << "] : " << x_c[vector_iter] <<endl;
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
		}
	}

	delete[] x_c;
}

template <typename T>
void ListData<T>::print_matrix(const char* desc, lapack_int m, lapack_int n, double* a, lapack_int lda ) {
	lapack_int i, j;
    printf( "\n %s\n", desc );
    for( i = 0; i < m; i++ ) {
        for( j = 0; j < n; j++ ) printf( " %6.2f", a[i*lda+j] );
    	printf( "\n" );
	}
}

template <typename T>
//tables are on consecutive form
double ListData<T>::ThreebyThreeDeterminant(double* three_x_three_table)
{ //following: https://wikimedia.org/api/rest_v1/media/math/render/svg/4633ab870e238995bc59acfccf5d5e8f1efe1984
    double det = 0;
    det += three_x_three_table[0] * three_x_three_table[4] * three_x_three_table[8];
    det += three_x_three_table[1] * three_x_three_table[5] * three_x_three_table[6];
    det += three_x_three_table[2] * three_x_three_table[3] * three_x_three_table[7];
    det -= three_x_three_table[2] * three_x_three_table[4] * three_x_three_table[6];
    det -= three_x_three_table[1] * three_x_three_table[3] * three_x_three_table[8];
    det -= three_x_three_table[0] * three_x_three_table[5] * three_x_three_table[7];
    return det;
}

template <typename T>
double* ListData<T>::MxN_MatrixMinusMatrix(Metrics* myMetric, double* matrix1, double* matrix2, double* result_matrix, int m, int n )
{
	for (int row = 0; row < m; ++row)
	{
		for (int column = 0; column < n; column++)
		{
			result_matrix[3*row + column] = matrix1[3*row + column] - matrix2[3*row + column];
		}
		
		//cout << returned_table[3*i+0] << "\t" << returned_table[3*i+1] << "\t" << returned_table[3*i+2] << endl;
	}
}

template <typename T>
double ListData<T>::cRMSD(Metrics* myMetric, double** conformation_X, double** conformation_Y)
{
	string GARBAGE;
	double* comformation_X_consec = NULL;
	comformation_X_consec = new double[3*myMetric->point_dimension];
	for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
	{
		for (int vector_iter = 0; vector_iter <3; vector_iter++)
		{
			comformation_X_consec[3*current_backbone_atom + vector_iter] = conformation_X[current_backbone_atom][vector_iter];
		}
	}

	double* comformation_X_consec_rotated = NULL;
	comformation_X_consec_rotated = new double[3*myMetric->point_dimension];

	double* comformation_Y_consec = NULL;
	comformation_Y_consec = new double[3*myMetric->point_dimension];
	for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
	{
		for (int vector_iter = 0; vector_iter <3; vector_iter++)
		{
			comformation_Y_consec[3*current_backbone_atom + vector_iter] = conformation_Y[current_backbone_atom][vector_iter];
		}
	}

	double* returned_table = NULL;
	returned_table = new double[3*myMetric->point_dimension];

	double* x_q_minus_y = NULL;
	x_q_minus_y = new double[3*myMetric->point_dimension];

	double* q_rotation_table = new double[3*3];

	double* u_table = new double[3*3];
	double* singular = new double[3];
	double* v_t_table = new double[3*3];
	double* stat = new double[2*3];

	double Frobenius_norm = 0;


	/*cout << "X Matrix : " <<endl;
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		cout << comformation_X_consec[3*i+0] << "\t" << comformation_X_consec[3*i+1] << "\t" << comformation_X_consec[3*i+2] << endl;
	}
	cout << "Y Matrix : " <<endl;
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		cout << comformation_Y_consec[3*i+0] << "\t" << comformation_Y_consec[3*i+1] << "\t" << comformation_Y_consec[3*i+2] << endl;
	}*/

//	calculate X^T * Y = C
//	cblas_dgemm(row/col major, 	A/A^T, 		B/B^T, 		  A/C rows,	B cols, A cols/B rows, scalar alpha, , conformation_X, A rows, conformation_Y, B rows, scalar beta,		C return table, C rows 						 )
	cblas_dgemm (CblasRowMajor, CblasTrans, CblasNoTrans, 3, 3, myMetric->point_dimension , 1, comformation_X_consec, 3 , comformation_Y_consec, 3, 0, returned_table, 3);


	//LAPACKE_dgesvd( LAPACK_ROW_MAJOR, 'A', 'A', m, n, a, lda,s, u, ldu, vt, ldvt, superb );
	/*cout << "SVD Matrix : " <<endl;
	for (int i = 0; i < 3; ++i)
	{
		cout << returned_table[3*i+0] << "\t" << returned_table[3*i+1] << "\t" << returned_table[3*i+2] << endl;
	}*/
	LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', 3, 3, returned_table, 3, singular, u_table, 3, v_t_table, 3, stat );
	/*cout << "U - SVD Matrix : " <<endl;
	for (int i = 0; i < 3; ++i)
	{
		//cout << returned_table[i+0] << "\t" << returned_table[i+1] << "\t" << returned_table[i+2] << endl;
		cout << u_table[3*i+0] << "\t" << u_table[3*i+1] << "\t" << u_table[3*i+2] << endl;
	}*/

	/*cout << "VT - SVD Matrix : " <<endl;
	for (int i = 0; i < 3; ++i)
	{
		//cout << returned_table[i+0] << "\t" << returned_table[i+1] << "\t" << returned_table[i+2] << endl;
		cout << v_t_table[3*i+0] << "\t" << v_t_table[3*i+1] << "\t" << v_t_table[3*i+2] << endl;
	}*/

	string desc = "Singular values";
	//print_matrix( desc.c_str(), 1, 3, singular, 1 );

	//CHECK σ3 is >0
	if (singular[2] <= 0)
	{
		//cin >> GARBAGE;
		cout << "SVD check failed : returning dist INT_MAX" <<endl;
		return INT_MAX;
	}

	//Q is U*V^T
	cblas_dgemm (CblasRowMajor, CblasNoTrans, CblasNoTrans, 3, 3, 3 , 1, u_table, 3 , v_t_table, 3, 0, q_rotation_table, 3);

	/*//cout << "Q rotation Matrix : " <<endl;
	for (int i = 0; i < 3; ++i)
	{
		q_rotation_table[3*i+0] = 1;
		q_rotation_table[3*i+1] = -1;
		q_rotation_table[3*i+2] = 0;
		//cout << returned_table[i+0] << "\t" << returned_table[i+1] << "\t" << returned_table[i+2] << endl;
		//cout << q_rotation_table[i+0] << "\t" << q_rotation_table[i+1] << "\t" << q_rotation_table[i+2] << endl;
	}
*/
	/*cout << "Q rotation Matrix : " <<endl;
	for (int i = 0; i < 3; ++i)
	{
		//cout << returned_table[i+0] << "\t" << returned_table[i+1] << "\t" << returned_table[i+2] << endl;
		cout << q_rotation_table[3*i+0] << "\t" << q_rotation_table[3*i+1] << "\t" << q_rotation_table[3*i+2] << endl;
	}*/


	//cout << "det of Q: " << ThreebyThreeDeterminant(q_rotation_table) <<endl;
	if (ThreebyThreeDeterminant(q_rotation_table) < 0)
	{	//negate 3rd column of U
		for (int i = 0; i < 3; ++i)
		{
			u_table[3*i+2] = -u_table[3*i+2];
			//recalculate Q
			cblas_dgemm (CblasRowMajor, CblasNoTrans, CblasNoTrans, 3, 3, 3 , 1, u_table, 3 , v_t_table, 3, 0, q_rotation_table, 3);
		}
	}

	//X_rotated = X * Q
	cblas_dgemm (CblasRowMajor, CblasNoTrans, CblasNoTrans, myMetric->point_dimension, 3, 3 , 1, comformation_X_consec, 3 , q_rotation_table, 3, 0, comformation_X_consec_rotated, 3);

	this->MxN_MatrixMinusMatrix(myMetric, comformation_X_consec_rotated, comformation_Y_consec , x_q_minus_y, myMetric->point_dimension, 3);

	/*cout << "Matrix for frobi 1: " <<endl;
	for (int i = 0; i < myMetric->point_number; ++i)
	{
		//cout << returned_table[i+0] << "\t" << returned_table[i+1] << "\t" << returned_table[i+2] << endl;
		cout << x_q_minus_y[3*i+0] << "\t" << x_q_minus_y[3*i+1] << "\t" << x_q_minus_y[3*i+2] << endl;
	}*/
	Frobenius_norm = LAPACKE_dlange(LAPACK_ROW_MAJOR, 'F', myMetric->point_dimension, 3, x_q_minus_y, 3);
	
	//cout << "Frobenius_norm : " << Frobenius_norm <<endl;
	//cout << "Returning : " << Frobenius_norm/(sqrt(myMetric->point_number)) <<endl;

	delete[] comformation_X_consec;
	
	delete[] comformation_X_consec_rotated;

	delete[] comformation_Y_consec;

	delete[] returned_table;

	delete[] x_q_minus_y;

	delete[] q_rotation_table;

	delete[] u_table;

	delete[] singular;

	delete[] v_t_table;

	delete[] stat;

	return Frobenius_norm/(sqrt(myMetric->point_number));
}


template <typename T>
void ListData<T>::DistanceConformationVectorHandle(ifstream& inputFile, ofstream& outputFile, Conf* myConf, Metrics* myMetric, ClusterTable* clusterTable, double** distance_matrix, int* centroids, int** clusterAssign, int L, int k, double*** all_conformation_table)
{
	ofstream experim;
	experim.open("experim.dat");		

	string GARBAGE;
	int iteration = 0;
	int r = 0;
	double* conformation_distance_vector = NULL;		//current distances will be used as key vector
	double* first_conf_distances = NULL;
	double** distance_pairs = NULL;			//distance pairs will be chosen from the first conformation each time

	int all_pairs_num = ((myMetric->point_dimension)*(myMetric->point_dimension - 1))/2;


	double** all_first_conf_distances = new double*[all_pairs_num];
	for (int array_iter = 0; array_iter < all_pairs_num; array_iter++)
	{
		all_first_conf_distances[array_iter] = new double[2];
	}

	AllFirstConfDistances(myMetric, all_conformation_table, all_first_conf_distances);

	quickSort_twolist(all_first_conf_distances, 0, all_pairs_num-1);

	double** clusteringData = new double*[7];
	for (int data_iter = 0; data_iter < 7; data_iter++)
	{
		clusteringData[data_iter] = new double[5];
	}

	double* currentData = new double[5];

	ListData<double*>* euclideanList = NULL;

	for(int r_select = 0; r_select < 3; r_select++)
	{
		if (r_select == 0)
		{	//N
			r = myMetric->point_dimension;
		}
		else if (r_select == 1)
		{	//N^1.5
			
			r = pow(myMetric->point_dimension, 1.5);
		}
		else if (r_select == 2)
		{	//(N*(N-1))/2
			r = all_pairs_num;
		}

		first_conf_distances = new double[r];		//which pairs should everyone cosider?




		distance_pairs = new double*[r];		//distance pairs will be chosen from the first conformation
		for (int r_new = 0; r_new < r; r_new++)
		{
			distance_pairs[r_new] = new double[2];
		}

		for (int T_select = 0; T_select < 3; T_select++)
		{		//T_select == 0 -> r smallest, T_select == 0 -> r biggest, T_select == 0 -> r random
			if (euclideanList != NULL)
			{
				delete euclideanList;
			}
			euclideanList = new ListData<double*>();
			if (T_select == 0)		//r smallest
			{
				//cout << "r smallest" <<endl;
				for (int r_iter = 0; r_iter < r; r_iter++)
				{
					first_conf_distances[r_iter] = all_first_conf_distances[r_iter][0];
					//cout << "best item : pair_no : " << all_first_conf_distances[r_iter][0] << " pair dis: " << all_first_conf_distances[r_iter][1] <<endl;
				}
				//cout << "######################################" <<endl;
			}
			else if (T_select == 1)		//r smallest
			{
				//cout << "r highest" <<endl;
				for (int r_iter = 0; r_iter < r; r_iter++)
				{
					first_conf_distances[r_iter] = all_first_conf_distances[all_pairs_num - 1 - r_iter][0];
					//cout << "best item : pair_no : " << all_first_conf_distances[all_pairs_num - 1 - r_iter][0] << " pair dis: " << all_first_conf_distances[all_pairs_num - 1 - r_iter][1] <<endl;
				}
				//cout << "######################################" <<endl;
			}
			else 
			{
				//cout << "r random" <<endl;
				int random_pair = 0;
				for (int r_iter = 0; r_iter < r; r_iter++)
				{
					random_pair = rand() % all_pairs_num;
					first_conf_distances[r_iter] = all_first_conf_distances[random_pair][0];
					//cout << "best item : pair_no : " << all_first_conf_distances[random_pair][0] << " pair dis: " << all_first_conf_distances[random_pair][1] <<endl;
					//cout << "best item : random_pair_no : " << random_pair << " pair dis: " << all_first_conf_distances[random_pair][1] <<endl;
				}
				//cout << "######################################" <<endl;
			}

			quickSort(first_conf_distances, 0, r-1);

			for (int conformation_no = 0; conformation_no < myMetric->point_number; conformation_no++)
			{
				conformation_distance_vector = new double[r];		//to be inserted on euclideanList
				ConformationDistanceVector(myMetric, all_conformation_table, first_conf_distances, conformation_no, r, conformation_distance_vector);

				/*cout << "conformation " << conformation_no << " distance vector: " <<endl;
				for(int r_iter = 0; r_iter < r; r_iter++)
				{
					cout << conformation_distance_vector[r_iter] << "\t";
				}
				cout << endl;*/
				euclideanList->Insert(conformation_distance_vector, conformation_no, "current_backbone_atom", &conformation_distance_vector);
			}

			euclideanList->ClusterHandleExercise3( inputFile, experim, myConf, myMetric, clusterTable, distance_matrix, centroids, clusterAssign, L,  k, 0, currentData, true);
			
			clusterAssign= new int*[myMetric->point_number];	//deleting clusterAssign on first round inside clusterhandexercise3
			for (int i = 0; i < myMetric->point_number; ++i)
			{
			    clusterAssign[i] = new int[3];
			    clusterAssign[i][0] = -1;
			    clusterAssign[i][1] = -1;
			    clusterAssign[i][2] = -1;
			}
			clusterTable = new ClusterTable(myConf->number_of_clusters);


			currentData[0] = r_select;		//current r
			currentData[1] = T_select; // current T
			for(int data_iter = 0; data_iter < 5; data_iter++)
			{
				clusteringData[iteration][data_iter] = currentData[data_iter];
			}

			experim << "#####################################" <<endl;
			experim << "Above is with:" <<endl;
			experim << "r: ";
			switch((int)currentData[0])
			{
			   	case 0 :
			        experim << "Ν" << endl; 
			        break;
			    case 1 :
			    	experim << "N^{1.5}" <<endl;
			    	break;
			    case 2 :
			        experim << "All distance pairs" << endl;
			        break;
			    default :
			    	break;
			}
			experim << "T: ";
			switch((int)currentData[1])
			{
			   	case 0 :
			        experim << "min distance pairs" << endl; 
			        break;
			    case 1 :
			    	experim << "max distance pairs" <<endl;
			    	break;
			    case 2 :
			        experim << "random distance pairs" << endl;
			        break;
			    default :
			    	break;
			}
			experim << "k: " << currentData[2] <<endl;
			experim << "Silhouette: " << currentData[3] <<endl;
			experim << "Clustering time: " << currentData[4] <<endl;

			experim << "#####################################" <<endl;


			iteration++;


			if (r == all_pairs_num)
			{
				T_select = 3;
			}
			cout << "above is r_select: " <<r_select << " T_select: " << T_select <<endl;

		}

		for (int r_new = 0; r_new < r; r_new++)
		{
			delete[] distance_pairs[r_new];
		}
		delete[] distance_pairs;		//distance pairs will be chosen from the first conformation

		delete[] first_conf_distances;

	}

	delete euclideanList;

	for (int array_iter = 0; array_iter < all_pairs_num; array_iter++)
	{
		delete[] all_first_conf_distances[array_iter];
	}

	delete[] all_first_conf_distances;
	

	for (int data_iter = 0; data_iter < 7; data_iter++)
	{
		delete[] clusteringData[data_iter];
	}

	delete[] clusteringData;
	

	delete[] currentData;

	delete clusterTable;

	for (int i = 0; i < myMetric->point_number; ++i)
	{
		delete[] clusterAssign[i];
	}
	delete[] clusterAssign;
}

template <typename T>
void ListData<T>::AllFirstConfDistances(Metrics* myMetric, double*** all_conformation_table, double** all_first_conf_distances)
{
	int distance_pair_counter = 0;
	int current_conformation_no = 0;		//for first conformation
	//for every backbone atom
	for(int current_backbone_atom_no = 0; current_backbone_atom_no < myMetric->point_dimension; current_backbone_atom_no++)
	{
		//for every next backbone atom
		for (int next_backbone_atom_no = current_backbone_atom_no + 1; next_backbone_atom_no < myMetric->point_dimension; next_backbone_atom_no++)
		{
			all_first_conf_distances[distance_pair_counter][0] = distance_pair_counter;
			all_first_conf_distances[distance_pair_counter][1] = DistanceEuclid(all_conformation_table[0][current_backbone_atom_no],all_conformation_table[0][next_backbone_atom_no] , 3);
			//cout << "Distance of pair " << all_first_conf_distances[distance_pair_counter][0] << " : " << all_first_conf_distances[distance_pair_counter][1] <<endl;
			
			distance_pair_counter++;		//first backbone atom with second have pair_no = 0, first with third pair_no = 1 etc.
		}
	}
}

template <typename T>			//first conf distances is sorted from small to big
void ListData<T>::ConformationDistanceVector(Metrics* myMetric, double*** all_conformation_table, double* first_conf_distances, int conformation_no, int r, double* conformation_distance_vector)
{
	int distance_pair_counter = 0;
	int r_iter = 0;
	int all_pairs_num = ((myMetric->point_dimension)*(myMetric->point_dimension - 1))/2;
	//for every atom in the given atom
	for(int current_backbone_atom_no = 0; current_backbone_atom_no < myMetric->point_dimension; current_backbone_atom_no++)
	{
		//for every next backbone atom
		for (int next_backbone_atom_no = current_backbone_atom_no + 1; next_backbone_atom_no < myMetric->point_dimension; next_backbone_atom_no++)
		{
			if (distance_pair_counter == first_conf_distances[r_iter])
			{
				conformation_distance_vector[r_iter] = DistanceEuclid(all_conformation_table[conformation_no][current_backbone_atom_no], all_conformation_table[conformation_no][next_backbone_atom_no] , 3);
				r_iter++;
				if (r_iter < r)
				{
					if (first_conf_distances[r_iter] == first_conf_distances[r_iter - 1])
					{
						if(first_conf_distances[r_iter] + 1 < all_pairs_num)
						{
							first_conf_distances[r_iter]++;
						}
						else		//leave it as zero - it's just one element
						{

						}
						
					}
				}
				
			}
			if (r_iter == r)
			{
				return;
			}
			distance_pair_counter++;		//first backbone atom with second have pair_no = 0, first with third pair_no = 1 etc.
		}
	}
}



