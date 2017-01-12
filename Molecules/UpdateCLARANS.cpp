#include "Headers.h"
#include "UtilsH.h"
#include "Algorithms.h"



bool CLARANS(Conf* myConf, Metrics* myMetric, double** distanceMatrix, int* centroids,  ClusterTable* clusterTable, int** clusterAssign)
{
	//string GARBAGE;
	int newMedoid;
	bool changeflag = false;
	int cluster;
	bool changed = false;
	int randomCentroid, randomNonCentroid;
	int minimumCentroid, minimumNonCentroid;
	double currentSubtraction;
	double maximumSubtraction;

	std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
	std::cout.precision(20);

	for (int i = 0; i < myConf->clarans_iterations; ++i)
	{
		changeflag = false;
		maximumSubtraction = INT_MIN;
		if (i > 0) {
		}
		PAM(myConf, myMetric, distanceMatrix, centroids, clusterTable, clusterAssign);
		/*for (int cluster_iter = 0; cluster_iter < myConf->number_of_clusters; cluster_iter++)
		{
			clusterTable->PrintClusterNo(cluster_iter);
		}*/
		for (int j = 0; j < myConf->clarans_set_fraction; j ++) {
			randomCentroid = (int)(((double)rand() / (double) RAND_MAX)*(myConf->number_of_clusters));
			if (randomCentroid == myConf->number_of_clusters) 
			{
				randomCentroid--;
			}
			do 
			{
				randomNonCentroid = (int)(((double)rand() / (double) RAND_MAX)*(myMetric->point_number));
				if (randomNonCentroid == myMetric->point_number) 
				{
					randomCentroid--;
				}

			}while(Exists(centroids, myConf->number_of_clusters, randomNonCentroid));
			cluster = randomCentroid;
			currentSubtraction = clusterTable->ClusterDistanceFromCentroid(distanceMatrix, cluster, centroids[randomCentroid]) - clusterTable->ClusterDistanceFromCentroid(distanceMatrix, cluster, randomNonCentroid);
			if (currentSubtraction > 0)
			{
				if (currentSubtraction > maximumSubtraction)
				{
					maximumSubtraction = currentSubtraction;
					//cout << "chan is the best we have - maximumSubtraction is  : " << maximumSubtraction << endl;
					minimumCentroid = randomCentroid;
					//cout << "minimumCentroid : " << minimumCentroid << endl;
					minimumNonCentroid = randomNonCentroid;
					//cout << "minimumNonCentroid : " << minimumNonCentroid << endl;
					changeflag = true;
				}
			}
		}
		if (!changeflag) 
		{
			return (int)changeflag;
		}

		//swapping
		/*cout << "==================" << endl << "PRINTING CLUSTERS : " <<endl;
		for (int w = 0; w <myConf->number_of_clusters; w++) {
			cout << centroids[w] << " ";
		}
		cout <<endl;*/
		
		cluster = minimumCentroid;

		//cout << "Changing " << centroids[minimumCentroid] <<" with " << minimumNonCentroid <<endl;
		for (int j = 0; j < myMetric->point_number; ++j)		//updating clusterAssign
		{
			//cout << "current minimum centroid " << j << " "  << " - " << clusterAssign[j][0] <<endl;
			if(clusterAssign[j][0] == centroids[minimumCentroid]) {
				//cout << "changed0" <<endl;
				clusterAssign[j][0] = minimumNonCentroid;
			}
			//cout << "current minimum centroid " << j << " "  << " - " << clusterAssign[j][1] <<endl;
			if(clusterAssign[j][1] == centroids[minimumCentroid]) {
				//cout << "changed1" <<endl;
				clusterAssign[j][1] = minimumNonCentroid;
			}
			//cout << "current minimum centroid " << j << " "  << " - " << clusterAssign[j][2] <<endl;
			if(clusterAssign[j][2] == centroids[minimumCentroid]) {
				//cout << "changed2" <<endl;
				clusterAssign[j][2] = minimumNonCentroid;
			}
		}

		centroids[cluster]  = minimumNonCentroid;

	}
	return changed;
}