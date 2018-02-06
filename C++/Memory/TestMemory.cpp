//**************************************************************
//         Test routine for data structures
//**************************************************************

#include <iostream>

using namespace std;

struct MyStruct
{
    char a;         // 1 byte, padding 3
    int b;          // 4 bytes
    short c;        // 2 bytes, padding 6
    long long d;    // 8 bytes
    char e;         // 1 byte, padding 7
};

void TestAlignment()
{
    cout << "Size of MyStruct is: " << sizeof(MyStruct) << endl;
    cout << "Alignment of MyStruct is: " << alignment_of<MyStruct>::value << endl;

    MyStruct ms;
    ms.a = 'a';
    ms.b = 1;
    ms.c = 2;
    ms.d = 3;
    ms.e = 'b';
    cout << "Address of MyStruct a: " << hex << (long)&(ms.a) << endl;
    cout << "Address of MyStruct b: " << hex << (long)&(ms.b) << endl;
    cout << "Address of MyStruct c: " << hex << (long)&(ms.c) << endl;
    cout << "Address of MyStruct d: " << hex << (long)&(ms.d) << endl;
    cout << "Address of MyStruct e: " << hex << (long)&(ms.e) << endl;
}

int main()
{
    TestAlignment();
}

