#include "Headers.h"
#include "ListData.h"

template <typename T>
void ListData<T>::ListInsertionVector(std::ifstream& inputFile, Metrics* myMetric)
{
   		string GARBAGE;
   		string genericStr;
   		string pointStr;
   		string itemNos;
   		double** currentConformation = new double*[myMetric->point_dimension];
        for (int backbone_atom_index = 0; backbone_atom_index < myMetric->point_dimension; backbone_atom_index++)
        {
            currentConformation[backbone_atom_index] = new double[3];
        }
   		//bitset<64> currentPoint;
   		int count_ins = 0;
   		int point_number = 0;
   		int index;
   		double* point;
   		double* ratings;
   		double items_rated = 0;
   		double total_rating = 0;
   		double R_u = 0;

        inputFile.clear();          //Restart
        inputFile.seekg(0, ios::beg);   //Data file back from start

        inputFile >> GARBAGE;   //Read noConform
        inputFile >> GARBAGE;   //Read N
        getline(inputFile, genericStr);

        //for every conformation
        for (int current_conformation_no = 0; current_conformation_no < myMetric->point_number; current_conformation_no++)
        {
            //for every atom of a conformation (point)
            for (int current_backbone_atom = 0; current_backbone_atom < myMetric->point_dimension; current_backbone_atom++)
            {
                getline(inputFile, genericStr);
                //cout << "line : " << genericStr <<endl;
                stringstream linestream(genericStr);
                //for every dimension of a point : (x,y,z)
               for (int current_point_dimension = 0; current_point_dimension < 3; current_point_dimension++)
               {
                    getline(linestream, pointStr, '\t');
                    currentConformation[current_backbone_atom][current_point_dimension] = stod(pointStr);
                    //cout << "read : " << stod(pointStr) <<endl;
               }
               
            }

            this->Insert(currentConformation, current_conformation_no, "current_backbone_atom", currentConformation);
            currentConformation = new double*[myMetric->point_dimension];
            for (int backbone_atom_index = 0; backbone_atom_index < myMetric->point_dimension; backbone_atom_index++)
            {
                currentConformation[backbone_atom_index] = new double[3];
            }

        }

}


template <typename T>
void ListData<T>::DistanceMatrixComputationVector(Metrics* myMetric, double** distance_matrix, double*** all_conformation_table) {

	for (int i = 0; i < myMetric->point_number; i++) {
		cout << "item " << i << "\t";
		for (int j = 0; j < myMetric->point_number; j++) {
			if (j <= i) {
				distance_matrix[i][j] = 0;
			}
			else 
            {
                distance_matrix[i][j] = this->cRMSD(myMetric, all_conformation_table[i], all_conformation_table[j]);
			}
		}
	}
    cout <<endl;
}


template <typename T>         //items are point_no
double ListData<T>::SimilarityEuclidean(Metrics* myMetric, double** distance_matrix, double item1, double item2)
{
   return 1/(1+ DistanceMatrixDistance(distance_matrix, item1, item2));
}

template <typename T>      // items are vectors
double ListData<T>::SimilarityCosine(Metrics* myMetric, double* item1, double* item2)
{
   //return cosine similarity: sim(u,v) = <u,v>/(||u||*||v||)
   return (dot_product(item1, item2, myMetric->point_dimension)/(euclid_norm(item1, myMetric->point_dimension)*euclid_norm(item2, myMetric->point_dimension)));
}