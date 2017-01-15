#include "Headers.h"
#include "Algorithms.h"
#include "UtilsH.h"


bool ALaLoyds(Conf* myConf, Metrics* myMetric, double** distanceMatrix, int* centroids,  ClusterTable* clusterTable, int** clusterAssign)
{
	int newMedoid;
	int cluster_no;
	bool changed = false;

	for (int i = 0; i < myConf->number_of_clusters; ++i)
	{
		newMedoid = clusterTable->ClusterDistance(myMetric, distanceMatrix, i, clusterAssign);
		//cout << "newmedoid is: " <<newMedoid <<endl;
		if (newMedoid == -1)		//centroid is alone in this cluster
		{
			//cout << "There was a serious problem in the previous assignment - Aborting" <<endl;
			//exit(-1);
		}
		else if (newMedoid != centroids[i]) {
			changed = true;
			for (int j = 0; j < myMetric->point_number; ++j)		//updating clusterAssign
			{
				if(clusterAssign[j][0] == centroids[i]) {
					clusterAssign[j][0] = newMedoid;
				}
				if(clusterAssign[j][1] == centroids[i]) {
					clusterAssign[j][1] = newMedoid;
				}
				if(clusterAssign[j][2] == centroids[i]) {
					clusterAssign[j][2] = newMedoid;
				}
			}
			centroids[i] = newMedoid;
		}
	}
	
	return changed;
}
