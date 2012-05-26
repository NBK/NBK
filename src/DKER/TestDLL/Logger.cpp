#include "Logger.h"

using namespace std;

bool CLogger::LOGALL = true;

std::ofstream CLogger::oFile;

void CLogger::init(std::string fileName)
{
	if (LOGALL)
	{
		oFile.open(fileName.c_str(),ios_base::out | ios_base::trunc);
	}
}

void CLogger::addEntry(std::string line)
{
	if (LOGALL)
	{
		oFile << line << endl;
		oFile.flush();
	}
}
