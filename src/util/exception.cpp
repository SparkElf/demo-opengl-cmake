#pragma once
#include <string>
#include <iostream>
using namespace std;
class Exception{
    string info; 
public:
    Exception(string info):info(info){}
    Exception(string info,bool doPrint){
        this->info=info;
        if(doPrint==true)
            cout<<info<<endl;
    }
};