#include <iostream>
#include <string>
#include "bryHashMap.h"

using namespace std;

// constructor for the hashmap
bHash::bHash() {

    for(int i = 0; i < tableSize; i++)
    {
        hashTable[i] = new crashData;
        hashTable[i]->caseNumber = 0;
        hashTable[i]->latitude = 0;
        hashTable[i]->longitude = 0;
        hashTable[i]->totalVehicle = 0;
        hashTable[i]->next = nullptr;
    }
}

// Adds item into the hashmap
void bHash::AddItem(int caseNumber, double latitude, double longitude, int totalVehicle) {

    int bucket = Hash(caseNumber);

    if(hashTable[bucket]->caseNumber == 0)
    {
        hashTable[bucket]->caseNumber = caseNumber;
        hashTable[bucket]->latitude = latitude;
        hashTable[bucket]->longitude = longitude;
        hashTable[bucket]->totalVehicle = totalVehicle;
    }
    else
    {
        // chaining
        crashData* ptr = hashTable[bucket];
        crashData* n = new crashData;
        n->caseNumber = caseNumber;
        n->latitude = latitude;
        n->longitude = longitude;
        n->totalVehicle = totalVehicle;
        n->next = nullptr;
        while(ptr->next != nullptr)
        {
            ptr = ptr->next;

        }
        ptr->next = n;
    }
}

// Function for hashing
int bHash::Hash(int key) {
    // Hash function using modulo operation
    return key % 210000000 % tableSize;
}


//// Shows number of items in a bucket
//int bHash::NumberOfItemsInBucket(int bucket) {
//    int count = 0;
//
//    if(hashTable[bucket]->caseNumber == 0)
//    {
//        return count;
//    }
//    else
//    {
//        count++;
//        crashData* ptr = hashTable[bucket];
//        while(ptr->next != nullptr)
//        {
//            count++;
//            ptr = ptr->next;
//        }
//    }
//    return count;
//}

/// Show total number of elements in hashMap
//int bHash::GetTotalItems() {
//    int totalItems = 0;
//    for (int i = 0; i < tableSize; i++) {
//        crashData* ptr = hashTable[i];
//        while (ptr != nullptr && ptr->caseNumber != 0) {
//            totalItems++;
//            ptr = ptr->next;
//        }
//    }
//    return totalItems;
//}
//

/// Function to print table and values
//void bHash::PrintTable() {
//
//    // number of elements in bucket, and prints table
//    int number;
//    for (int i = 0; i < tableSize; i++)
//    {
//        number = NumberOfItemsInBucket(i);
//        cout << "------------------------" << endl;
//        cout << "Bucket =  " << i << endl;
//        cout << hashTable[i]->caseNumber << endl;
//        cout << hashTable[i]->latitude << endl;
//        cout << hashTable[i]->longitude << endl;
//        cout << hashTable[i]->totalVehicle << endl;
//        cout << "# of items = " << number << endl;
//        cout << "------------------------" << endl;
//    }
//}



