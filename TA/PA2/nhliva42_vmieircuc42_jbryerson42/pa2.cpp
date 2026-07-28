/*
Name: Jackson Ryerson, Vadin Mieir-Cucksey, Nolan Hliva
Date: 3/29/2026
Reason: To sort items with prices, ratings, and amount bought
*/
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
float mertime[41] = {};
float lomutotime[41] = {};
float hoaretime[41] = {};
int count = 0;

class ItemFeature {
    public:
        int itemNum;
        float price;
        float rating;
        int purchases;
       

        void setItem(int itemNum){
            this->itemNum = itemNum;
        }

        void setPrice(float price){
            this->price = price;
        }

        void setRating(float rating){
            this->rating = rating;
        }

        void setPurchases(int purchases){
            this->purchases = purchases;
        }

        int getItemNum(){
            return itemNum;
        }

        float getPrice(){
            return price;
        }

        float getRating(){
            return rating;
        }

        int getPurchases(){
            return purchases;
        }

};

void merge(ItemFeature arr[], int left, int mid, int right, bool order, bool priceOrPurchase){
    //create temp arrays
    int n1 = mid - left + 1;
    int n2 = right - mid;

    ItemFeature* L = new ItemFeature[n1];
    ItemFeature* R = new ItemFeature[n2];

    for (int i = 0; i < n1; i++){
        L[i] = arr[left + i];   
    }
    for (int j = 0; j < n2; j++){
        R[j] = arr[mid + 1 + j];
    }

    //merge the temp arrays back into arr
    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2){
        if(priceOrPurchase){
            if(order){
                if(L[i].getPrice() <= R[j].getPrice()){
                    arr[k] = L[i];
                    i++;
                }
                else{
                    arr[k] = R[j];
                    j++;
                }
            }
            else{
                if(L[i].getPrice() >= R[j].getPrice()){
                    arr[k] = L[i];
                    i++;
                }
                else{
                    arr[k] = R[j];
                    j++;
                }
            }
        }
        else{
            if(order){
                if(L[i].getPurchases() <= R[j].getPurchases()){
                    arr[k] = L[i];
                    i++;
                }
                else{
                    arr[k] = R[j];
                    j++;
                }
            }
            else{
                if(L[i].getPurchases() >= R[j].getPurchases()){
                    arr[k] = L[i];
                    i++;
                }
                else{
                    arr[k] = R[j];
                    j++;
                }
            }
        }
        k++;
    }

    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void merge_sort(ItemFeature arr[], int left, int right, bool order, bool priceOrPurchase){
   
if(left >= right){
    return;
}

int mid =   left + (right - left) / 2;

merge_sort(arr, left, mid, order, priceOrPurchase);
merge_sort(arr, mid + 1, right, order, priceOrPurchase);
merge(arr, left, mid, right, order, priceOrPurchase);



}

