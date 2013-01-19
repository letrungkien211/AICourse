#ifndef CRYPTARITHMETIC_HPP
#define CRYPTARITHMETIC_HPP
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <map>
#include <cstdio>

using namespace std;

#include "backtrack.hpp"

#define EMPTY '$'
#define UNKNOWN 100

class CRYPTARITHMETIC:public Backtrack{
private:
    map<char,int> mapChar;  // map characters to values
    string str[3]; 	    // 3 strings
    bool available[10];     // available[i]=true means i is available for assignment
    bool assigned[10];      // E.g (7a+bb=cc) assigned[7]=true
    string var;             // string of unique variables
    int length;             // length of longest string, also number of constraints to check
    int carry[10];          // Carry bit
public:
//inherited from Backtrack
    virtual void Update(int index, int value);  // Assign and refresh state
    virtual void Restore(int index, int value);  // Unassign and refresh state
    virtual bool IsValid(int index, int value);  // Check if var[index]=value is valid 
    virtual void PrintSolution();  // Print solution

    void Initialize(int argc, char *argv[]);  // Initialize
    bool IsSolvable();
    bool IsValidForm(int index);  
};    

#endif
