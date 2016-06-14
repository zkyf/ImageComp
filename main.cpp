#define _USE_MATH_DEFINES

#include <string>
#include <opencv2\opencv.hpp>
#include <vector>
#include <ctime>
#include <math.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "bp.h"
#include "icUI.h"

using namespace std;
using namespace cv;

int main(){
	Mat input = imread("test.png");
	input.convertTo(input, CV_8UC3);
	Mat mask = getMask(input);
	getPoints(input, mask);
	return 0;
}