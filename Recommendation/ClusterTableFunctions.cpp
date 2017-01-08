#include "Headers.h"
#include "ClusterTable.h"

ClusterTable::ClusterTable()
{
	number_of_clusters = -1;
	clusterTable = NULL;
	//cout << "------->  ClusterTable initialized successfully!" << endl << endl;
}

ClusterTable::ClusterTable(int number_of_clusters)
{
	this->number_of_clusters = number_of_clusters;
	this->clusterTable = new ClusterNode*[number_of_clusters];
    //cout << "list actuel: " << clusterTable << endl;
	for (int i = 0; i < number_of_clusters; ++i)
	{
		this->clusterTable[i] = NULL;
	}
	//cout << "------->  ClusterTable initialized successfully!" << endl << endl;
}


ClusterTable::~ClusterTable()
{
	for (int i = 0; i < number_of_clusters; i++)
   	{
	    if (clusterTable[i] != NULL) 
		{
	   		ClusterNode *prev = NULL;
	      	ClusterNode *list = clusterTable[i];
	      	while (list != NULL) 
	      	{
	        	prev = list;
	           	list = list->getNext();
	           	delete prev;
	      	}
		}
	}
}



ClusterNode** ClusterTable::getArray()
{
    return clusterTable;
}


void ClusterTable::setArray(ClusterNode** clusterTable)
{
    this->clusterTable = clusterTable;
}


ClusterNode* ClusterTable::getList()
{
    return *clusterTable;
}


void ClusterTable::setList(ClusterNode* table)
{
    *clusterTable = table;
}


int ClusterTable::getClusterNumber()
{
	return number_of_clusters;
}


void ClusterTable::setClusterNumber(int number_of_clusters)
{
	this->number_of_clusters = number_of_clusters;
}




void ClusterTable::Remove(int item_no, int cluster_no) 
{

	if (cluster_no == -1)
	{
		return;
	}
    if (clusterTable[cluster_no] != NULL) 
    {
    	ClusterNode *prev = NULL;
        ClusterNode *list = clusterTable[cluster_no];
        while ((list->getNext() != NULL) && (list->getItemNo() != item_no)) 
        {
        	prev = list;
            list = list->getNext();
        }
        if (list->getItemNo() == item_no) 
        {
        	if (prev == NULL) 
            {
            	ClusterNode *next= list->getNext();
                delete list;
                clusterTable[cluster_no] = next;
            } 
            else 
            {
            	ClusterNode *_next = list->getNext();
                delete list;
            	prev->setNext(_next);
            }
        }
    }
}





void ClusterTable::InsertAtCluster(int item_no, int cluster_no) 
{
    if (cluster_no == -1) {
        //cout << "This should not happen - 908" <<endl;
        return;
    }
	ClusterNode* prev = NULL;
    ClusterNode* list = this->clusterTable[cluster_no];
    ClusterNode* newNode;
    while (list != NULL)
    {
        prev = list;
        list = list->getNext();
    }
    newNode = new ClusterNode(item_no, NULL);
    if (prev == NULL)
    {
        clusterTable[cluster_no] = newNode;
    }
    else
    {
        prev->setNext(newNode);
    }

    //cout << "Item inserted in cluster table with cluster : " << cluster_no <<endl;
    list = this->clusterTable[cluster_no];
}

//returns medoid :S
int ClusterTable::ClusterDistance(Metrics* myMetric, double** distanceMatrix, int cluster_no, int** clusterAssign)      //return medoid
{
    double minDistance = INT_MAX;
    double clusterDistance;
    int minDistanceMedoid = -1;

    ClusterNode* driverNode = this->clusterTable[cluster_no];     //used without clusterAssign, only table -FASTER
    ClusterNode* currentNode;
    while (driverNode != NULL) 
    {
        clusterDistance = 0;
        currentNode = this->clusterTable[cluster_no];
        while (currentNode != NULL) {
            clusterDistance += DistanceMatrixDistance(distanceMatrix, driverNode->getItemNo(), currentNode->getItemNo());
            //  cout << "curr dis: " << clusterDistance <<endl;
            currentNode = currentNode->getNext();
        }
        if (clusterDistance < minDistance) {
            minDistance = clusterDistance;
            minDistanceMedoid = driverNode->getItemNo();
        }
        driverNode = driverNode->getNext();
    }
    //cout << "giving " << minDistanceMedoid <<" back" <<endl;

    return minDistanceMedoid;
}


