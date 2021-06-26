#include <chrono>
#include <windows.h>
#include "TestTracker.h"


using namespace std;
using namespace cv;

static const string outputFile = testDataPath + result;

void TestTracker::SetUp()
{
    //if (!initLicense()) return -1;
    result_file.open(outputFile, ios::app);
    if (!result_file.is_open()) {
        return;
    }
}

void TestTracker::verifyTracker()
{
    const char* assetFolder = TestCommon::getModelFile();

    for (size_t i = 1; i < testData.size(); i++)
    {
        vector<string> rowData = testData[i];
        cerr << rowData[0] + "--" + rowData[1] + ":" << endl;
        if (rowData.empty())
            break;

        string isCompareImage = "";
        if (rowData.size() >= 5) { 
            isCompareImage = rowData[4];//这列验证实际图片和期望图片是否一张图片
         }
        int actualValue = 0;

        TestCompare::equal(atoi(rowData[3].c_str()), actualValue);
    }

}

TEST_F(TestTracker, verifySetFaceOrientation)
{
    testData = TestCommon::readCSVData(testDataPath + "setFaceOrientation.csv");
    verifyTracker();
}


void TestTracker::TearDown()
{
    result_file.close();
}


