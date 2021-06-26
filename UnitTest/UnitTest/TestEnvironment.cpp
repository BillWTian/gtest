#include "TestEnvironment.h"

wstring TestEnvironment::current_Microsecond = L"";
wstring TestEnvironment::wstUnitTestPath = L"";

wstring TestEnvironment::getUnitTestPath()
{
    if (wstUnitTestPath == L"")
    {
        //�õ���ǰĿ¼
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
*@brief   ��ȡ�洢������Ϣ��·����\\UnitTest\\Data\\ResultData\\
*@return   wstring
*/
wstring TestEnvironment::getResultDataPath()
{
    wstring sDestTestPath = TestEnvironment::getUnitTestPath();
    sDestTestPath += L"Data\\ResultData\\";
    return sDestTestPath;
}

/*!
*@brief    ��ȡ\\UnitTest\\Data\\ʱ���\\·��
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
*@brief    ���캯��,��ȡʱ���,�½��ļ���
*/
TestEnvironment::TestEnvironment()
{
    //��ȡ��ǰ��΢����
    LARGE_INTEGER  large_interger;
    __int64  microsecond = large_interger.QuadPart;
    current_Microsecond = to_wstring(microsecond);
    std::wcout << "ʱ�����ֵΪ" << current_Microsecond << endl;
    //����ʱ���
    wstring stampFilePath = getDestPath();
    if (!createFolder(stampFilePath))
    {
        std::cout << "������ʱ����������ļ�ʧ��!" << endl;
    }
}

/*!
*@brief   ִ����������֮ǰ�Ĳ�����
*/
void TestEnvironment::SetUp()
{

}

/*!
*@brief  ִ����������֮��Ĳ���,TODO
*/
void TestEnvironment::TearDown()
{
}

/*!
*@brief  ����ָ�����ļ���
*/
bool TestEnvironment::createFolder(wstring filePath)
{	
    QString sDestPath = QString::fromStdWString(filePath); 
    QFileInfo destfile(sDestPath);
    if(!destfile.exists())
    {
        string strDest = sDestPath.toStdString();
        char* char_dest = (char*)strDest.c_str();
        //mkdir�����ɹ�����0, ����ʧ�ܷ���-1
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

