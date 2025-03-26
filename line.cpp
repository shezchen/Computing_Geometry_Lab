//
// Created by aoyeningluosi on 24-11-5.
//
#include <iostream>
#include "line.h"
int main() {
    point a(2.0,2.0),b(3.2,10);
    if(a*b>0)
        std::cout<<"a<b"<<std::endl;
    else if(a*b==0)
        std::cout<<"a with b"<<std::endl;
    else
        std::cout<<"a>b"<<std::endl;
    return 0;
}