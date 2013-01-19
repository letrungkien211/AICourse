#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <cstdio>
#include <vector>

#include "data.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv){
    string dirName(argv[1]);
    string imgFileName(argv[2]);
    string outputFileName(argv[3]);
    int imgNum = atoi(argv[4]);

    Data::Image2Matrix(dirName, imgFileName, outputFileName, imgNum);

    return 0;
}

