#include "Headers.h"
#include "Algorithms.h"
#include "UtilsH.h"


void PAM(Conf* myConf, Metrics* myMetric, double** distanceMatrix, int* centroids,  ClusterTable* clusterTable, int** clusterAssign)
{
	string GARBAGE;
	int size_of_cluster;
    int* new_centroids;
    int* best_centroids;
    int cluster_no = 0;
    double minDistance, minDistanceSecond;
    double secondBest;
    bool already_centroid;
	int column, row, j;

	for (int i = 0; i < myMetric->point_number; ++i)
	{
		minDistance = INT_MAX;
		secondBest = INT_MAX;
		for (int k = 0; k < myConf->number_of_clusters; ++k)
		{
			j = centroids[k];
			if (j != i)
			{
				if (j < i)
				{
					column = i;
					row = j;
				}
				else
				{
					column = j;
					row = i;
				}
				//cout << "Old dist: " << minDistance << " dist with " << j << " : " << distanceMatrix[row][column] <<endl;
				/*for (int exists_centroid = 0; exists_centroid < myConf->number_of_clusters; exists_centroid++) 
				{
					if ()
				}*/

				//already_centroid = find(begin(centroids), end(centroids), x) != end(centroids);

				if (distanceMatrix[row][column] < minDistance) //&& !already_centroid)
				{
					secondBest = minDistance;
					//cout << "secondBest -----> " << secondBest << endl;
					clusterAssign[i][1] = clusterAssign[i][0];
					//cout << "secondBest done change" << endl;
					minDistance = distanceMatrix[row][column];
					//cout << "actual min -----> " << minDistance << endl;
					clusterAssign[i][0] = j;
					//cout << "minDistance done change" << endl;
					if (clusterAssign[i][1] == -1)
					{
						minDistanceSecond = INT_MAX;
						for (int remain_cluster = k+1; remain_cluster < myConf->number_of_clusters; ++remain_cluster)
						{
							if (DistanceMatrixDistance(distanceMatrix, i, remain_cluster) < minDistanceSecond)
							{
								clusterAssign[i][1] = centroids[remain_cluster];
							}
						}
					}
				}
				
			}
		}

		if (clusterAssign[i][2] != clusterAssign[i][0]) 
		{
			if (clusterAssign[i][2] != -1) {
				cluster_no = ReturnCluster(myConf, centroids, clusterAssign[i][2]);
			}
			clusterTable->Remove(i, cluster_no);
			for(int k = 0; k < myConf->number_of_clusters; k++) {
				if (centroids[k] == clusterAssign[i][0]) 
				{
					if(!clusterTable->ClusterDuplicate(i,k))
					{
						clusterTable->InsertAtCluster(i, k);
					}
					break;
				}
			}
		
			clusterAssign[i][2] = clusterAssign[i][0];
		}
	}
}
