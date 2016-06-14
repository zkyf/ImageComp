#include "icUI.h"

#include <iostream>
using namespace std;

static Point2i mouse_pos;
static int mouse_event;
static int mouse_flags;

static void mouseCallBack(int event, int x, int y, int flags, void* ustc)
{
	mouse_event = event;
	mouse_pos = Point(x, y);
	mouse_flags = flags;
}

Mat getMask(Mat input)
{
	if (input.empty()) return Mat();
	Mat ret(input.size(), CV_8U, Scalar::all(0));
	Mat toshow = input.clone();
	int size = 10;
	imshow("Generate Input Mask", toshow);
	setMouseCallback("Generate Input Mask", mouseCallBack);
	while (1)
	{
		Mat totoshow = toshow.clone();
		char key = waitKey(10);
		if (key == '[')
		{
			size = (size > 1) ? size - 1 : size;
		}
		else if (key == ']')
		{
			size++;
		}
		if (mouse_event == CV_EVENT_RBUTTONDOWN)
		{
			break;
		}
		if ((mouse_event == CV_EVENT_MOUSEMOVE && (mouse_flags & CV_EVENT_FLAG_LBUTTON)) ||
				(mouse_event == CV_EVENT_LBUTTONDOWN))
		{
			circle(ret, mouse_pos, size, Scalar(255), -1);
			circle(toshow, mouse_pos, size, Scalar(255, 0, 0), -1);
			circle(totoshow, mouse_pos, size, Scalar(255, 0, 0), -1);
		}
		circle(totoshow, mouse_pos, size, Scalar(0, 0, 255), -1);
		imshow("Generate Input Mask", totoshow);
	}
	destroyWindow("Generate Input Mask");
	return ret;
}

vector<icPoint> getPoints(Mat input, Mat mask)
{
	vector<icPoint> ret;
	if (input.empty()) return ret;
	const int size = 2;
	const int error = 3;
	int count = 0;
	int lpoint = -1;
	imshow("Specify structure curves", input);
	setMouseCallback("Specify structure curves", mouseCallBack);
	Mat _mask;
	cvtColor(mask, _mask, CV_GRAY2BGR);
	Mat temp;
	Mat lines(input.size(), CV_8U, Scalar::all(0));
	bitwise_xor(input, _mask, temp);
	bool conti = true;
	while (1)
	{
		Mat toshow = temp.clone();
		if (mouse_event == CV_EVENT_LBUTTONDOWN)
		{
			bool selected = false;
			for (int i = 0; i < count; i++)
			{
				Point diff = ret[i].pos - mouse_pos;
				double err = sqrt(diff.x*diff.x + diff.y*diff.y);
				if (err <= error)
				{
					selected = true;
					if (i == lpoint || lpoint == -1) break;
					cout << "Clicked old point # " << i << endl;
					if (conti)
					{
						ret[i].neighbors.push_back(lpoint);
						ret[lpoint].neighbors.push_back(i);
					}
					lpoint = i;
					break;
				}
			}
			if (!selected)
			{
				icPoint newp;
				newp.pos = mouse_pos;
				newp.num = count;
				if (mask.at<uchar>(newp.pos) > 0)
				{
					newp.type = icPoint::INNER;
				}
				else
				{
					newp.type = icPoint::OUTER;
				}
				if (conti)
				{
					if (lpoint != -1)
					{
						newp.neighbors.push_back(lpoint);
						ret[lpoint].neighbors.push_back(count);
					}
				}
				ret.push_back(newp);
				lpoint = count;
				count++;
			}
			conti = true;
			cout << "Now in total " << count << " anchors" << endl;
		}
		for (int i = 0; i < count; i++)
		{
			for (int j = 0; j < ret[i].neighbors.size(); j++)
			{
				int nei = ret[i].neighbors[j];
				line(toshow, ret[i].pos, ret[nei].pos, Scalar(255, 0, 0));
			}
			if (i == lpoint && conti)
			{
				circle(toshow, ret[i].pos, size, Scalar(0, 255, 0), -1);
			}
			else
			{
				switch(ret[i].type)
				{
					case icPoint::INNER:
					circle(toshow, ret[i].pos, size, Scalar(255, 0, 0), -1); break;
					case icPoint::OUTER:
					circle(toshow, ret[i].pos, size, Scalar(37, 127, 255), -1); break;
					case icPoint::BORDER:
					circle(toshow, ret[i].pos, size, Scalar(0, 0, 255), -1); break;
				}
			}
		}
		circle(toshow, mouse_pos, size, Scalar(0, 0, 255), -1);
		if (mouse_event == CV_EVENT_RBUTTONDOWN)
		{
			conti = false;
		}
		imshow("Specify structure curves", toshow);
		char key = waitKey(10);
		if (key==' ')
		{
			ret[lpoint].type = icPoint::BORDER;
		}
		if(key == 27)
		{
			break;
		}
	}
	destroyWindow("Specify structure curves");
	for (int i = 0; i < ret.size(); i++)
	{

	}
	return ret;
}

Mat getRegions(Mat input, Mat mask, vector<icPoint> points)
{
	int top = input.rows, bottom = 0;
	int left = input.cols, right = 0;
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].pos.x < left) left = points[i].pos.x;
		if (points[i].pos.x > right) right = points[i].pos.x;
		if (points[i].pos.y < top) top = points[i].pos.y;
		if (points[i].pos.y > bottom) bottom = points[i].pos.y;
	}
	return Mat();
}