#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

class CLogger
{
public:

	static void init(std::string fileName);
	static void addEntry(std::string line);

private:

	static std::ofstream	oFile;
	static bool				LOGALL;

};

#endif // LOGGER_H