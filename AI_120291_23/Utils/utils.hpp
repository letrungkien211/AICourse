#ifndef UTILS_HPP
#define UTILS_HPP

#include <cv.h>
#include <Eigen/Dense>
#include <string>

namespace Utils{
    using namespace cv;
    using namespace std;
    using namespace Eigen;

    void Unroll(const MatrixXd &src, MatrixXd &des);

    MatrixXd AddOne(const MatrixXd &m);

    MatrixXd RemoveOne(const MatrixXd &m);

    Mat eigen2cv(const MatrixXd& eigenMat);

    MatrixXd cv2eigen(const Mat& cvMat);

    MatrixXd Sigmoid(const MatrixXd& v);

    MatrixXd SigmoidGrad(const MatrixXd& v);

    double Sigmoid(double z);

    double SigmoidGrad(double z);
    
    void LabelToMatrix(const MatrixXd &label, int outputSize, MatrixXd &output);

    void SaveMatrix(const MatrixXd &m, const string &filename);

    void LoadMatrix(MatrixXd &m, const string &filename);
};

#endif
