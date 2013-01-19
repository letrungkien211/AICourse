#ifndef BACKTRACK_HPP
#define BACKTRACK_HPP

class Backtrack{
protected:
    int DOMAIN_SIZE; // Size of the domain
    int NUM_VAR;     // Number of variables
    int numSolution; // number of solution
    int numAssignment; // number of assigments
    /***** Interface for implementation*****/
    // Check if an assignment is valid
    virtual bool IsValid(int index, int value) = 0;
    // Make an assignment and update state
    virtual void Update(int index, int value) = 0;
    // Undo the assignment and reset state
    virtual void Restore(int index, int value) = 0;
    // Print solutions
    virtual void PrintSolution() = 0;
public:
    virtual ~Backtrack(){}
    Backtrack():DOMAIN_SIZE(0), NUM_VAR(0), numSolution(0), numAssignment(0){}
    // Sovle by backtracking algorithm
    void Solve(int index){
	// A solution is reached when all variables are assigned
	if(index>=NUM_VAR){
	    numSolution++;
	    PrintSolution();  // Print the solution
	    return;
	}
	// Loop all over domain of var[k]                   	
	for(int value = 0; value < DOMAIN_SIZE; value++){
	    // If an assigment is possible                  
	    if(IsValid(index,value)){
		numAssignment++;
		// Make an assignment and update state
		Update(index,value);  
		// Continue with next variable's assignment
		Solve(index+1);       
		// Undo the assignment and reset state
		Restore(index,value); 
	    }
	}
    }
    int NumSolution() const{
	return numSolution;
    }
    int NumAssigment() const{
	return numAssignment;
    }
};


#endif
