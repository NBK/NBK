#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>

namespace utils
{
	class CLogger
	{
	public:
		CLogger();
		~CLogger();

		static GLvoid	setEntryStart();
		static GLvoid	setEntryEnd(const char *text, ...);
		static GLvoid	addEntry(const char *text, ...);
		static GLvoid	getLog(std::vector<std::string> &log);

	private:

		static GLint indices;

		static GLint	getNextFreeIndex();
		static GLint	getLastIndex();
	};
}

#endif // LOGGER_H