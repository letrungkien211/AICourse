#include "nqueen.hpp"

NQueen::NQueen(int size):Backtrack(){
    this->size = size;
    DOMAIN_SIZE = size;
    NUM_VAR = size;
    if(size>MAXSIZE){
	cout << "Maximum size allowed is: " << MAXSIZE <<endl;
	exit(-4);
    }
    for(int i = 0; i<size; i++)
	data[i] = -1;
    for(int i = 0; i<size; i++)
	column[i] = true;
    for(int i = 0; i<2*size-1; i++)
	leftDiagonal[i] = rightDiagonal[i] = AVAILABLE;
}
// Check assignment validibility
bool NQueen::IsValid(int row, int col){
    if(row ==0 && col>=(size+1)/2)
	return false;
    return column[col] == AVAILABLE && leftDiagonal[row+col] == AVAILABLE && rightDiagonal[row-col+size-1] == AVAILABLE;
}
// Assign and update consistency
void NQueen::Update(int row, int col){
    data[row] = col;
    column[col] =leftDiagonal[row+col] = rightDiagonal[row-col+size-1] = !AVAILABLE;
}
// Unassign and restore consistency
void NQueen::Restore(int row, int col){
    data[row] = -1;
    column[col] =leftDiagonal[row+col] = rightDiagonal[row-col+size-1] = AVAILABLE;
}

void NQueen::PrintSolution(){
    // Print current solution
    cout << "#"<< numSolution <<":"<<endl;
    for(int i = 0; i<size; ++i){
	for(int j = 0; j<size; ++j){
	    if(data[i]==j)
		cout << "Q ";
	    else
		cout << "* ";
	}
	cout<<endl;
    }

    // Print the symetric version of current solution
    // This would reduce half of computing time
    if(size%2 && data[0]==(size/2))
	return;       
    cout << "#"<< ++numSolution <<":"<<endl;
    for(int i = 0; i<size; ++i){
	for(int j = 0; j<size; ++j){
	    if(data[i]==size-1-j)
		cout << "Q ";
	    else
		cout << "* ";
	}
	cout<<endl;
    }

}

int main(int argc, char *argv[]){
    if(argc<2){
	cout << "./nqueen 10" <<endl;
	exit(-1);
    }
    int size = atoi(argv[1]);
    NQueen nqueen(size);
    clock_t start = clock();
    nqueen.Solve(0);
    cout << "Number of solutions: " <<nqueen.NumSolution() << endl;
    cout << "Number of assigments:" << nqueen.NumAssigment() <<endl;
    clock_t end = clock();
    cout <<"Execution's time: " <<((double)end-(double)start)/(double)(CLOCKS_PER_SEC)<<" (s)"<<endl;
    return 0;
}
