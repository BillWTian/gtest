#pragma once
#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class TestCompare
{
public:
	static void equal(int expect, int actual, string property="");
	static void lessThan(int expect, int actual, string property);
	static void compareImageSimilarity(Mat expect, Mat actual);
};
