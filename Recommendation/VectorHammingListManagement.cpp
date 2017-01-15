#include "Headers.h"
#include "ListData.h"

template <typename T>
void ListData<T>::ListInsertionVector(std::ifstream& inputFile, Metrics* myMetric)
{
   		string GARBAGE;
   		string genericStr;
   		string pointStr;
   		string itemNos;
   		double* currentPoint = new double[myMetric->point_dimension];
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


   		if (strcmp(myMetric->metric_space.c_str(), "vector") == 0)
   		{
   			getline(inputFile, genericStr);
   			ratings = new double[myMetric->point_dimension];
   			for (int i = 0; i < myMetric->point_dimension; ++i)
   			{
   				ratings[i] = 0;
   			}
   			do {

   				stringstream linestream(genericStr);
   				getline(linestream, pointStr, '\t');
   				//cout << pointStr <<endl;
   				if (stoi(pointStr) != point_number)	//if we are changing user
   				{
   					
   					if (point_number != 0)		//we are changing user
   					{
   						//cin >> GARBAGE;

   						R_u = (double)(total_rating / items_rated);
   						for (int i = 0; i < myMetric->point_dimension; ++i)		//for loop to change to R'(u,i) = R(u,i) - R(u)
   						{														//for every rated item
   							if (ratings[i] != 0)								//currentPoint holds that
   							{
   								currentPoint[i] = ratings[i] - R_u;
   							}
   						}
   						
   						/*cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" <<endl;
   						cout << "Inserting: ";
   						for (int i = 0; i < myMetric->point_dimension; ++i)
   						{
   							cout << currentPoint[i] << " ";
   						}
   						cout <<endl;
   						cout << "Ratings: ";
   						for (int i = 0; i < myMetric->point_dimension; ++i)
   						{
   							cout << ratings[i] << " ";
   						}
   						cout << endl;
   						cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" <<endl;
                     cin >> GARBAGE;*/

   						this->Insert(currentPoint, count_ins, to_string(point_number), ratings);

  						items_rated = 0;
  						total_rating = 0;

   						count_ins++;
   					}
   					point_number = stoi(pointStr);
   					ratings = new double[myMetric->point_dimension];
   					currentPoint = new double[myMetric->point_dimension];	//new currentpoint
   					for (int i = 0; i < myMetric->point_dimension; ++i)
   					{
   						currentPoint[i] = 0; 		//reset currentpoint
   						ratings[i] = 0;
   					}
   				}
   				getline(linestream, pointStr, '\t');
   				index = stoi(pointStr);
   				getline(linestream, pointStr, '\t');
   				ratings[index-1] = stoi(pointStr);
   				if (stoi(pointStr) !=0)			//these used for loop to change to R'(u,i) = R(u,i) - R(u) for every rated item
   				{
   					items_rated++;
   					total_rating += ratings[index-1];
   				}

   	   		}while(getline(inputFile, genericStr));
   	   		R_u = (double)(total_rating / items_rated);
   	   		for (int i = 0; i < myMetric->point_dimension; ++i)		//for loop to change to R'(u,i) = R(u,i) - R(u)
   	   		{														//for every rated item
   	   			if (ratings[i] != 0)								//currentPoint holds that
   	   			{
   	   				currentPoint[i] = ratings[i] - R_u;
   	   			}
   	   		}
   	   		this->Insert(currentPoint, count_ins, to_string(point_number), ratings);
   	   		count_ins++;
   		}

}


template <typename T>
void ListData<T>::DistanceMatrixComputationVector(Metrics* myMetric, double** distance_matrix) {
	Node<T>* driver_node;
	Node<T>* current_node;

	driver_node = this->header;

	for (int i = 0; i < myMetric->point_number; i++) {
		current_node = driver_node->getNext();
		cout << "item " << i << "\t";
		for (int j = 0; j < myMetric->point_number; j++) {
			if (j <= i) {
				distance_matrix[i][j] = 0;
			}
			else {

				if (strcmp(myMetric->metric_space.c_str(), "vector") == 0)
				{
					if (strcmp(myMetric->metric.c_str(), "euclidean") == 0)
					{
						distance_matrix[i][j] = DistanceEuclid(current_node->getKey(), driver_node->getKey(), myMetric->point_dimension);
						//cout << distance_matrix[i][j] << endl;
						current_node = current_node->getNext();

					}
					
					if (strcmp(myMetric->metric.c_str(), "cosine") == 0)
					{
						distance_matrix[i][j] = this->CosineDistance(current_node->getRatings(), driver_node->getRatings(), myMetric->point_dimension);
						//cout << distance_matrix[i][j] << "\t";
						current_node = current_node->getNext();
					}
				}
			}
		}
		driver_node = driver_node->getNext();
	}
   cout << endl;
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