double ClusterTable::ClusterDistanceFromCentroid(double** distanceMatrix, int cluster_no, int centroid)      //return distance of cluster from point
{
    double minDistance = INT_MAX;
    double clusterDistance = 0;
    int minDistanceMedoid = -1;
    ClusterNode* currentNode;
    currentNode = this->clusterTable[cluster_no];
    while (currentNode != NULL) {
        //cout << "in while" << endl;
        clusterDistance += DistanceMatrixDistance(distanceMatrix, centroid, currentNode->getItemNo());
        //cout << "curr dis: " << clusterDistance <<endl;
        currentNode = currentNode->getNext();
    }
    return clusterDistance;
}

void ClusterTable::Init_Tables(double*** distance_matrix, Metrics* myMetric, Conf* myConf, int** centroids, ClusterTable** clusterTable, int*** clusterAssign)
{
    (*distance_matrix) = new double*[myMetric->point_number];       //distance matrix creation
    for (int i = 0; i < myMetric->point_number; i++) {
        (*distance_matrix)[i] = new double[myMetric->point_number];
    }
    (*centroids) = new int[myMetric->point_number];
    for (int i = 0; i < myMetric->point_number; ++i)
    {
        (*centroids)[i] = 0;
    }

    (*clusterTable) = new ClusterTable(myConf->number_of_clusters);
    (*clusterAssign)= new int*[myMetric->point_number];
    for (int i = 0; i < myMetric->point_number; ++i)
    {
        (*clusterAssign)[i] = new int[3];
        (*clusterAssign)[i][0] = -1;
        (*clusterAssign)[i][1] = -1;
        (*clusterAssign)[i][2] = -1;
    }
}

int ClusterTable::ReturnClusterSize(int cluster_no)
{
    ClusterNode* currentNode = clusterTable[cluster_no];
    if (currentNode == NULL)
    {
        //cout << "exiting..." << endl;
    }

    int count_items = 0;
    while(currentNode != NULL)
    {
        count_items++;
        currentNode = currentNode->getNext();
    }
    return count_items;
}


int* ClusterTable::PrintClusterDataForList(int cluster_no, int* size_of_cluster)      //sends a list of item numbers to be processed on ListData
{
    int index = 0;
    *size_of_cluster = 0;
    ClusterNode* currentNode = clusterTable[cluster_no];
    int* items_in_cluster;
    while (currentNode != NULL)
    {
        (*size_of_cluster) = (*size_of_cluster) + 1;
        currentNode = currentNode->getNext();
    }
    if ((*size_of_cluster) == 0) 
    {
        return NULL;
    }
    items_in_cluster = new int[*size_of_cluster];

    currentNode = clusterTable[cluster_no];
    while (currentNode != NULL)
    {
        items_in_cluster[index] = currentNode->getItemNo();
        currentNode = currentNode->getNext();
        index++;
    }
    return items_in_cluster;
}

void ClusterTable::PrintClusterUsingNames(ofstream& outputFile, string* items_in_cluster_itemName, int size_of_cluster, int cluster_no)      //sends a list of item numbers to be processed on ListData
{
    outputFile << "CLUSTER-" << cluster_no +1 << " {";
    for (int item_iter = 0; item_iter <= size_of_cluster; item_iter++)
    {
        outputFile << items_in_cluster_itemName[item_iter];
        if (item_iter == size_of_cluster || items_in_cluster_itemName[item_iter].compare("") == 0)
        {
            break;
        }
        if (item_iter < size_of_cluster && items_in_cluster_itemName[item_iter+1].compare("") != 0) 
        {
            outputFile << ", ";
        }
        
    }
    outputFile << "}" <<endl;
}


