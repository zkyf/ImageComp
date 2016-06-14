#include "public.h"

#ifndef _IC_UI_H
#define _IC_UI_H

// Get the mask of the area to complete
Mat getMask(Mat input);
// Get the user specified curves
vector<icPoint> getPoints(Mat input, Mat mask);

#endif