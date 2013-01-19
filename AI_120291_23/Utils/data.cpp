#include "data.hpp"
#include <string>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
using namespace cv;
using namespace Utils;

void Data::Image2Matrix(const string &dirName, 
			const string &imgFileName,
			const string &outputFileName, 
			int imgNum){
    char fname[100];
    char nodename[100];
    sprintf(fname, "%s/%s.xml", dirName.c_str(), outputFileName.c_str());
    FileStorage cbfs(fname, CV_STORAGE_WRITE);

    cout << "Start Image2Matrix" <<endl;
    for(int i = 0; i<imgNum; i++){
	sprintf(fname, "%s/%s%04d.jpg", dirName.c_str(), imgFileName.c_str(),i+1);
//	cout << fname <<endl;
	Mat cimg = imread(fname);
	Mat gimg;
	cvtColor(cimg, gimg,CV_RGB2GRAY);
	Mat fimg;
	gimg.convertTo(fimg, CV_32F);
	fimg/=255;
	sprintf(nodename, "%s%04d", outputFileName.c_str(), i);
	cbfs << string(nodename) << fimg;
    }
}


void Data::Load(const string &dataFileName, 
		const string &labelFileName,
		MatrixXd &trainData, 
		MatrixXd &trainLabel,
		int numData)
{
    FileStorage dataFile(dataFileName, CV_STORAGE_READ);
    FileStorage labelFile(labelFileName, CV_STORAGE_READ);

    Mat tmpTrainLabel;
    labelFile["mnistlabel"] >> tmpTrainLabel;
    trainLabel = cv2eigen(tmpTrainLabel);
    assert(numData <= trainLabel.rows());
//    cout << "Start Load" <<endl;
    char str[100];
    for(int i = 0 ; i<numData; i++){
//	cout << "#" << i<<endl;
	sprintf(str, "%s%04d", "mnistdata", i);
//	cout <<str<<endl;
	Mat data;
	dataFile[str] >> data;
	assert(data.rows>0);
	if(i==0){
	    if(trainData.rows()!=numData || trainData.cols()!=data.rows*data.cols)
		trainData.resize(numData, data.rows*data.cols);
	}
	MatrixXd tmp;
//	cout << "#" << i<<endl;
	Unroll(cv2eigen(data), tmp);
//	cout << "#" << i<<endl;
	trainData.row(i) = tmp;
    }
}

void Data::MakeNoise(const MatrixXd &src, MatrixXd &des, double range){
    unsigned int rows = src.rows();
    unsigned int cols = src.cols();
    if(des.rows()!=rows || des.cols()!=cols){
	des.resize(rows, cols);
    }
    for(int i = 0; i < rows; i++){
	for(int j = 0; j <cols; j++){
	    des(i,j) = src(i,j)+(range*(rand()/(double)RAND_MAX)-range/2.0);
	    des(i,j) = des(i,j) > 1 ? 1 : des(i,j);
	    des(i,j) = des(i,j) < 0 ? 0 : des(i,j);
	}
    }
}
