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

const int WCHARSEQUAL = 0; //_tcscmp(a,b)比较两个wchar_t相等,则返回0
const int FOURPARAMS = 4;  //命令行参入参数的个数

wchar_t* GetProcessName(int argc, _TCHAR* argv[], int& suiteNumber);
char* GetXMLFileInfo(int argc, _TCHAR* argv[], int& suiteNumber);
int GetRepeatRunTime(int argc, _TCHAR* argv[], int& suiteNumber);
char* GetTestSuiteFilter(int suiteNumber, _TCHAR* argv[]);
void EndProgram(wchar_t* processName);
void initApplication();


//cmd命令行的输入的格式UnitTest.exe  "TestM*:"  -fileName  "TestPM"  -autoExit "nitTestd.exe"
int _tmain(int argc, _TCHAR* argv[])
{
//	TestCaseManager::SetUnitTestObjPoint();

	int suiteNumber = argc;//获取运行用例使用的参数
	//获取进程名
	wchar_t* processName = GetProcessName(argc, argv, suiteNumber);
	//获取存储结果的文件名(路径默认)
	char* resultfileinfo = GetXMLFileInfo(argc, argv, suiteNumber);
	//获取重跑次数
	int repeat_time = GetRepeatRunTime(argc, argv, suiteNumber);
	//获取运行的用例名   
	char* testSuite_result = GetTestSuiteFilter(suiteNumber, argv);
	if (testSuite_result[0] != '\0')//命令行中指定了运行范围，使用命令行制定范围
	{
		testing::GTEST_FLAG(filter) = testSuite_result;
	}
	
	else//命令行未制定范围，使用下面的运行范围
	{

		testing::GTEST_FLAG(filter) =
			//"TestCaseName.verify1Model"
			;
	}

	initApplication();

	testing::GTEST_FLAG(repeat) = repeat_time;

#ifdef NDEBUG
	testing::GTEST_FLAG(catch_exceptions) = true;//输出捕捉异常, 继续执行下一条 
#else
	testing::GTEST_FLAG(catch_exceptions) = false;//捕捉异常, 弹窗异常信息
#endif	

	testing::GTEST_FLAG(output) = resultfileinfo;//设置测试结果输出到指定文件
	testing::AddGlobalTestEnvironment(new TestEnvironment);

	testing::InitGoogleTest(&argc, argv);


	RUN_ALL_TESTS();

	//outputFilterCaseName();

	delete[]resultfileinfo;
	delete[]testSuite_result;

	TerminateProcess(GetCurrentProcess(), 0);
	//EndProgram(processName);//结束程序,自动结束进程
	//delete[] processName;
}



/*!
*@brief    获取进程名,用例运行结束,自动结束进程

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
*@brief    获取存储运行结果的XML的路径和文件名
*@param    int argc, 命令行中传入参数的个数
*@param    _TCHAR* argv[], 命令行中传入的参数
*@param    int &suiteNumber, 获取用例时使用的参数下标
*@return   存储运行结果的XML的路径和文件名,
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
				//下面的两句必须分开
				
				string strFileName = qs_FileName.toStdString();
				cresultfileinfo_name = (char*)strFileName.c_str();
				int resultfile_length = strlen(cresultfileinfo_path.toStdString().c_str()) + strlen(cresultfileinfo_name) + strlen(cresultfileinfo_suffix);

				char* resultfileinfo = new char[resultfile_length + 1];
				//合并结果文件的路径和名字
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
*@brief    获取重复运行的次数
*@param    int argc, 命令行中传入参数的个数
*@param    _TCHAR* argv[], 命令行中传入的参数
*@param    int &suiteNumber, 获取运行的用例使用的参数
*@return   重复运行的次数
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
*@brief    获取运行用例的集合(命令行中可以指定多个用例,每个用例后面需要加冒号)
*@param    int &suiteNumber, 获取运行的用例使用的参数
*@param    _TCHAR* argv[], 命令行中传入的参数
*@return   运行用例的集合,需手动释放内存
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
*@brief    结束指定的进程
*@param    wchar_t* processName, 进程名
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