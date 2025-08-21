/*
Convert the following functions into templates
int Add(int x,int y) ; //Return the sum of two elements
int ArraySum(const int *pArr, size_t arrSize); //Returns the sum of array elements
int Max(const int *pArr, size_t arrSize) ; //Return the largest element in the array
std::pair<int,int> MinMax(const int *pArr, size_t arrSize) ; //Return the smallest and largest element in a pair
Take care to avoid copies of user-defined objects wherever applicable.
*/

#include <iostream>
using namespace std;

template<typename T>
T Add(T a, T b){
    return a + b;
}

template<typename T>
T ArraySum(const T *pArr, T arrSize){
    T sum{};
    for(int i = 0; i < arrSize ; i++){
        sum = sum + pArr[i];
    }
    return sum;
}

template<typename T>
T Max(const T *pArr, T arrSize){
    T maxNum = pArr[0];
    for(int i = 0 ; i < arrSize ; i++){
        if(pArr[i] > maxNum){
            maxNum = pArr[i];
        }
    }
    return maxNum;
}

//template<typename T>

int main(){

    int arr[] = {10, 20, 5, 40, 25};

    std::cout << "Add: " << Add(5, 7) << "\n";
    std::cout << "ArraySum: " << ArraySum(arr, 5) << "\n";
    std::cout << "Max: " << Max(arr, 5) << "\n";
    return 0;
}