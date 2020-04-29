#include "Allocator.h"
#include <iostream>
using namespace std;

//����� ��� ����������
int main()
{
    Allocator allo;
    allo.makeAllocator(10);

    //���������� �� ������
    char* a=allo.alloc(4);
    for (int i=0; i < 4; ++i) {
        *(a + i)='a';
    }
    if (a != nullptr){
    	cout << "1st test: passed" << endl;
    }
    else cout << "1st test: not passed" << endl;
    
    //������������ �� ������
    char* b=allo.alloc(4);
    for (int i=0; i < 4; ++i) {
        *(b + i)='a';
    }
    if (b != nullptr){
    	cout << "2nd test: passed" << endl;
    }
    else cout << "2nd test: not passed" << endl;
    
    //�� �����, ����� ������ ���������
    char* c=allo.alloc(4);
    if (c == nullptr){
    	cout << "3rd test: passed" << endl;
    }
    else cout << "3rd test: not passed" << endl;

    allo.reset();

    //����� ����� ����� reset'�
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


