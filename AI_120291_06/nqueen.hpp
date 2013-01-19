#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#define abs(x) (x>0) ? x : -x

using namespace std;

#include "backtrack.hpp"

#define MAXSIZE 15     // maximum size
#define AVAILABLE true 

// Board class to reprent the chess board with n queens on it
class NQueen : public Backtrack{
private:
    int data[MAXSIZE]; // board data
    int size; // board size
    bool column[MAXSIZE]; // column available state
    bool leftDiagonal[2*MAXSIZE-1]; // left diagonal available state
    bool rightDiagonal[2*MAXSIZE-1]; // right diagonal available state
public:
    NQueen(int size);  // Contructor
    virtual bool IsValid(int row, int col);  // Check if put a queen in (row, col) is valid
    virtual void Update(int row, int col);   // Place queen to (row, col) and update state's variables
    virtual void Restore(int row, int col);  // Place queen to (row, col) and update state's variables
    virtual void PrintSolution();            // print the symetric board
};
