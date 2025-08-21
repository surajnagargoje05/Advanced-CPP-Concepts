/*
    Templates in C++:
    -----------------
    - Templates allow us to write generic code that works with any data type 
      (int, float, double, string, user-defined class, etc.) without rewriting 
      the same function multiple times.
    - Instead of creating separate functions for int, float, etc., 
      we create one function template and the compiler generates the 
      appropriate function at compile time (called "template instantiation").

    Why use Templates?
    ------------------
    - Code reusability (write once, use for many types).
    - Type safety (compiler checks type at compile-time).
    - Cleaner and shorter code compared to function overloading.

    How it is used here:
    --------------------
    - We wrote a single function template `Max<T>(T a, T b)` which works 
      for ANY data type that supports `>` operator.
    - When we call `Max(10, 20)`, the compiler instantiates `Max<int>`.
    - When we call `Max(19.1f, 11.2f)`, the compiler instantiates `Max<float>`.
    - So, same template function works for different data types without duplication.
*/

#include <iostream>
using namespace std;

// int max(int a, int b){
//     if(a > b){
//         return a;
//     }
//     else {
//         return b;
//     }
// }

// float max(float a, float b){
//     if(a > b){
//         return a;
//     }
//     else {
//         return b;
//     }
// }

template<typename T>
T Max(T a, T b){
        if(a > b){
        return a;
    }
    else {
        return b;
    }
}
int main(){

    auto num = Max(10, 20);
    cout << num << endl;

    auto num1 = Max(19.1f, 11.2f);
    cout << num1 << endl;
    return 0;
}