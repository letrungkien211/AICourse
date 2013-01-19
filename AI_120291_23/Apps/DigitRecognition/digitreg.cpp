/*************************************************/
#include <iostream>
#include <Eigen/Dense>
#include <cstdlib>
#include <ctime>
#include <fstream>

// My headers
#include "bpnn.hpp"
#include "utils.hpp"
#include "data.hpp"

/*************************************************/

using namespace std;
using namespace Eigen;
using namespace cv;
using namespace Utils;

int numData = 900;
int numTrain = 20;
int numCross = 100;
int numTest = 100;
int numHidden = 30;
int maxIteration = 2000;
double learningRate = 0.5;
double lambda = 0;
double momentum = 0.5;
double threshold_ = 0.00001;

int Exp1(int numTrain, double noiseRatio, int numHidden, int maxIteration,
	 double learningRate, double lambda, double momentum,
	 double threshold){
    int numNoise = numTrain*noiseRatio;
    
    MatrixXd data, label, 
	trainInput, trainLabel, trainOutput, 
	testInput, testLabel, testOutput,
	crossInput, crossLabel, crossOutput, trainedCrossOutput,
	trainedTestLabel, trainedTestOutput;
    double crossError;
    
    string dataFileName("mnistdata.xml");
    string labelFileName("mnistlabel.xml");


    BPNN bpnn(400,numHidden,10);

    //cout << "Start Load" <<endl;
    Data::Load(dataFileName, labelFileName, data, label, numData);
    // MatrixXd noise;
    // Data::MakeNoise(data, noise, 0.5);
    // Utils::SaveMatrix(noise, string("noise"));
    // Utils::LoadMatrix(noise, string("noise"));
    
    //cout << "Start Convert" <<endl;
    LabelToMatrix(label, 10, trainOutput);
    //cout << "Initialize " << endl;
    trainInput = data.block(0,0,numTrain,400);
    //trainInput.block(0,0,numNoise,400) = noise.block(0,0,numNoise, 400);

    trainLabel = label.block(0,0,numTrain,1);

    for(int i = 0; i < numNoise; i++){
	trainLabel(i) = trainLabel(i+1);
    }
    
    LabelToMatrix(trainLabel, 10, trainOutput);

    crossInput = data.block(numTrain, 0, numCross, 400);
    crossLabel = label.block(numTrain,0,numCross,1);
    LabelToMatrix(crossLabel,10, crossOutput);

    testInput = data.block(numTrain+numCross,0,numTest,400);
    testLabel = label.block(numTrain+numCross,0,numTest,1);
    LabelToMatrix(testLabel,10, testOutput);

//    cout << "Start train" <<endl;
//    cout << trainInput.rows() << " " <<trainInput.cols() << endl;
//    cout << trainOutput.rows() << " " <<trainOutput.cols() << endl;
//    cout << label.rows() << " " <<label.cols() << endl;

    bpnn.SetWeight();
    double trainError;
    bpnn = bpnn.Train(trainInput, trainOutput, learningRate, lambda, momentum, maxIteration, threshold, trainError);
    bpnn.Predict(testInput, trainedTestOutput, trainedTestLabel);
    bpnn.ComputeAll(crossInput, trainedCrossOutput);
    crossError = 1/(2.0*numCross)*((trainedCrossOutput-crossOutput).squaredNorm());
    int cnt = 0;
    for(int i = 0; i < numTest; i++){
	// cout << testLabel(i) << " " <<trainedTestLabel(i)<<endl;
	if(testLabel(i) == trainedTestLabel(i)){
	    cnt++;
	}
    }
    // cout << numHidden <<", " << trainError <<", " << crossError <<", " << cnt/(double)numTest <<endl;
    //cout << numTrain <<", " << trainError <<", " << crossError <<", " << cnt/(double)numTest <<endl;
    //cout << noiseRatio <<", " << trainError <<", " << crossError <<", " << cnt/(double)numTest <<endl;
    //cout << numHidden <<", " << trainError <<", " << crossError <<", " << cnt/(double)numTest <<endl;
    //cout << lambda <<", " << trainError <<", " << crossError <<", " << cnt/(double)numTest <<endl;
    cout << "Train Error: " << trainError <<endl;
    cout << "Cross Validation Error: " << crossError <<endl;
    cout << "Precision: " << cnt/(double)numTest <<endl;
    return 0;
}

int main(int argc, char **argv){
    srand(time(NULL));
    // for(int i = 1; i <= 35; i++){
    // 	Exp1(numTrain*i,0.25, 10, 1000,learningRate,0,momentum,0.00001);
    // }

    // for(int i = 10; i <= 30; i+=3){
    // 	Exp1(300,i,1000,learningRate,0,momentum,0.0001);
    // }

    // for(int i = 1; i <= 25; i++){
    // 	Exp1(300,i/100.0, 10,1000,learningRate,0,momentum,0.00001);
    // }

    // for(int i = 5; i < 60; i+=2){
    // 	Exp1(300,0.25, i,10000,learningRate,0,momentum,0.001);
    // }

    // double lambda[] = {0.00, 0.001, 0.003, 0.01, 0.03, 0.1,0.3, 0.5};
    // for(int i = 0; i < 7; i++){
    // 	Exp1(300,0.25, 50,10000,learningRate,lambda[i],momentum,0.001);
    // }

    Exp1(700,0, 20,1000,learningRate,0.1,momentum,0.001);
}


/*************************************************/

