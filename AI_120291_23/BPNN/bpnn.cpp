// System headers
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "cv.h"

// My headers
#include "bpnn.hpp"
#include "utils.hpp"

// Namespaces
using namespace std;
using namespace Eigen;
using namespace cv;
using namespace Utils;


// Default constructor 
BPNN::BPNN(){
}

// Constructor with layers' size infomation
BPNN::BPNN(const vector<int> &layerSize){
    Initialize(layerSize);
}


BPNN::BPNN(int inputLayerSize, int hiddenLayerSize, int outputLayerSize){
    vector<int> layerSize(3);
    layerSize[0] = inputLayerSize;
    layerSize[1] = hiddenLayerSize;
    layerSize[2] = outputLayerSize;
    Initialize(layerSize);
}
// Initialize the network
void BPNN::Initialize(const vector<int> &layerSize){
    // Intialize parameters
    this->layerSize = layerSize;
    output.resize(LayerNum());
    buffer.resize(LayerNum());
    delta.resize(LayerNum());
    weightGrad.resize(LayerNum()-1);
    prevWeightGrad.resize(LayerNum()-1);
    weight.resize(LayerNum()-1);

    // Resize Matrixes
    for(int l = 0; l<LayerNum(); l++){
	int cur = layerSize[l];
	if(l>0){
	    buffer[l].resize(cur, 1);
	    delta[l].resize(cur, 1);
	}
	if(l<LayerNum()-1){
	    int next = layerSize[l+1];
	    weight[l].resize(next, cur+1);
	    weightGrad[l].resize(next, cur+1);
	    prevWeightGrad[l].resize(next, cur+1);
	}
	output[l].resize(cur,1);
    }

    // All the weight is zero except for the bias
    for(int l=0; l<LayerNum()-1; l++){
	weight[l].fill(0);
    }
}

// Compute the output and store
void BPNN::Compute(const MatrixXd &in, MatrixXd& out){
    // Initialize input layer 
    output[0] = in;
    // Compute from the first hidden layer to output layer
    for(int l = 1; l<LayerNum(); l++){
	buffer[l] = weight[l-1]*AddOne(output[l-1]);
	output[l] = Sigmoid(buffer[l]);
    }
    out = output[LayerNum()-1];
}

// Compute and output without store
void BPNN::Compute(const MatrixXd &in){
    // Initialize input layer 
    output[0] = in;
    // Compute from the first hidden layer to output layer
    for(int l = 1; l<LayerNum(); l++){
	buffer[l] = weight[l-1]*AddOne(output[l-1]);
	output[l] = Sigmoid(buffer[l]);
    }
}

// Compute all outputs of set of inputs
void BPNN::ComputeAll(const MatrixXd &in, MatrixXd &out){

    if(out.rows()!=in.rows() || out.cols()!=OutputLayerSize())
	out.resize(in.rows(), OutputLayerSize());
    for(int i =0, numData=in.rows(); i< numData; i++){
	MatrixXd tmp(OutputLayerSize(),1);
	Compute(in.row(i).transpose(), tmp);
	out.row(i) = tmp.transpose();
    }
}

// Run forward propagation to calculate cost, delta, weightGrad and output
double BPNN::Run(const MatrixXd &trainInput, const MatrixXd &trainOuput){
    double cost = 0.0;

    // Step 1: Compute output
    Compute(trainInput);

    // Step 2: Compute delta and accumulate cost 
    for(int l = LayerNum()-1; l>0; --l){
	if(l == LayerNum()-1){
	    delta[l] = output[l]-trainOuput;
	    cost+=delta[l].squaredNorm(); 
	}
	else{
	    delta[l]=RemoveOne(weight[l].transpose()*delta[l+1]).array()*
		output[l].array()*(1-output[l].array()).array();
	}
    }

    // Step 3: Accumulate gradient for weight
    for(int l = 0; l<LayerNum()-1; l++){
	weightGrad[l]+= delta[l+1]*AddOne(output[l]).transpose();
    }

    return cost;
}

// Run epoch with a set of data
double BPNN::RunEpoch(const MatrixXd &trainInput, const MatrixXd &trainOuput, double lambda){
    double cost = 0.0;
    double trainNum = trainInput.rows();

    // Step 1: Set weightGrad 0
    for(int l =0; l<LayerNum()-1; l++){
	weightGrad[l].fill(0);
    }

    // Step 2: Calculate cost function, delta and weightGrad
    for(int i=0; i<trainNum; i++){
	cost += Run(trainInput.row(i).transpose(), trainOuput.row(i).transpose());
    }
    
    // Step 3: Calculate cost and weightGrad regularsized version
    double sumWeight = 0.0;
    for(int l=0; l<LayerNum()-1; l++){
	weightGrad[l]/=trainNum;
	weightGrad[l].block(0,1,layerSize[l+1], layerSize[l])+=
	    lambda*weight[l].block(0,1, layerSize[l+1], layerSize[l])/trainNum;
	sumWeight += weight[l].block(0,1, layerSize[l+1], layerSize[l]).array().square().sum();
    }
    
    // Regularized cost function
    cost = cost/(2*trainNum)+ lambda*sumWeight/trainNum;
    return cost;
}

