#include <opencv2\opencv.hpp>
using namespace cv;

#include <iostream>
#include <vector>
using namespace std;

#ifndef _IC_PUBLIC_H
#define _IC_PUBLIC_H

struct icPoint
{
	enum PointType
	{
		BORDER,
		INNER,
		OUTER
	};
	Point2i pos;
	Point2i from;
	int num;
	vector<int> neighbors;
	PointType type;
};

#endif