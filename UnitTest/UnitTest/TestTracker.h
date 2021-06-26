#pragma once
#include "TestCommon.h"
#include "gtest/gtest.h"
#include "TestConst.h"
#include "TestCompare.h"

class TestTracker :public TestCommon, public ::testing::Test
{
protected:
	void SetUp();
	void TearDown();
	void verifyTracker();
public:
	std::vector<vector<string>> testData;
};


