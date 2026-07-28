/*
    Title:      Programming Assignment, Part 2
    Author:     Logan Wolford, Jocelyn Smith, Killian Miller
    Date:       3/29/2026 - 3/31/2026
    Purpose:    Advanced Data Commerce Analysis
    Type:       C++
    Credit:     We did not work with anyone outside our group for this assignment
*/

//Libraries
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
using namespace std;
using namespace std::chrono;

//Structure for Item
struct Item {
    int id;
    double price;
    double rating;
    int purchases;
};

//Prototypes
void mergeSort(Item arrItems[], int startIndex, int endIndex, string feature, bool lowToHigh);
void quickSortLomuto(Item arrItems[], int startIndex, int endIndex, string feature, bool lowToHigh);
void quickSortHoare(Item arrItems[], int startIndex, int endIndex, string feature, bool lowToHigh);
void saveToFile(string fileName, Item mergeArray[], Item lomutoArray[], Item hoareArray[], int n, string feature);

int main(){
    //Add a variable to hold the number of items and make an array of that size
    const int totalItems = 50;
    Item arrItems[totalItems];

    /*
    I tried to do the try and catch method you talked about for our problem checkpoint 1. :)
    It is very cool and I didn't know you could do that with reading from files. It set's up
    a lot of protection to stop the program from shutting down without clear explanation, It also
    looks like it will help with knowing exactly what went wrong when it comes to the type 
    of error thrown.
    */
    try {
        ifstream inputFile;

        //Setting exceptions
        inputFile.exceptions(ifstream::failbit | ifstream::badbit);

        //Put data inside array
        inputFile.open("items.txt");
        for (int i = 0; i < totalItems; i++) {
            inputFile >> arrItems[i].id;
            inputFile >> arrItems[i].price;
            inputFile >> arrItems[i].rating;
            inputFile >> arrItems[i].purchases;
        }
        
        inputFile.close();
    } 
    catch (const ifstream::failure& e) {
        cerr << "File Operation Error: " << e.what() << endl;
        return 1;
    } 
    catch (const exception& e) {
        cerr << "General Error: " << e.what() << endl;
        return 1;
    }

    //Price: Low to High
    Item arrPriceMerge_ltoh[50];
    Item arrPriceLomuto_ltoh[50];
    Item arrPriceHoare_ltoh[50];
    
    for (int k = 0; k < 50; k++){
        arrPriceMerge_ltoh[k] = arrItems[k];
        arrPriceLomuto_ltoh[k] = arrItems[k];
        arrPriceHoare_ltoh[k] = arrItems[k];
    }
    //Run sorts
    mergeSort(arrPriceMerge_ltoh, 0, 49, "price", true);
    quickSortLomuto(arrPriceLomuto_ltoh, 0, 49, "price", true);
    quickSortHoare(arrPriceHoare_ltoh, 0, 49, "price", true);

    //Save results
    saveToFile("price_ltoh.txt", arrPriceMerge_ltoh, arrPriceLomuto_ltoh, arrPriceHoare_ltoh, 50, "price");

    //Price: High to Low
    Item arrPriceMerge_htol[50];
    Item arrPriceLomuto_htol[50];
    Item arrPriceHoare_htol[50];
    
    for (int k = 0; k < 50; k++){
        arrPriceMerge_htol[k] = arrItems[k];
        arrPriceLomuto_htol[k] = arrItems[k];
        arrPriceHoare_htol[k] = arrItems[k];
    }
    //Run sorts
    mergeSort(arrPriceMerge_htol, 0, 49, "price", false);
    quickSortLomuto(arrPriceLomuto_htol, 0, 49, "price", false);
    quickSortHoare(arrPriceHoare_htol, 0, 49, "price", false);

    //Save results
    saveToFile("price_htol.txt", arrPriceMerge_htol, arrPriceLomuto_htol, arrPriceHoare_htol, 50, "price");

    //Purchases: Low to High
    Item arrPurchMerge_ltoh[50];
    Item arrPurchLomuto_ltoh[50];
    Item arrPurchHoare_ltoh[50];
    
    for (int k = 0; k < 50; k++){
        arrPurchMerge_ltoh[k] = arrItems[k];
        arrPurchLomuto_ltoh[k] = arrItems[k];
        arrPurchHoare_ltoh[k] = arrItems[k];
    }
    //Run sorts
    mergeSort(arrPurchMerge_ltoh, 0, 49, "purchases", true);
    quickSortLomuto(arrPurchLomuto_ltoh, 0, 49, "purchases", true);
    quickSortHoare(arrPurchHoare_ltoh, 0, 49, "purchases", true);

    //Save results
    saveToFile("purchases_ltoh.txt", arrPurchMerge_ltoh, arrPurchLomuto_ltoh, arrPurchHoare_ltoh, 50, "purchases");

    //Purchases: High to Low
    Item arrPurchMerge_htol[50];
    Item arrPurchLomuto_htol[50];
    Item arrPurchHoare_htol[50];
    
    for (int k = 0; k < 50; k++){
        arrPurchMerge_htol[k] = arrItems[k];
        arrPurchLomuto_htol[k] = arrItems[k];
        arrPurchHoare_htol[k] = arrItems[k];
    }
    //Run sorts
    mergeSort(arrPurchMerge_htol, 0, 49, "purchases", false);
    quickSortLomuto(arrPurchLomuto_htol, 0, 49, "purchases", false);
    quickSortHoare(arrPurchHoare_htol, 0, 49, "purchases", false);

    //Save results
    saveToFile("purchases_htol.txt", arrPurchMerge_htol, arrPurchLomuto_htol, arrPurchHoare_htol, 50, "purchases");

    //Runtimes
    ofstream runtimeFile("runtimes.txt");
    for (int i = 10; i <= 50; i++){
        //Initialize temp arrays
        Item arrMerge_temp[50];
        Item arrLomuto_temp[50];
        Item arrHoare_temp[50];
        
        //Give temp arrays the original data
        for (int k = 0; k < i; k++){
            arrMerge_temp[k] = arrItems[k];
            arrLomuto_temp[k] = arrItems[k];
            arrHoare_temp[k] = arrItems[k];
        }

        //Measure MerTimei
        auto startMerge = high_resolution_clock::now();
        mergeSort(arrMerge_temp, 0, i - 1, "price", true);
        auto endMerge = high_resolution_clock::now();

        auto MerTimei = duration_cast<nanoseconds>(endMerge - startMerge).count();

        //Measure QLomTimei
        auto startLomuto = high_resolution_clock::now();
        quickSortLomuto(arrLomuto_temp, 0, i - 1, "price", true);
        auto endLomuto = high_resolution_clock::now();

        auto QLomTimei = duration_cast<nanoseconds>(endLomuto - startLomuto).count();

        //Measure QHoaTimei
        auto startHoare = high_resolution_clock::now();
        quickSortHoare(arrHoare_temp, 0, i - 1, "price", true);
        auto endHoare = high_resolution_clock::now();

        auto QHoaTimei = duration_cast<nanoseconds>(endHoare - startHoare).count();

        //Write MerTime, QLomTime, and QHoaTime to the file
        runtimeFile << MerTimei << ", " << QLomTimei << ", " << QHoaTimei << endl;
    }
    runtimeFile.close();

    return 0;
}

