#include <iostream>
using namespace std;

// int sum(int a, int b){
//     return a + b;
// }

// float sum(float a, float b){
//     return a + b;
// }


int main(){

    int a = 20;
    int b = 30;

    float c = 30.22;
    float d = 33.33;

    // function_name = [](inpute)->return_type{ function body }

    auto display = [](){
        cout <<"Hello from Lambda function " << endl;
    };

    // auto sum = [](int a, int b) -> int{
    //     return a + b;
    // };
    // Generic Lambda introduced in C++14
    auto sum = [](auto a, auto b){
        return a + b;
    };

    display();

    cout << sum(a, b) << endl;
    cout << sum(c, d) << endl;
    display();

    return 0;
}