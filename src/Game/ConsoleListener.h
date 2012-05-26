#ifndef CONSOLE_LISTENER_H
#define CONSOLE_LISTENER_H

#include "system.h"
#include <GL/gl.h>
#include <string>
#include <vector>
#include "Conversions.h"

/*
	Every class that wants to register as a console compliant class
	needs to inherit from this abstract class.
*/

namespace control
{
	class CConsoleListener
	{
	public:
		CConsoleListener();
		~CConsoleListener();

		virtual std::string onAction(std::string keyword, std::string params) = 0;

		static	bool		checkParams(std::string params, int paramsCount, std::string &result, std::vector<std::string> &tokenizedParams);
	};
};

#endif // CONSOLE_LISTENER_H