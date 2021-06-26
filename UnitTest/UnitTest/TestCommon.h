#pragma once
#ifdef system("cls")
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <sstream>
#include <opencv2/opencv.hpp>
using namespace std;

class TestCommon
{
public:
	static std::vector<std::string> splitWithPattern(const std::string& str, const std::string& pattern);
	static std::vector<vector<string>> readCSVData(string path);
	static const char* getModelFile();
	static void writeImage(std::string& fileName, const std::vector<unsigned char>& img);
    static void getFileNames(const QString& path, QStringList* filesList, QStringList filter);
public:
	ofstream result_file;
};