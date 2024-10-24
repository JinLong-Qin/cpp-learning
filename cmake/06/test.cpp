#include <iostream>
using namespace std;

#define NUMBER 3
// #define DEBUG 1


int main(){
    int a = 10;
    #ifdef DEBUG
        cout << "测试" << endl;
    #endif
    for(int i=0; i<NUMBER; ++i)
    {
        printf("hello, GCC!!!\n");
    }
    return 0;
}