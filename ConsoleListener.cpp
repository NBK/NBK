#include "ConsoleListener.h"
#include "utils.h"
#include "Conversions.h"

using namespace std;
using namespace utils;
using namespace game_utils;

namespace control
{
	CConsoleListener::CConsoleListener()
	{
	}

	CConsoleListener::~CConsoleListener()
	{
	}

	bool CConsoleListener::checkParams(string params, int paramsCount, string &result, vector<string> &tokenizedParams)
	{
		if (params.size()==0)
		{
			result = "Params needed!";
			return false;
		}

		std::vector<string> tParams;
		sStringUtils::tokenizeString(tParams,params," ");

		if ((GLint)tParams.size()<paramsCount || (GLint)tParams.size()>paramsCount)
		{
			result = "Params needed: "+CConversions::intToStr(paramsCount);
			return false;
		}

		for (unsigned int i=0; i<tParams.size(); i++)
		{
			tokenizedParams.push_back(tParams[i]);
		}

		return true;
	}
};