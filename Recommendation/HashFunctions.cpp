#include "Hash.h"


using namespace std;

//HEADHASHNODE FUNCTIONS

template <typename T>
headHashNode<T>::headHashNode()
{
    bucket_key = -1;
    bucket = NULL;
}


template <typename T>
headHashNode<T>::~headHashNode() 
{
    if (bucket != NULL)
    {
        delete bucket;
        bucket = NULL;
        //cout << "HeadHashNode deleted bucket." << endl;
    }
    else
    {
        //cout << "HeadHashNode deleted else ." << endl;
    }
}


template <typename T>
int headHashNode<T>::getBucketKey() {
    return bucket_key;
}


template <typename T>
void headHashNode<T>::setBucketKey(int bucket_key) {
    this->bucket_key = bucket_key;
}


template <typename T>
Node<T>* headHashNode<T>::getBucket()
{
    return bucket;
}


template <typename T>
void headHashNode<T>::setBucket(Node<T>* bucket)
{
    this->bucket = bucket;
}


template <typename T>
int headHashNode<T>::Insert(int hashResult, Node<T>* newItem, int g)
{
    newItem->setNext(this->bucket);
    newItem->setG(g);
    this->bucket = newItem;
    return 0;
}


template <typename T>
void headHashNode<T>::InsertTrick(int g, TrickList<T>* trickList, int L) {
    Node<T>* currentNode;
    currentNode = this->bucket;
    while(currentNode != NULL) {
        if (currentNode->getG() == g) {
            trickList->Insert(currentNode, L);
            //cout << "Added in trickList : " << currentNode->getKey()[0] << endl;
        }
        //cout << "NodeID: " << currentNode->getG() << " - g: " <<g <<endl;
        currentNode = currentNode->getNext();
    }
}

/*
template <typename T>
void headHashNode<T>::Duplicate(T item)
 {
    Node<T> *currentNode = bucket;
    while (currentNode != NULL)
    {
        if (currentNode->getKey() )
    }
}*/


template <typename T>
void headHashNode<T>::printHash() {
    cout << endl;
    cout << ">>>>> Printing Bucket" <<endl;
    if (this->bucket_key == -1) {
        cout << "Bucket empty" << endl;
    }
    else {
        cout << ">> Bucket key: " << bucket_key << endl;
        Node<T>* current;
        current = this->bucket;

        while (current != NULL) {
            cout << "Item:" << current->getKey() <<endl;
            current = current->getNext();
        }

        cout << ">>>>> " <<endl;
        cout << endl;
    }
}


template <typename T>
Hash<T>::Hash()
{
	hashTable = NULL;
}


template <typename T>
void Hash<T>::initHash(int tableSize) {
/*    int tableSize;
    //cout << "metric_space " << metric << endl;
    if (strcmp(metric.c_str(), "hamming") == 0) {
        tableSize = pow(2, k);
    }
    if (strcmp(metric.c_str(), "euclidean") == 0) {
        tableSize = k;
    }
    if (strcmp(metric.c_str(), "cosine") == 0) {
        tableSize = pow(2, k);
    }
    if (strcmp(metric.c_str(), "matrix") == 0) {
        tableSize = pow(2, k);
    }*/
    this->tableSize = tableSize;
	this->hashTable = new headHashNode<T>[tableSize]();
    //this->metric_space = metric;
}


template <typename T>
headHashNode<T>* Hash<T>::getHashTable()
{
    return hashTable;
}


template <typename T>
void Hash<T>::Insert(int hashResult, T newItem, int g, int itemno, string itemName) {
    if (this->hashTable[hashResult].getBucketKey() == -1) {       //First item on this bucket
        this->hashTable[hashResult].setBucketKey(hashResult);
    }

    Node<T>* newNode = new Node<T>(newItem, g, itemno, itemName);

    this->hashTable[hashResult].Insert(hashResult, newNode, g);
    //cout << "Printing in Hash<T>::Insert just insert: " << hashTable[hashResult].getBucket()->getItemNo()<<endl;
}

template <typename T>
void Hash<T>::MoveToBack(int item_no, int cluster_no) 
{
    Node<T>* currentNode = hashTable[cluster_no].getBucket();
    Node<T> *prev = NULL;
    Node<T> *list = currentNode;
    Node<T> *list2 = currentNode;
    if (currentNode != NULL) 
    {
        while ((list->getNext() != NULL) && (list->getItemNo() != item_no)) 
        {
            prev = list;
            list = list->getNext();
        }
        if (list->getItemNo() == item_no) 
        {
            if (prev == NULL) 
            {
                Node<T> *next= list->getNext();
                //delete list;
                hashTable[cluster_no].setBucket(next);
            } 
            else 
            {
                Node<T> *_next = list->getNext();
                //delete list;
                prev->setNext(_next);
            }
        }
    }
    currentNode = list;
    list = hashTable[cluster_no].getBucket();
    while (list != NULL)
    {
        prev = list;
        list = list->getNext();
    }
    currentNode->setNext(NULL);
    if (prev == NULL)
    {
        hashTable[cluster_no].setBucket(currentNode);
    }
    else
    {
        prev->setNext(currentNode);
    }
    //cout << "Item inserted in hash table with hash : " << cluster_no <<endl;
}




template <typename T>
Hash<T>::~Hash()
{
    if (hashTable != NULL)
    {
        delete[] hashTable;
        hashTable = NULL;
        //cout << "Hash table deleted." << endl;
    }
    else
    {
        //cout << "Hash table deleted." << endl;
    }
}

template <typename T>
void Hash<T>::ReInitializeHashTable(int L, int tableSize)
{
    Node<T>* nodePtr;

    for (int bucket = 0; bucket < tableSize; bucket++)  //for every bucket of the hash table
    {
        nodePtr = hashTable[bucket].getBucket();
        while (nodePtr != NULL)
        {
            nodePtr->setFlagForAssignment(0);
            nodePtr->setFlagAsAssigned(0);
            nodePtr->setDistanceFromCentroid(INT_MAX);
            nodePtr->setCentroid(-1);
            nodePtr->setSecondBestCentroid(-1);
            nodePtr->setSecondBestDistance(INT_MAX);
            nodePtr = nodePtr->getNext();
        }
    }
}


template <typename T>
void Hash<T>::printHash() {
    cout << endl;
    cout << "------- Printing Hash Table -------" <<endl;
    for (int i = 0; i < tableSize; i++) {
        this->hashTable[i].printHash();
    }
    cout << "------- ------------------- -------" <<endl;
    cout << endl;
}