void saveToFile(string fileName, Item mergeArray[], Item lomutoArray[], Item hoareArray[], int n, string feature) {
    ofstream outFile(fileName);

    //Print Merge Sort results
    for (int i = 0; i < n; i++){
        outFile << mergeArray[i].id << " ";
        
        if (feature == "price"){
            outFile << mergeArray[i].price;
        }
        if (feature == "purchases"){
            outFile << mergeArray[i].purchases;
        }
        outFile << endl;
    }
    outFile << endl;

    //Print Lomuto results
    for (int i = 0; i < n; i++){
        outFile << lomutoArray[i].id << " ";
        
        if (feature == "price"){
            outFile << lomutoArray[i].price;
        }
        if (feature == "purchases"){
            outFile << lomutoArray[i].purchases;
        }
        outFile << endl;
    }
    outFile << endl;

    //Print Hoare results
    for (int i = 0; i < n; i++){
        outFile << hoareArray[i].id << " ";
        
        if (feature == "price"){
            outFile << hoareArray[i].price;
        }
        if (feature == "purchases"){
            outFile << hoareArray[i].purchases;
        }
        outFile << endl;
    }

    outFile.close();
}

//Merge Sort Algorithm
void mergeSort(Item arrItems[], int startIndex, int endIndex, string feature, bool lowToHigh){
    //Base Case
    if (startIndex >= endIndex){
        return;
    }

    //Find the middle index
    int midIndex = startIndex + (endIndex - startIndex) / 2;
    
    //Recursively call mergesort
    mergeSort(arrItems, startIndex, midIndex, feature, lowToHigh);
    mergeSort(arrItems, midIndex + 1, endIndex, feature, lowToHigh);

    //Set the left and right sizes of the new arrays
    int leftSize = midIndex - startIndex + 1;
    int rightSize = endIndex - midIndex;

    //Dynamically allocate new arrays for each recursion
    Item* arrLeft = new Item[leftSize];
    Item* arrRight = new Item[rightSize];

    //Add Data to both the new arrays
    for (int i = 0; i < leftSize; i++){
        arrLeft[i] = arrItems[startIndex + i];
    }
    for (int j = 0; j < rightSize; j++){
        arrRight[j] = arrItems[midIndex + 1 + j];
    }

    //Pointers
    int leftPointer = 0;
    int rightPointer = 0;
    int mainArrayIndex = startIndex;
    
    while (leftPointer < leftSize && rightPointer < rightSize){
        bool isSmaller = false;

        //Sort according to feature and order
        if (feature == "price"){
            if (lowToHigh == true){
                if (arrLeft[leftPointer].price <= arrRight[rightPointer].price){
                    isSmaller = true;
                }
            }
            if (!lowToHigh){
                if (arrLeft[leftPointer].price >= arrRight[rightPointer].price){
                    isSmaller = true;
                }
            }
        }
        if (feature == "purchases"){
            if (lowToHigh == true){
                if (arrLeft[leftPointer].purchases <= arrRight[rightPointer].purchases){
                    isSmaller = true;
                }
            }
            if (!lowToHigh){
                if (arrLeft[leftPointer].purchases >= arrRight[rightPointer].purchases){
                    isSmaller = true;
                }
            }
        }

        //If the variable is smaller it moves the pointer accordingly and vice versa
        if (isSmaller == true){
            arrItems[mainArrayIndex] = arrLeft[leftPointer]; 
            leftPointer++;
        } else {
            arrItems[mainArrayIndex] = arrRight[rightPointer]; 
            rightPointer++;
        }
        mainArrayIndex++;
    }

    //Leftover items are put inside array
    while (leftPointer < leftSize){
        arrItems[mainArrayIndex] = arrLeft[leftPointer]; 
        leftPointer++; 
        mainArrayIndex++;
    }
    while (rightPointer < rightSize){
        arrItems[mainArrayIndex] = arrRight[rightPointer]; 
        rightPointer++; 
        mainArrayIndex++; 
    }

    //Delete dynamic arrays
    delete [] arrLeft;
    delete [] arrRight;
}

