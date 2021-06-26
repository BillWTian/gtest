#include "TestCommon.h"
#include <ActiveQt/QAxObject>
#include <QDebug>
#include "TestConst.h"
#include <QDir>

const char* TestCommon::getModelFile() {
    string modelfilePath = "../../../build/output/assets";
    return modelfilePath.c_str();
}

std::vector<std::string> TestCommon::splitWithPattern(const std::string& str, const std::string& pattern)
{
    std::vector<std::string> resVec;
    QStringList data_info = QString::fromStdString(str).split(QString::fromStdString(pattern));
    for (int i = 0; i < data_info.size(); i++) {
        resVec.push_back(data_info[i].toStdString());
        cerr << data_info[i].toStdString() << endl;
    }
    return resVec;
}

std::vector<vector<string>> TestCommon::readCSVData(string path) {
    ifstream csvFile(path, ios::in);
    string lineString;
    vector<vector<string>> strArray;
    while (getline(csvFile, lineString))
    {
        // 打印整行字符串  
        cout << lineString << endl;
        // 存成二维表结构  
        std::stringstream ss(lineString);
        string str;
        vector<string> lineArray;
        // 按照逗号分隔  
        while (getline(ss, str, ','))
            lineArray.push_back(str);
        strArray.push_back(lineArray);
    }
    return strArray;
}



void TestCommon::writeImage(std::string& fileName, const std::vector<unsigned char>& img)
{
    ofstream fs(fileName, ios::binary);
    fs.write((const char*)&(img.front()), img.size());
    fs.close();
}

void TestCommon::getFileNames(const QString& filePath, QStringList* filesList, QStringList filter)
{
    QDir dir(filePath);
    if (!dir.exists()) {
        return;
    }
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);

    QFileInfoList list = dir.entryInfoList();

    QStringList fileinfolist = dir.entryList(filter, QDir::Files | QDir::NoDotAndDotDot);
    if (list.size() < 1) {
        return;
    }
    int i = 0;

    do {
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();
        if (bisDir) {
            getFileNames(fileInfo.filePath(), filesList, filter);
        }
        else {
            for (int j = 0; j < fileinfolist.size(); j++) {
                filesList->push_back(fileInfo.path() + '/' + fileinfolist.at(j));
            }
            break;
        }
        i++;
    } while (i < list.size());

}