void ClusterTable::PrintClusterNo(int cluster_no)
{
    ClusterNode* currentNode = clusterTable[cluster_no];
    if (currentNode == NULL)
    {
        //cout << "exiting..." << endl;
        //exit(1);
        return;
    }
    cout << "CLUSTER-" << cluster_no +1 << " {";
    while(currentNode != NULL)
    {
        cout << currentNode->getItemNo();
        currentNode = currentNode->getNext();
        if (currentNode == NULL) 
        {
            break;
        }
        cout << ", ";
    }
    cout << "}" <<endl;
}



double ClusterTable::ClusterSilhouette(Conf* myConf, double** distanceMatrix, int* centroids,  int cluster_no, int** clusterAssign)
{
    double avg_silh = 0;
    int number_in_cluster = 0;          //number of items in cluster
    int number_in_scnd_cluster = 0;     //number of items in second best cluster
    double b_i, a_i;
    int number_of_scnd_cluster;         //index for second best cluster in clusterTable
    ClusterNode* currentNode = clusterTable[cluster_no];
    int times = 0;
    string GARBAGE;

    if (currentNode == NULL)
    {
        cout << "cluster empty" <<endl;
        return 1;
        //cout << "exiting..." << endl;
    }

    ClusterNode* secondNode = NULL;
    number_in_cluster = ReturnClusterSize(cluster_no);
    cout << "current cluster size: " << number_in_cluster <<endl;
    if(number_in_cluster > 0)
    {
        currentNode = clusterTable[cluster_no];
    }
    while (currentNode != NULL) {
        ++times;
        number_of_scnd_cluster = ReturnCluster(myConf, centroids, clusterAssign[currentNode->getItemNo()][1]);
        cout << "number_of_scnd_cluster : " << number_of_scnd_cluster <<endl;
        cout << "THIS CENTROID : " << clusterAssign[currentNode->getItemNo()][1] << endl;
            cout << "==================" << endl << "PRINTING CLUSTERS IN PrintingSilhouette fucntion : " <<endl;
            for (int w = 0; w <myConf->number_of_clusters; w++) {
                cout << centroids[w] << " ";
            }
            cout << endl;
        secondNode = clusterTable[number_of_scnd_cluster];
        number_in_scnd_cluster = ReturnClusterSize(number_of_scnd_cluster);
        cout << "number_in_scnd_cluster : " <<number_in_scnd_cluster <<endl;
        a_i = (double)ClusterDistanceFromCentroid(distanceMatrix, cluster_no, currentNode->getItemNo()) / (double) number_in_cluster;
        cout << "   ----> Silhouette: a_i of " << currentNode->getItemNo() << " : " << a_i <<endl;
        b_i = (double) ClusterDistanceFromCentroid(distanceMatrix, number_of_scnd_cluster, currentNode->getItemNo()) / (double)number_in_scnd_cluster;
        cout << "   ----> Silhouette: b_i of " << currentNode->getItemNo() << " : " << b_i <<endl;
        if (number_in_scnd_cluster == 0) {
            b_i = 1;
        }
        if (a_i >= b_i) 
        {
            cout << "   ----> Silhouette: adding in avg: " << (double)(b_i - a_i) / (double) a_i <<endl;
            avg_silh += (double)(b_i - a_i) / (double) a_i;
            //cout << "   ----> Silhouette: current avg_silh : " <<avg_silh <<endl;
        }
        else
        {
            cout << "   ----> Silhouette: adding in avg: " << (double)(b_i - a_i) / (double) b_i <<endl;
            avg_silh += (double)(b_i - a_i) / (double) b_i;
            cout << "   ----> Silhouette: current avg_silh " << currentNode->getItemNo() << " : " << avg_silh <<endl;
        }
        currentNode = currentNode->getNext();
    }
    cout << "   ----> Silhouette: return silhouette :" << ((double)avg_silh/(double) number_in_cluster) << endl;
    return ((double)avg_silh/(double) number_in_cluster);
}


