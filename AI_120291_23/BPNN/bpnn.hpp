#ifndef BPNN_HPP
#define BPNN_HPP

#include <Eigen/Dense>
#include <vector>
#include "utils.hpp"
using namespace std;
using namespace Eigen;

class BPNN{
private:
    // Data
    vector<MatrixXd> weight;
    vector<MatrixXd> weightGrad;
    vector<MatrixXd> prevWeightGrad;
    vector<MatrixXd> delta;
    vector<MatrixXd> output;
    vector<MatrixXd> buffer;
    vector<int> layerSize;

    // Update
    void Update(double learningRate, double momentum, int iteration, int maxIteration);
    // Check if convergence occurs
    bool CheckConvergence(double prev, double current, double min, double threshold);

public:
    // Weight initialize type
    enum WeightInitType {RANDOM, ZEROS};
    // Default constructor
    BPNN();

    // Contructor with layers' size
    BPNN(const vector<int> &layerSize);

    // Contructor 3 layer size
    BPNN(int inputLayerSize, int hiddenLayerSize, int outputLayerSize);

    // Initialization
    void Initialize(const vector<int>& layerSize);

    // Compute output and store
    void  Compute(const MatrixXd &in, MatrixXd& out);

    // Compute output
    void  Compute(const MatrixXd &in);

    // Compute output for a training set
    void ComputeAll(const MatrixXd &in, MatrixXd& out);
    
    // FeedForward
    double Run(const MatrixXd &in, const MatrixXd &out);
    
    // FeedForward with training data set
    double RunEpoch(const MatrixXd &trainInput, const MatrixXd &trainOutput, double lambda = 0);

    // Backpropagation training
    BPNN Train(const MatrixXd &trainInput, const MatrixXd &trainOuput, 
	       double learningRate, double lambda, double momentum,
	       int maxIteration, double threshold, double & cost);

    // Predict patterns
    void Predict(const MatrixXd &in, MatrixXd &out, MatrixXd &outputLabel);

    // Save the network to xml file
    void Save(const string& filename);

    // Load the network from xml file
    void Load(const string& filename);

    /*Element Access*/
    void SetWeight(const vector<MatrixXd> &weight);
    void SetWeight(WeightInitType type = RANDOM);
    vector<MatrixXd> Weight() const;
    MatrixXd Output() const;
    vector<int> LayerSize() const;
    int InputLayerSize() const ;
    int OutputLayerSize() const ;
    int LayerNum() const ;
};
#endif