//Quick Sort algorithm using Lomuto
void quickSortLomuto(Item arrItems[], int startIndex, int endIndex, string feature, bool lowToHigh){
    //Base Case
    if (startIndex >= endIndex){
        return;
    }

    //Create the pivot item and key index pointer
    Item pivotItem = arrItems[endIndex];
    int keyIndex = startIndex - 1;

    //Makes the pointer that compares every item to the pivot and divides the array accordingly
    for (int searchIndex = startIndex; searchIndex < endIndex; searchIndex++){
        bool shouldSwap = false;

        //Feature and order
        if (feature == "price"){
            if (lowToHigh == true){
                if (arrItems[searchIndex].price <= pivotItem.price){
                    shouldSwap = true;
                }
            }
            if (!lowToHigh){
                if (arrItems[searchIndex].price >= pivotItem.price){
                    shouldSwap = true;
                }
            }
        }
        if (feature == "purchases"){
            if (lowToHigh == true){
                if (arrItems[searchIndex].purchases <= pivotItem.purchases){
                    shouldSwap = true;
                }
            }
            if (!lowToHigh){
                if (arrItems[searchIndex].purchases >= pivotItem.purchases){
                    shouldSwap = true;
                }
            }
        }

        //Swap if needed
        if (shouldSwap == true){
            keyIndex++;
            Item temp = arrItems[keyIndex]; 
            arrItems[keyIndex] = arrItems[searchIndex]; 
            arrItems[searchIndex] = temp;
        }
    }

    //Put the pivot in its place
    Item temp = arrItems[keyIndex + 1]; 
    arrItems[keyIndex + 1] = arrItems[endIndex]; 
    arrItems[endIndex] = temp;
    int partitionIndex = keyIndex + 1;

    //Recursive calls
    quickSortLomuto(arrItems, startIndex, partitionIndex - 1, feature, lowToHigh);
    quickSortLomuto(arrItems, partitionIndex + 1, endIndex, feature, lowToHigh);
}

