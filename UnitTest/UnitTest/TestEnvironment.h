#pragma once
#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <stdlib.h>
#include <direct.h>
#include <QFileInfo>
using namespace std;

class TestEnvironment : public testing::Environment
{
public:
    TestEnvironment();
    //���а���ִ��ǰ
    virtual void SetUp();
    //���а���ִ�к�
    virtual void TearDown();
    static wstring getDestPath();
    static wstring getUnitTestPath();
    static wstring getResultDataPath();
    static bool createFolder(wstring filePath);
public:
    static wstring current_Microsecond;
private:
    static wstring wstBinPath;
    static wstring wstUnitTestPath;
};