double ClusterTable::PrintingSilhouette(ofstream& outputFile, Conf* myConf, double** distanceMatrix, int* centroids, int** clusterAssign)
{
    double* s_i = new double[myConf->number_of_clusters];
    double s_total = 0;
    

    outputFile << "Silhouette: [";
    for (int i = 0; i < myConf->number_of_clusters; ++i)
    {
        s_i[i] = this->ClusterSilhouette(myConf, distanceMatrix, centroids,  i, clusterAssign);
        outputFile << s_i[i] << ", ";
        s_total += s_i[i];
    }
    delete[] s_i;
    outputFile << s_total / myConf->number_of_clusters << "]" <<endl;
}

double ClusterTable::ReturnSilhouette(ofstream& outputFile, Conf* myConf, double** distanceMatrix, int* centroids, int** clusterAssign)
{
    double* s_i = new double[myConf->number_of_clusters];
    double s_total = 0;
    

    //outputFile << "Silhouette: [";
    for (int i = 0; i < myConf->number_of_clusters; ++i)
    {
        s_i[i] = this->ClusterSilhouette(myConf, distanceMatrix, centroids,  i, clusterAssign);
        //outputFile << s_i[i] << ", ";
        s_total += s_i[i];
    }
    delete[] s_i;
    //outputFile << s_total / myConf->number_of_clusters << "]" <<endl;
    return s_total / myConf->number_of_clusters;
}

int ClusterTable::CreateClusterTableFromClusterAssign(Conf* myConf, Metrics* myMetric,int** clusterAssign, int* centroids)
{
    int cluster_no;
    for (int point_iter = 0; point_iter < myMetric->point_number; point_iter++)
    {
        //cout << "Creating clustertable - checking args: clusterAssign[point_iter][2]: " << clusterAssign[point_iter][2] <<endl;
        for (int centroid_iter = 0; centroid_iter < myConf->number_of_clusters; centroid_iter++)    //find centroid index
        {
            if (centroids[centroid_iter] == clusterAssign[point_iter][2]) 
            {
                cluster_no = centroid_iter;
            }
        }
        if (clusterTable[cluster_no] == NULL)
        {
            //cout << "Attempting to add on EMPTY cluster" <<endl;
        }
        else 
        {
            //cout << "Attempting to add on NONEMPTY cluster" <<endl;
        }
        if (!ClusterDuplicate(point_iter, cluster_no))
        {
            this->InsertAtCluster(point_iter, cluster_no);        //insert point on the cluster it was assigned to
        }
    }
}

bool ClusterTable::ClusterDuplicate(int point_no,int cluster_no)    //returns true if duplicate exists
{
    ClusterNode* currentNode = clusterTable[cluster_no];
    while (currentNode != NULL)
    {
        if (currentNode->getItemNo() == point_no)
        {
            return true;
        }
        currentNode = currentNode->getNext();
    }
    return false;
}


//returns the next point_no - if point_no_before == -1, then return the first point_no -- returns -1 if no more items could be found
int ClusterTable::ClusterItemNumberNext(int point_no_before,int cluster_no)    
{
    ClusterNode* currentNode = clusterTable[cluster_no];
    if (point_no_before == -1)
    {
        if (currentNode != NULL)
        {
           return currentNode->getItemNo();     //return the first items itemNumber
        }
        else 
        {
            return -1;
        }
    }
    else    //if we have more items in the cluster and the before element is on them
    {
        while (currentNode != NULL && currentNode->getItemNo() != point_no_before)
        {
            currentNode = currentNode->getNext();
        }
        if (currentNode!= NULL && currentNode->getNext() != NULL)
        {
            return (currentNode->getNext())->getItemNo();       //return the next items itemNumber
        }
        else
        {
            return -1;
        }

    }

}

