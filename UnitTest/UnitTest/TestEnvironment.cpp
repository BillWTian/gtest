#include "TestEnvironment.h"

wstring TestEnvironment::current_Microsecond = L"";
wstring TestEnvironment::wstUnitTestPath = L"";

wstring TestEnvironment::getUnitTestPath()
{
    if (wstUnitTestPath == L"")
    {
        //得到当前目录
        wchar_t sGTestPath[500];
        wstring sRemainPath = L"\\..\\..\\..\\..\\..\\unittestdata\\";
        wstring unitTestPath_Release = sGTestPath + sRemainPath;

        wstUnitTestPath = unitTestPath_Release;
        QString sDestPath = QString::fromStdWString(wstUnitTestPath);
        QFileInfo destfile(sDestPath);
    }
    return wstUnitTestPath;
}

/*!
*@brief   获取存储期望信息的路径：\\UnitTest\\Data\\ResultData\\
*@return   wstring
*/
wstring TestEnvironment::getResultDataPath()
{
    wstring sDestTestPath = TestEnvironment::getUnitTestPath();
    sDestTestPath += L"Data\\ResultData\\";
    return sDestTestPath;
}

/*!
*@brief    获取\\UnitTest\\Data\\时间戳\\路径
*@return   wstring
*/
wstring TestEnvironment::getDestPath()
{
    wstring sUnitTestPath = getUnitTestPath();
    wstring sDestTestPath = sUnitTestPath + L"Data\\" + current_Microsecond + L"\\";
    QFileInfo fileInfo(QString::fromStdWString(sDestTestPath));
    auto strFolder = fileInfo.absoluteFilePath();
    strFolder = strFolder.replace("/", "\\");
    sDestTestPath = strFolder.toStdWString();
    return sDestTestPath;
}


/*!
*@brief    构造函数,获取时间戳,新建文件夹
*/
TestEnvironment::TestEnvironment()
{
    //获取当前的微妙数
    LARGE_INTEGER  large_interger;
    __int64  microsecond = large_interger.QuadPart;
    current_Microsecond = to_wstring(microsecond);
    std::wcout << "时间戳的值为" << current_Microsecond << endl;
    //创建时间戳
    wstring stampFilePath = getDestPath();
    if (!createFolder(stampFilePath))
    {
        std::cout << "创建以时间戳命名的文件失败!" << endl;
    }
}

/*!
*@brief   执行所有用例之前的操作库
*/
void TestEnvironment::SetUp()
{

}

/*!
*@brief  执行所有用例之后的操作,TODO
*/
void TestEnvironment::TearDown()
{
}

/*!
*@brief  创建指定的文件夹
*/
bool TestEnvironment::createFolder(wstring filePath)
{	
    QString sDestPath = QString::fromStdWString(filePath); 
    QFileInfo destfile(sDestPath);
    if(!destfile.exists())
    {
        string strDest = sDestPath.toStdString();
        char* char_dest = (char*)strDest.c_str();
        //mkdir创建成功返回0, 创建失败返回-1
        mkdir(char_dest);		
    }
    if(!destfile.exists())
    {
        return false;
    }
    else
    {
        return true;
    }
}