//Quick Sort algorithm using Hoare
void quickSortHoare(Item arrItems[], int startIndex, int endIndex, string feature, bool lowToHigh){
    //Base Case: Stop if pointers meet or cross
    if (startIndex >= endIndex){
        return;
    }

    //Create the pivot item and left and right pointers
    Item pivotItem = arrItems[startIndex];
    int leftPointer = startIndex;
    int rightPointer = endIndex;

    while (leftPointer <= rightPointer){
        //Check feature and order
        if (feature == "price"){
            if (lowToHigh == true){
                while (arrItems[leftPointer].price < pivotItem.price){
                    leftPointer++;
                }
                while (arrItems[rightPointer].price > pivotItem.price){
                    rightPointer--;
                }
            }
            if (!lowToHigh){
                while (arrItems[leftPointer].price > pivotItem.price){
                    leftPointer++;
                }
                while (arrItems[rightPointer].price < pivotItem.price){
                    rightPointer--;
                }
            }
        }
        if (feature == "purchases"){
            if (lowToHigh == true){
                while (arrItems[leftPointer].purchases < pivotItem.purchases){
                    leftPointer++;
                }
                while (arrItems[rightPointer].purchases > pivotItem.purchases){
                    rightPointer--;
                }
            }
            if (!lowToHigh){
                while (arrItems[leftPointer].purchases > pivotItem.purchases){
                    leftPointer++;
                }
                while (arrItems[rightPointer].purchases < pivotItem.purchases){
                    rightPointer--;
                }
            }
        }

        //Check if pointers have crossed
        if (leftPointer <= rightPointer) {
            //Swap left and right items that are being pointed at
            Item temp = arrItems[leftPointer]; 
            arrItems[leftPointer] = arrItems[rightPointer]; 
            arrItems[rightPointer] = temp;
            
            //increment
            leftPointer++;
            rightPointer--;
        }
    }

    //Recursive calls
    if (startIndex < rightPointer) {
        quickSortHoare(arrItems, startIndex, rightPointer, feature, lowToHigh);
    }
    if (leftPointer < endIndex) {
        quickSortHoare(arrItems, leftPointer, endIndex, feature, lowToHigh);
    }
}