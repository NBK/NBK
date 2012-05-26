#include "system.h"
#include <GL/gl.h>
#include <stdio.h>
#include <fstream>
#include "Logger.h"

using namespace std;

namespace utils
{
	GLint CLogger::indices = 0;

	CLogger::CLogger()
	{
	}

	CLogger::~CLogger()
	{
	}

	DWORD st[100];

	GLvoid CLogger::setEntryStart()
	{
		st[getNextFreeIndex()]=timeGetTime();
	}

	GLint CLogger::getNextFreeIndex()
	{
		return indices++;
	}

	GLint CLogger::getLastIndex()
	{
		return --indices;
	}

	GLvoid CLogger::setEntryEnd(const char *text, ...)
	{
		GLint ref = getLastIndex();

		static bool first_line=true;

		if (first_line)
		{
#ifdef WIN32
			SYSTEMTIME systime;
			GetSystemTime(&systime);
#else
			time_t currenttime;
			struct tm *systime;
			currenttime = time(NULL);
			systime = localtime(&currenttime);
#endif

			char date[100];
#ifdef WIN32
			sprintf(date,"%d.%d.%d - %d:%d:%d",systime.wDay,systime.wMonth,systime.wYear,systime.wHour,systime.wMinute,systime.wSecond);
#else
			strftime(date,sizeof(date),"%d.%m.%Y - %H:%M:%S",systime);
#endif

			FILE *log_file=fopen("logger.log","wc");
			if (!log_file)
			{
				return;
			}
			first_line=false;
			char string[100];
			sprintf(string,"New entry: %s\n------------------------------\n",date);
			fwrite(string,1,strlen(string),log_file);
			fclose(log_file);
		}

		char string[256];
		va_list	ap;

		if (text == NULL)
		{
			return;
		}

		va_start(ap,text);
		GLint string_length=vsprintf(string,text,ap);
		va_end(ap);

		FILE *log_file=fopen("logger.log","a");
		if (!log_file)
		{
			return;
		}

		if (ref==-1)
		{
			string[string_length]='\n';
		}

		if (ref!=-1)
		{
			char line[256];

			for (int i=0; i<256; i++)
			{
				line[i] = ' ';
			}

			for (int i=0; i<string_length; i++)
			{
				line[i] = string[i];
			}

			string_length=sprintf(string,"Time taken: %ld ms\n",timeGetTime()-st[ref]);

			memcpy(line+64,string,string_length);

			fwrite(line,1,string_length+64,log_file);
		}

		fclose(log_file);
	}

	GLvoid CLogger::addEntry(const char *text, ...)
	{

		char string[256];
		va_list	ap;

		if (text == NULL)
		{
			return;
		}

		va_start(ap,text);
		GLint string_length=vsprintf(string,text,ap);
		va_end(ap);

		FILE *log_file=fopen("logger.log","a");
		if (!log_file)
		{
			return;
		}

		fwrite(string,1,string_length,log_file);

		fclose(log_file);
	}

	GLvoid CLogger::getLog(vector<string> &log)
	{
		ifstream iFile;

		iFile.open("logger.log",ios_base::in);

		char line[512];

		while (iFile.good())
		{
			iFile.getline(line,512,'\n');
			log.push_back(string(line));
		}

		iFile.close();
	}
};