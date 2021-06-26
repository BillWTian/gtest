#include "TestCompare.h"
#include <QDebug>
#include "TestCompare.h"

void TestCompare::equal(int expect, int actual, string property) {
    EXPECT_EQ(actual, expect) << property << " expect：" << expect << ";actual:" << actual;
}

void TestCompare::lessThan(int expect, int actual,string property) {
	if (expect> actual)
	{
		ADD_FAILURE() << property+" acutal value should be greater than expect value";
	}
}

void TestCompare::compareImageSimilarity(Mat expect, Mat actual)
{
	Mat matDst1, matDst2;
	if (!expect.data || !actual.data)           // Check for success
	{
		ADD_FAILURE() << " Couldn't read the test image！！！ ";
		return;
	}

	cv::resize(expect, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	cv::resize(actual, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

	cv::cvtColor(matDst1, matDst1, COLOR_BGR2GRAY);
	cv::cvtColor(matDst2, matDst2, COLOR_BGR2GRAY);

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++)
	{
		uchar* data1 = matDst1.ptr<uchar>(i);
		uchar* data2 = matDst2.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++)
		{
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
		if (arr1[i] != arr2[i]) {
			++iDiffNum;
		}
	//如果不相同的数据位数不超过10，就说明两张图像很相似；如果大于20，就说明这是两张不同的图像
		cerr << "The two image similarity：" << iDiffNum << endl;
		EXPECT_LE(iDiffNum, 20);
}