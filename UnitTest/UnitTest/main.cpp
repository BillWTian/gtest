#include "gtest/gtest.h"
#include <QTranslator>
#include "TestEnvironment.h"
#include <QtWidgets/QApplication>
#include <QString>
#include <QSet>
#include <QSettings>
#include <conio.h>
#include <cstring>

#include <tchar.h>
#include <string>
using namespace std;

const int WCHARSEQUAL = 0; //_tcscmp(a,b)�Ƚ�����wchar_t���,�򷵻�0
const int FOURPARAMS = 4;  //�����в�������ĸ���

wchar_t* GetProcessName(int argc, _TCHAR* argv[], int& suiteNumber);
char* GetXMLFileInfo(int argc, _TCHAR* argv[], int& suiteNumber);
int GetRepeatRunTime(int argc, _TCHAR* argv[], int& suiteNumber);
char* GetTestSuiteFilter(int suiteNumber, _TCHAR* argv[]);
void EndProgram(wchar_t* processName);
void initApplication();


//cmd�����е�����ĸ�ʽUnitTest.exe  "TestM*:"  -fileName  "TestPM"  -autoExit "nitTestd.exe"
int _tmain(int argc, _TCHAR* argv[])
{
//	TestCaseManager::SetUnitTestObjPoint();

	int suiteNumber = argc;//��ȡ��������ʹ�õĲ���
	//��ȡ������
	wchar_t* processName = GetProcessName(argc, argv, suiteNumber);
	//��ȡ�洢������ļ���(·��Ĭ��)
	char* resultfileinfo = GetXMLFileInfo(argc, argv, suiteNumber);
	//��ȡ���ܴ���
	int repeat_time = GetRepeatRunTime(argc, argv, suiteNumber);
	//��ȡ���е�������   
	char* testSuite_result = GetTestSuiteFilter(suiteNumber, argv);
	if (testSuite_result[0] != '\0')//��������ָ�������з�Χ��ʹ���������ƶ���Χ
	{
		testing::GTEST_FLAG(filter) = testSuite_result;
	}
	
	else//������δ�ƶ���Χ��ʹ����������з�Χ
	{

		testing::GTEST_FLAG(filter) =
			//"TestCaseName.verify1Model"
			;
	}

	initApplication();

	testing::GTEST_FLAG(repeat) = repeat_time;

#ifdef NDEBUG
	testing::GTEST_FLAG(catch_exceptions) = true;//�����׽�쳣, ����ִ����һ�� 
#else
	testing::GTEST_FLAG(catch_exceptions) = false;//��׽�쳣, �����쳣��Ϣ
#endif	

	testing::GTEST_FLAG(output) = resultfileinfo;//���ò��Խ�������ָ���ļ�
	testing::AddGlobalTestEnvironment(new TestEnvironment);

	testing::InitGoogleTest(&argc, argv);


	RUN_ALL_TESTS();

	//outputFilterCaseName();

	delete[]resultfileinfo;
	delete[]testSuite_result;

	TerminateProcess(GetCurrentProcess(), 0);
	//EndProgram(processName);//��������,�Զ���������
	//delete[] processName;
}



/*!
*@brief    ��ȡ������,�������н���,�Զ���������

*/
wchar_t* GetProcessName(int argc, _TCHAR* argv[], int& suiteNumber)
{
	bool isAutoExit = false;
	wchar_t* process_Name = L"";
	if (argc >= FOURPARAMS)
	{
		const _TCHAR cStrAutoParam[] = _T("-autoExit");
		for (int i = 1; i < argc; ++i)
		{
			int autoExitParam = _tcscmp(argv[i], cStrAutoParam);
			if (autoExitParam == WCHARSEQUAL)
			{
				process_Name = argv[i + 1];
				suiteNumber -= 2;
				break;
			}
		}
	}
	int processNameLength = wcslen(process_Name);
	wchar_t* processName = new wchar_t[processNameLength + 1];
	wcscpy(processName, process_Name);
	return processName;
}