// Update weight based on Simulated Annealing
void BPNN::Update(double learningRate, double momentum, int iteration, int maxIteration){
    double temp = momentum*exp(-iteration/(double)maxIteration);
    for(int l =0; l<LayerNum()-1; l++){
	weightGrad[l]+= temp*prevWeightGrad[l];
	weight[l]-=learningRate*weightGrad[l];
	prevWeightGrad[l] = weightGrad[l];
    }
}

// Train the network by Backprop algorithm
BPNN BPNN::Train(const MatrixXd &trainInput, const MatrixXd &trainOuput, 
		 double learningRate, double lambda,double momentum, int maxIteration, double threshold, double & cost){

    cost = 0.0; // current cost
    double prevCost = cost;  // previous cost
    double minCost = 100; // min cost so far

    assert(trainInput.cols() == InputLayerSize() && trainOuput.cols() == OutputLayerSize());
    

    for(int l=0; l<LayerNum()-1; l++){
	prevWeightGrad[l].fill(0);
    }
    
    // Loop 
    for(int i =0; i <maxIteration; i++){
        // Run an epoch first
	cost = RunEpoch(trainInput, trainOuput, lambda);
        // Update weight
	Update(learningRate, momentum, i, maxIteration);
        // Check convergence
	if(CheckConvergence(prevCost, cost, minCost, threshold)){
	    cout << "Convert at iteration: "<< i <<endl;
	    break;
	}
        // Update minCost if 
	if(cost<minCost)
	    minCost = cost;
	// Print out for debug
	if(i%100==0)
	    cout << i <<","<<cost <<endl;
    }
    return *this;
}

// Predict patterns
void BPNN::Predict(const MatrixXd &in, MatrixXd &out, MatrixXd &outputLabel){
    int numData = in.rows();
    // Compute the output
    ComputeAll(in, out);
    if(outputLabel.rows()!=in.rows() || outputLabel.cols()!=1)
	outputLabel.resize(in.rows(), 1);
    // Label by max value
    for(int i =0; i < numData; i++){
	int r,c;
	out.row(i).maxCoeff(&r,&c);
	outputLabel(i)= c;
    }
}

// Randomize weight to avoid the same local minima every time running the program
void BPNN::SetWeight(WeightInitType type){
    // Randomize all the weight except the one for bias
    for(int i=0; i<LayerNum()-1; i++){
	if(type==RANDOM)
	    weight[i].setRandom();
	else
	    weight[i].fill(0);
    }
}

// Check if convergence occurs
bool BPNN::CheckConvergence(double prev, double current, double min, double threshold){
    if(current<threshold){
	return true;
    }
    return false;
}

// Save the network to xml files
void BPNN::Save(const string& filename){
    FileStorage file(filename, CV_STORAGE_WRITE);
    Mat cvLayerSize(LayerNum(), 1, CV_32F);
    for(int l =0; l<LayerNum(); ++l){
	cvLayerSize.at<float>(l,0) = layerSize[l];
    }

    file << "NetworkLayerSize" << cvLayerSize;
    for(int l =0; l<LayerNum()-1; ++l){
	stringstream ss;
	ss << "Weight_"<< l;
	file << ss.str() << eigen2cv(weight[l]);
    }
    
}

// Load the network from xml files
void BPNN::Load(const string& filename){
    FileStorage file(filename, CV_STORAGE_READ);
    Mat cvLayerSize;
    file["NetworkLayerSize"] >> cvLayerSize;
    for(int l = 0; l<cvLayerSize.rows; l++)
	layerSize[l] = cvLayerSize.at<float>(l,0);

    vector<Mat> cvWeight(cvLayerSize.rows-1);
    for(int l =0; l<LayerNum()-1; ++l){
	stringstream ss;
	ss << "Weight_"<< l;
	file[ss.str()] >> cvWeight[l];
	weight[l] = cv2eigen(cvWeight[l]);
    }
}

/*Element Access*/
void BPNN::SetWeight(const vector<MatrixXd> &weight){
    this->weight = weight;
}

vector<MatrixXd> BPNN::Weight() const {
    return weight;
}

MatrixXd BPNN::Output() const {
    return output[LayerNum()-1];
}

vector<int> BPNN::LayerSize() const{
    return layerSize;
}

int BPNN::InputLayerSize() const {
    return layerSize[0];
}

int BPNN::OutputLayerSize() const{
    return layerSize[LayerNum()-1];
}

int BPNN::LayerNum() const {
    return layerSize.size();
}




