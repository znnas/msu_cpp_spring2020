#include "Allocator.h"
#include <iostream>
using namespace std;

//тесты для аллокатора
int main()
{
    Allocator allo;
    allo.makeAllocator(10);

    //выделяется ли память
    char* a=allo.alloc(4);
    for (int i=0; i < 4; ++i) {
        *(a + i)='a';
    }
    if (a != nullptr){
    	cout << "1st test: passed" << endl;
    }
    else cout << "1st test: not passed" << endl;
    
    //дописывается ли дальше
    char* b=allo.alloc(4);
    for (int i=0; i < 4; ++i) {
        *(b + i)='a';
    }
    if (b != nullptr){
    	cout << "2nd test: passed" << endl;
    }
    else cout << "2nd test: not passed" << endl;
    
    //не пишет, когда память кончилась
    char* c=allo.alloc(4);
    if (c == nullptr){
    	cout << "3rd test: passed" << endl;
    }
    else cout << "3rd test: not passed" << endl;

    allo.reset();

    //снова пишет после reset'а
    char* d=allo.alloc(4);
    for (int i=0; i < 4; ++i) {
        *(d + i)='a';
    }
    
    if (d != nullptr){
    cout << "4th test: passed" << endl;
    }
    else cout << "4th test: not passed" << endl;

    allo.~Allocator();
}