int Partition(ItemFeature arr[], int low, int high, bool order, bool priceOrPurchase){
    ItemFeature pivot = arr[high];
    int i = low - 1;

    for( int j = low; j <= high -1; j++){
        bool condition = false;

        if (priceOrPurchase) {
            // Sorting by price
            if (order) {
                condition = arr[j].getPrice() <= pivot.getPrice(); // low-to-high
            } else {
                condition = arr[j].getPrice() >= pivot.getPrice(); // high-to-low
            }
        } else {
            // Sorting by purchases
            if (order) {
                condition = arr[j].getPurchases() <= pivot.getPurchases(); // low-to-high
            } else {
                condition = arr[j].getPurchases() >= pivot.getPurchases(); // high-to-low
            }
        }

        if (condition) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
  
    swap(arr[i+1], arr[high]);
    return i + 1;
}
  

void QS_Lomuto_Partition(ItemFeature arr[], int low, int high, bool order , bool priceOrPurchase){
    if(low < high){
        int p = Partition(arr, low, high, order, priceOrPurchase);
        QS_Lomuto_Partition(arr, low, p - 1, order, priceOrPurchase);
        QS_Lomuto_Partition(arr, p + 1, high, order, priceOrPurchase);
    }

}


void Hoare_Partition(ItemFeature arr[], int low, int high, bool order, bool priceOrPurchase){
    static int count = 0; // static to keep track across recursion
 
    if(low < high){
        ItemFeature pivot = arr[low];
 
        int i = low - 1;
        int j = high + 1;
 
        while(true){
            i++;
            float valueI, valuePivot;
            if(priceOrPurchase){
                valueI = arr[i].getPrice();
                valuePivot = pivot.getPrice();
            }else{
                valueI = arr[i].getPurchases();
                valuePivot = pivot.getPurchases();
            }
 
            if(order){
                // LOW TO HIGHHHHH
                while(valueI < valuePivot){
                    i++;
                    if(priceOrPurchase){
                        valueI = arr[i].getPrice();
                    }else{
                        valueI = arr[i].getPurchases();
                    }  
                }
            }else{
                while(valueI > valuePivot){
                    i++;
                    if(priceOrPurchase){
                        valueI = arr[i].getPrice();
                    }else{
                        valueI = arr[i].getPurchases();
                    }  
                }
            }
 
            //j time
            j--;
            float valueJ;
            if(priceOrPurchase){
                valueJ = arr[j].getPrice();
            }else{
                valueJ = arr[j].getPurchases();
            }
 
            if(order){
                // low to high again
                while(valueJ > valuePivot){
                    j--;
                    if(priceOrPurchase){
                        valueJ = arr[j].getPrice();
                    }else{
                        valueJ = arr[j].getPurchases();
                    }  
                }
            }else{
                while(valueJ < valuePivot){
                    j--;
                    if(priceOrPurchase){
                        valueJ = arr[j].getPrice();
                    }else{
                        valueJ = arr[j].getPurchases();
                    }  
                }
            }
 
            if(i >= j)
                break;
 
            // Swap elements at i and j
            ItemFeature temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
 
        int p = j;
 
        Hoare_Partition(arr, low, p, order, priceOrPurchase);
        Hoare_Partition(arr, p + 1, high, order, priceOrPurchase);
    }
}

int main(){
    ifstream myfile("items.txt");
    ofstream out("runtimes.txt");
    ofstream out2("price_htol.txt");
    ofstream out3("purchases_ltoh.txt");
    ofstream out4("purchases_htol.txt");
    ofstream out5("price_ltoh.txt");
    //original array
    ItemFeature item[50];

    for(int i=0; i<50; i++){
        int itemNum1;
        float price1;
        float rating1;
        int purchases1;

        myfile >> itemNum1 >> price1 >> rating1 >> purchases1;

        item[i].setItem(itemNum1);
        item[i].setPrice(price1);
        item[i].setRating(rating1);
        item[i].setPurchases(purchases1);
    }

    for (int i = 10; i <= 50; i++) {
        ItemFeature temp[50];
        for (int j = 0; j < i; j++){
            temp[j] = item[j];
        }
        auto start = chrono::high_resolution_clock::now();
        merge_sort(temp, 0, i-1, true, true); 
        auto end = chrono::high_resolution_clock::now();

        mertime[i - 10] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }
    
    for (int i = 10; i <= 50; i++) {
        ItemFeature temp[50];
        for (int j = 0; j < i; j++){
            temp[j] = item[j];
        }
        auto start = chrono::high_resolution_clock::now();
        Hoare_Partition(temp, 0, i-1, true, true); 
        auto end = chrono::high_resolution_clock::now();

        hoaretime[i - 10] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
}
    
    for (int i = 10; i <= 50; i++) {
        ItemFeature temp[50];
        for (int j = 0; j < i; j++){
            temp[j] = item[j];
        }
        auto start = chrono::high_resolution_clock::now();
        QS_Lomuto_Partition(temp, 0, i-1, true, true); 
        auto end = chrono::high_resolution_clock::now();

        lomutotime[i - 10] = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // copy the unsorted array for later use
    ItemFeature mergecopy1[50];
    for(int x=0; x < 50; x++){
        mergecopy1[x] = item[x];
    }

    ItemFeature mergecopy2[50];
    for(int x=0; x < 50; x++){
        mergecopy2[x] = item[x];
    }

    ItemFeature mergecopy3[50];
    for(int x=0; x < 50; x++){
        mergecopy3[x] = item[x];
    }

    ItemFeature mergecopy4[50];
    for(int x=0; x < 50; x++){
        mergecopy4[x] = item[x];
    }

    ItemFeature lomutocopy1[50];
    for(int x=0; x < 50; x++){
        lomutocopy1[x] = item[x];
    }
    ItemFeature lomutocopy2[50];
    for(int x=0; x < 50; x++){
        lomutocopy2[x] = item[x];
    }
    ItemFeature lomutocopy3[50];
    for(int x=0; x < 50; x++){
        lomutocopy3[x] = item[x];
    }
    ItemFeature lomutocopy4[50];
    for(int x=0; x < 50; x++){
        lomutocopy4[x] = item[x];
    }

        ItemFeature hoarecopy1[50];
    for(int x=0; x < 50; x++){
        hoarecopy1[x] = item[x];
    }
    ItemFeature hoarecopy2[50];
    for(int x=0; x < 50; x++){
        hoarecopy2[x] = item[x];
    }
    ItemFeature hoarecopy3[50];
    for(int x=0; x < 50; x++){
        hoarecopy3[x] = item[x];
    }
    ItemFeature hoarecopy4[50];
    for(int x=0; x < 50; x++){
        hoarecopy4[x] = item[x];
    }
   
    merge_sort(mergecopy2, 0, 49, false, true);// high to low, price
    merge_sort(mergecopy3, 0, 49, true, false);// low to high, rating
    merge_sort(mergecopy4, 0, 49, false, false);// high to low, rating
    merge_sort(mergecopy1, 0, 49, true, true);//low to high, price, needs to go last for time
   
    QS_Lomuto_Partition(lomutocopy2, 0, 49, false, true);// high to low, price
    QS_Lomuto_Partition(lomutocopy3, 0, 49, true, false);// low to high, rating
    QS_Lomuto_Partition(lomutocopy4, 0, 49, false, false);// high to low, rating
    QS_Lomuto_Partition(lomutocopy1, 0, 49, true, true);// low to high, price
    Hoare_Partition(hoarecopy2, 0, 49, false, true);// high to low, price
    Hoare_Partition(hoarecopy3, 0, 49, true, false);// low to high, rating
    Hoare_Partition(hoarecopy4, 0, 49, false, false);// high to low, rating
    Hoare_Partition(hoarecopy1, 0, 49, true, true);// low to high, price
    for(int i = 0; i < 41; i++){
    out << "(" 
        << mertime[i] << ", "
        << lomutotime[i] << ", "
        << hoaretime[i] 
        << ")" << endl;
}
    //to see array on screen, need to output each to file
    //for (int i = 0; i < 50; i++) {
    //    std::cout << "Item number: " << bubblecopy1[i].itemNum << ". Price: " << bubblecopy1[i].price << endl;
    //}
    //std::cout << std::endl;
    // COMMENT OUT OR DELETE LATER


    // this section prints stuff to files
    out2 << "merge sort high-low price" << endl;
    for (int i = 0; i < 50; i++){
        out2 << "purchases: " << mergecopy2[i].purchases << ". Price: " << mergecopy2[i].price << endl;
    }
    out2 << "lomuto sort high-low price" << endl;
    for (int i = 0; i < 50; i++){
        out2 << "purchases: " << lomutocopy2[i].purchases << ". Price: " << lomutocopy2[i].price << endl;
    }
    out2 << "hoare sort high-low price" << endl;
    for (int i = 0; i < 50; i++){
        out2 << "purchases: " << hoarecopy2[i].purchases << ". Price: " << hoarecopy2[i].price << endl;
    }


    out5 << "merge sort low-high price" << endl;
    for (int i = 0; i < 50; i++){
        out5 << "purchases: " << mergecopy1[i].purchases << ". Price: " << mergecopy1[i].price << endl;
    }
    out5 << "lomuto sort low-high price" << endl;
    for (int i = 0; i < 50; i++){
        out5 << "purchases: " << lomutocopy1[i].purchases << ". Price: " << lomutocopy1[i].price << endl;
    }
    out5 << "hoare sort low-high price" << endl;
    for (int i = 0; i < 50; i++){
        out5 << "purchases: " << hoarecopy1[i].purchases << ". Price: " << hoarecopy1[i].price << endl;
    }



        out4 << "merge sort high-low purchases" << endl;
    for (int i = 0; i < 50; i++){
        out4 << "purchases: " << mergecopy4[i].purchases << ". Price: " << mergecopy4[i].price << endl;
    }
    out4 << "lomuto sort high-low purchases" << endl;
    for (int i = 0; i < 50; i++){
        out4 << "purchases: " << lomutocopy4[i].purchases << ". Price: " << lomutocopy4[i].price << endl;
    }
    out4 << "hoare sort high-low purchases" << endl;
    for (int i = 0; i < 50; i++){
        out4 << "purchases: " << hoarecopy4[i].purchases << ". Price: " << hoarecopy4[i].price << endl;
    }


        out3 << "merge sort low-high ratingurcchases" << endl;
    for (int i = 0; i < 50; i++){
        out3 << "purchases: " << mergecopy3[i].purchases << ". Price: " << mergecopy3[i].price << endl;
    }
    out3 << "lomuto sort low-high purchases" << endl;
    for (int i = 0; i < 50; i++){
        out3 << "purchases: " << lomutocopy3[i].purchases << ". Price: " << lomutocopy3[i].price << endl;
    }
    out3 << "hoare sort low-high purchases" << endl;
    for (int i = 0; i < 50; i++){
        out3 << "purchases: " << hoarecopy3[i].purchases << ". Price: " << hoarecopy3[i].price << endl;
    }


    // close all files
    out.close();
    out2.close();
    out3.close();
    out4.close();
    out5.close();
    myfile.close();
}// CLICK ME!!! https://youtu.be/dQw4w9WgXcQ?si=zug-zGv_53SXCqU-