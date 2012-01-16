#include "commons.h"
#include "Console.h"
#include "Text.h"
#include "utils.h"
#include "OGLUtils.h"
#include <cml/cml.h>

#include "SDLUtils.h"

using namespace std;
using namespace utils;
using namespace game_utils;
using namespace cml;
using namespace control;

#define NOACTIVE if (!active)return;

namespace control
{
	CConsole::CConsole()
	{
		active=false;
		forceRedraw=false;

		startY = CV_SETTINGS_WINDOW_HEIGHT/2;
		addY = 12;
		currentLine = ">> ";

		quadDescriptor.blend = false;
		quadDescriptor.color = vector3f(0.0f,0.0f,0.0f);
		quadDescriptor.size = vector2f((float)CV_SETTINGS_WINDOW_WIDTH,(float)startY);

		lastRegisteredListener = NULL;
		lastCommandGroupName = "";

		commandHistoryPos = 0;
	}

	CConsole::~CConsole()
	{
	}

	void CConsole::init()
	{
		// self register as the input event listener
		CV_GAME_MANAGER->getControlManager()->getInput()->registerListener(this);
	}

	void CConsole::update()
	{
		NOACTIVE;

		// erase too old context
		while (lines.size()>90)
		{
			lines.pop_back();
		}

		// print what has been written till now
		CText *textPrinter = CV_GAME_MANAGER->getTextPrinter();

		if (!cutChar('|'))
		{
			currentLine+="|";
		}

		// console background
		CPrimitives::drawQuad(quadDescriptor);

		int lc = 0;

		// the line in progress
		textPrinter->print(0.0f,(float)(startY-addY*lc++),"%s",currentLine.c_str());

		for (linesIter=lines.begin(); linesIter!=lines.end(); linesIter++)
		{
			// draw the other lines
			textPrinter->print(0.0f,(float)(startY-addY*lc++),"%s",(*linesIter).c_str());
		}
	}

	void CConsole::setActive(bool active)
	{
		this->active=active;
	}

	bool CConsole::isActive()
	{
		return active;
	}

	bool CConsole::setForceRedraw(bool forceRedraw)
	{
		bool oldState = this->forceRedraw;
		this->forceRedraw=forceRedraw;
		return oldState;
	}

	void CConsole::doAction(std::string &execLine)
	{
		std::vector<string> tInput;
		sStringUtils::tokenizeString(tInput,execLine," ");

		// extract the keyword name, index>0 belong to params for this keyword
		execLine=tInput[0];

		if (execLine=="CLS")
		{
			lines.clear();
		}
		else if (execLine=="CLH")
		{
			commandHistroy.clear();
		}
		else if (execLine=="HELP" || execLine=="H")
		{
			lines.push_front(" ");
			lines.push_front("> List of commands:");
			lines.push_front(">>> CONSOLE MANAGER");
			lines.push_front(">>>     CLS_______() clears the console window.");
			lines.push_front(">>>     CLH_______() clears the history.");

			/*for (clIter=consoleListeners.begin(); clIter!=consoleListeners.end(); clIter++)
			{
				int spaces = 10-clIter->first.length();
				string sspaces = "";
				while (spaces-->0)
				{
					sspaces+="_";
				}
				lines.push_front(">>> "+clIter->first+sspaces+clIter->second.first);
			}*/

			for (gIter = groups.begin(); gIter != groups.end(); gIter++)
			{
				lines.push_front(">>> "+gIter->first);

				for (std::vector<string>::iterator sIter = gIter->second.begin(); sIter != gIter->second.end(); sIter++)
				{
					pair<string, CConsoleListener*> p = consoleListeners[(*sIter)];

					string keyword = *sIter;
					string description = p.first;

					int spaces = 10-keyword.length();
					string sspaces = "";
					while (spaces-->0)
					{
						sspaces+="_";
					}
					lines.push_front(">>>     "+keyword+sspaces+description);
				}
			}
		}
		else if (consoleListeners.find(execLine)!=consoleListeners.end())
		{
			clIter = consoleListeners.find(execLine);

			string params = "";
			for (unsigned int i=1; i<tInput.size(); i++)
			{
				params+=tInput[i]+" ";
			}

			lines.push_front(string(">>> ")+clIter->second.second->onAction(clIter->first,params));
		}
		else
		{
			lines.push_front("ERROR: Unknown command!");
		}
	}

	void CConsole::parseString(std::string &line)
	{
		if (line.length()>3)
		{
			commandHistoryPos=0;
			line = line.substr(3,line.length()-3);
			commandHistroy.push_back(line);
			lines.push_front("> "+line);
			doAction(line);
			line = ">> ";
		}
	}

	bool CConsole::cutChar(char chr)
	{
		if(currentLine.length() == 0) return false;
		if (currentLine[currentLine.length()-1]==chr)
		{
			currentLine=currentLine.substr(0,currentLine.length()-1);
			return true;
		}
		return false;
	}

	// keyboard handling from CInputListener

	void CConsole::onKeyDown(int key)
	{

//printf("Key code: %d\n",key);
#ifdef WIN32
		if (key==192/*tilda*/)
#else
		if (key==SDLK_WORLD_18)
#endif
		{
			active=!active;
		}

		NOACTIVE;

		bool validKey = (key>=(int)'0' && key<=(int)'9') ||
						(key>=(int)'a' && key<=(int)'z') ||
						(key>=(int)'A' && key<=(int)'Z') ||
						key==(int)' ';

		char pressedChar = (char)key;

		if (key==188/*,*/)
		{
			validKey=true;
			pressedChar=',';
		}

		if (key==190/*.*/)
		{
			validKey=true;
			pressedChar='.';
		}

		if (key==VK_RETURN)
		{
			// handle the written string
			cutChar('|');
			parseString(currentLine);
		}
		else if (key==VK_BACK)
		{
			cutChar('|');
			if (currentLine.length()>3)
			{
				currentLine=currentLine.substr(0,currentLine.length()-1);
			}
		}
		else if (key==VK_PRIOR)
		{
			// page-up, display previous command
			if (commandHistroy.size()==0)
			{
				return;
			}

			currentLine = ">> "+commandHistroy[commandHistroy.size()-1-commandHistoryPos++];

			if (commandHistoryPos>=(int)commandHistroy.size())
			{
				commandHistoryPos=0;
			}
		}
		else if (validKey)
		{
			cutChar('|');
			currentLine+=pressedChar;
		}
	}

	void CConsole::onKeyUp(int key)
	{
		NOACTIVE;
	}

	void CConsole::onMouseClicked(int button)
	{
	}

	void CConsole::registerClass(CConsoleListener *listener, string commandGroupName)
	{
		if (groups.find(commandGroupName)==groups.end())
		{
			lastRegisteredListener=listener;
			groups[commandGroupName] = std::vector<string>();
			lastCommandGroupName=commandGroupName;
		}
		else
		{
			// already registered, no need to re-register
			lastRegisteredListener = NULL;
		}
	}

	void CConsole::addParam(string keyword, string description)
	{
		if (lastRegisteredListener!=NULL)
		{
			consoleListeners[keyword]= pair<string, CConsoleListener*>(description,lastRegisteredListener);
			groups[lastCommandGroupName].push_back(keyword);
		}
	}

	void CConsole::writeLine(std::string line)
	{
		lines.push_front("> "+line);

		if (forceRedraw)
		{
			currentLine = "";
			COGLUtils::clear();
			update();
			glFlush();
			COGLUtils::swapDC(CV_DEVICE_CONTEXT);
		}
	}
};