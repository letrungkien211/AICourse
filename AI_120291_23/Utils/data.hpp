#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <cv.h>
#include <Eigen/Dense>
#include "utils.hpp"

using namespace std;
using namespace cv;
using namespace Eigen;
class Data{
public:
    static void Load(const string &dataFileName, 
		     const string &labelFileName,
		     MatrixXd &trainData, 
		     MatrixXd &trainLabel,
		     int numData);
    static void Image2Matrix(const string &dirName, 
			     const string &imgFileName,
			     const string &outputFileName, 
			     int imgNum);
    static void MakeNoise(const MatrixXd &src, MatrixXd &des, double range);
};

#endif