/*!
*@brief    ��ȡ�洢���н����XML��·�����ļ���
*@param    int argc, �������д�������ĸ���
*@param    _TCHAR* argv[], �������д���Ĳ���
*@param    int &suiteNumber, ��ȡ����ʱʹ�õĲ����±�
*@return   �洢���н����XML��·�����ļ���,
*/
char* GetXMLFileInfo(int argc, _TCHAR* argv[], int& suiteNumber)
{
	QString xmlPathHeadName = "xml:";
	const _TCHAR cStrFileName[] = _T("-fileName");
	QString qs_FileName = "";

	auto strConfigFile = QApplication::applicationDirPath() + "/UnitTestConfig.ini";
	QSettings* configIniRead = new QSettings(strConfigFile, QSettings::IniFormat);
	QString cresultfileinfo = xmlPathHeadName + configIniRead->value("report/cresultfileinfo").toString();
	QString cresultfileinfo_path = xmlPathHeadName + configIniRead->value("report/path").toString();

	char* cresultfileinfo_name = "";
	char* cresultfileinfo_suffix = ".xml";

	if (argc >= FOURPARAMS)
	{
		for (int i = 1; i < argc; ++i)
		{
			int fileNameParam = _tcscmp(argv[i], cStrFileName);
			if (fileNameParam == WCHARSEQUAL)
			{
				qs_FileName = QString::fromWCharArray(argv[i + 1]);
				//������������ֿ�
				
				string strFileName = qs_FileName.toStdString();
				cresultfileinfo_name = (char*)strFileName.c_str();
				int resultfile_length = strlen(cresultfileinfo_path.toStdString().c_str()) + strlen(cresultfileinfo_name) + strlen(cresultfileinfo_suffix);

				char* resultfileinfo = new char[resultfile_length + 1];
				//�ϲ�����ļ���·��������
				strcpy(resultfileinfo, cresultfileinfo_path.toStdString().c_str());
				strcat(resultfileinfo, cresultfileinfo_name);
				strcat(resultfileinfo, cresultfileinfo_suffix);
				suiteNumber -= 2;

				return resultfileinfo;
			}
		}
	}
	int resultfile_length = cresultfileinfo.length();
	char* resultfileinfo = new char[resultfile_length + 1];
	strcpy(resultfileinfo, cresultfileinfo.toStdString().c_str());
	delete configIniRead;
	return resultfileinfo;
}

/*!
*@brief    ��ȡ�ظ����еĴ���
*@param    int argc, �������д�������ĸ���
*@param    _TCHAR* argv[], �������д���Ĳ���
*@param    int &suiteNumber, ��ȡ���е�����ʹ�õĲ���
*@return   �ظ����еĴ���
*/
int GetRepeatRunTime(int argc, _TCHAR* argv[], int& suiteNumber)
{
	const _TCHAR cRepeat[] = _T("-repeat");
	int repeat_time = 1;
	if (argc >= FOURPARAMS)
	{
		for (int i = 1; i < argc; ++i)
		{
			int repeatParam = _tcscmp(argv[i], cRepeat);
			if (repeatParam == WCHARSEQUAL)
			{
				QString strRepeatTime = QString::fromWCharArray(argv[i + 1]);
				repeat_time = strRepeatTime.toInt();
				suiteNumber -= 2;
				break;
			}
		}
	}
	return repeat_time;
}

/*!
*@brief    ��ȡ���������ļ���(�������п���ָ���������,ÿ������������Ҫ��ð��)
*@param    int &suiteNumber, ��ȡ���е�����ʹ�õĲ���
*@param    _TCHAR* argv[], �������д���Ĳ���
*@return   ���������ļ���,���ֶ��ͷ��ڴ�
*/
char* GetTestSuiteFilter(int suiteNumber, _TCHAR* argv[])
{
	QString qs_testsuites = "";
	for (int j = 1; j < suiteNumber; ++j)
	{
		qs_testsuites += QString::fromWCharArray(argv[j]);
	}
	string strtestSuite = qs_testsuites.toStdString();
	char* char_testsuites = (char*)strtestSuite.c_str();
	char* testsuites = new char[strlen(char_testsuites) + 1];
	strcpy(testsuites, char_testsuites);
	return testsuites;
}

/*!
*@brief    ����ָ���Ľ���
*@param    wchar_t* processName, ������
*@return   void
*/
void EndProgram(wchar_t* processName)
{
	printf("press any key to end...\n");
	int ch = _getch();
}

void initApplication()
{

}