#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <map>
#include <cstdio>

using namespace std;

#include "cryptarithmetic.hpp"    

// Initialization
void CRYPTARITHMETIC::Initialize(int argc, char*argv[]){
    DOMAIN_SIZE = 10;
    numSolution=0;
    length = strlen(argv[3]);
    for(int i =0; i< 10; i++){
	available[i] = true;
	assigned[i] = false;
    }

    for(int i =0; i<3; i++){
	string tmp(argv[i+1]);
	if(strlen(argv[i+1])<length)
	    str[i] = EMPTY+ tmp;
	else
	    str[i] = tmp;
    }
    var+=EMPTY;

    for(int i =0; i<length; i++){
	for(int j=0; j<3; j++){
	    char c = str[j][i];
	    if(var.find(c)==string::npos){
		if(isdigit(c)){
		    mapChar[c] = c-'0';
		    assigned[c-'0']=true;
		}
		else
		    var+=c;
	    }
	}
    }

    NUM_VAR = var.length();
    for(int i=0; i<NUM_VAR; i++){
	mapChar[var[i]] = UNKNOWN;
    }
    mapChar[EMPTY] = 0;

}

// Check if the puzzle is solvable or not
bool CRYPTARITHMETIC::IsSolvable(){
    // Too many arguments
    int cnt  =0;
    for(int i = 0 ; i<NUM_VAR; i++){
	if(isalpha(var[i]))
	    cnt++;
    }
    if(cnt>10)
	return false;
    return true;
}

// Print the solution
void CRYPTARITHMETIC::PrintSolution(){
    cout << "#"<< numSolution <<":"<<endl;
    for(int i=1; i<NUM_VAR; i++)
	cout << var[i]<<" --> "<<mapChar[var[i]]<<endl;
    for(int i=0; i <3; i++){
	for(int j=0; j<length; j++){
	    char c = str[i][j];
	    if(c!=EMPTY){
		cout << mapChar[c];
	    }
	}
	if(i==0)
	    cout << " + ";
	if(i==1)
	    cout << " = ";
    }
    cout <<endl;
}


//static int cnt1 = 0;
//static int cnt2 = 0;

// Unassign the value to var[index] and make value available for assigment
void CRYPTARITHMETIC::Restore(int index, int value){
    //  cout <<"-"<<++cnt1<<": " << var[index]<<"--->>\t" << value <<endl;
    mapChar[var[index]]= UNKNOWN;
    if(var[index]!='*')
	available[value] = true;
}

// Assign the value to var[index] and make value unavailable or assigment
void CRYPTARITHMETIC::Update(int index, int value){
    //  cout <<"+"<<++cnt2<<": " << var[index]<<"\t<<---" << value <<endl;
    mapChar[var[index]]= value;
    if(var[index]!='*')
	available[value] = false;
}

// Check if assigment of value to var[index] is possible or not
bool CRYPTARITHMETIC::IsValid(int index, int value){
    // First characters cannot be Zero
    if(value==0){
	for(int i=0; i<3; i++){
	    if(var[index]==str[i][0])
		return false;
	}
    }
    // Assign carry to UNKNOWN
    for(int i =0; i<=length; i++){
	carry[i] = UNKNOWN;
    }
    // If value is already used
    if(!available[value] || assigned[value])
	return false;
    // Temporary assignment
    mapChar[var[index]]= value;
    for(int i =0; i<length; i++){
	// If #i constraint is violated
	if(!IsValidForm(i)){
	    mapChar[var[index]]= UNKNOWN;
	    return false;
	}
    }
    // Undo assignment
    mapChar[var[index]]= UNKNOWN;
    return true;
}

// Check if #index constrain is valid or not
// For example, $send + $more = money ($ is empty)
// Has 5 constraints:
// 1. d+e = y+c1
// 2. n+e+c1 = e+10*c2
// 3. e+o+c2 = n+10*c3
// 4. s+m+c3 = o+10*c4
// 5.     c4 = m
// This function will check one of those constrains
// 
bool CRYPTARITHMETIC::IsValidForm(int index){
    int tmp[3];
    // Return true if there is an unassigned character
    for(int i =0; i<3; i++){
	tmp[i] = mapChar[str[i][index]];
	if(tmp[i]==UNKNOWN)
	    return true;
    }
    // Check 4 possible possibilities
    if (tmp[0]+tmp[1] == tmp[2]){
	if(carry[index]==1)
	    return false;
	carry[index] = carry[index+1] = 0;
	return true;
    }
    if(tmp[0]+tmp[1]+1 == tmp[2]){
	if(index==length-1)
	    return false;
	if(carry[index]== 1)
	    return false;
	carry[index] = 0;
	carry[index+1] = 1;
	return true;
    }
    if(tmp[0]+tmp[1] == tmp[2]+10){
	if(index==0)
	    return false;
	if(carry[index]==0)
	    return false;
	carry[index]=1;
	carry[index+1] = 0;
	return true;
    }
    if(tmp[0]+tmp[1]+1 == tmp[2]+10){
	if(index == 0)
	    return false;
	if(index==length-1)
	    return false;
	if(carry[index]==0)
	    return false;
	carry[index]=1;
	carry[index+1] = 1;
	return true;
    }
    return false;
}



/****************/
int main(int argc, char*argv[]){
    /*Initialization*/
    CRYPTARITHMETIC crypt;
    crypt.Initialize(argc, argv);
    /*Check solvability*/
    if(!crypt.IsSolvable()){
	cout << "Not solvable!" <<endl;
	return -1;
    }
    /*Backtracking*/
    clock_t start = clock();
    crypt.Solve(1);
    cout << "Number of solutions " <<crypt.NumSolution() << endl;
    cout << "Number of assigments:" << crypt.NumAssigment() <<endl;
    clock_t end = clock();
    cout <<"Execution's time: " <<((double)end-(double)start)/(double)(CLOCKS_PER_SEC)<<" (s)"<<endl;
    return 0;
}
