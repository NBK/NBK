#ifndef CONSOLE_H
#define CONSOLE_H

#include "system.h"
#include <GL/gl.h>
#include "InputListener.h"
#include "ConsoleListener.h"
#include <list>
#include "Primitives.h"

/*
	System for dinamically examining and setting system values.
*/

namespace control
{
	class CConsole: public CInputListener
	{
	public:
		CConsole();
		~CConsole();

		/*
			Connects to the input controller.
		*/
		void init();

		/*
			If active; console receives input from the keyboard and prints requested stuff
		*/
		void update();

		void	setActive(bool active);
		bool	isActive();

		virtual void onKeyDown(int key);
		virtual void onKeyUp(int key);
		virtual void onMouseClicked(int button);

		void	registerClass(control::CConsoleListener *listener, std::string commandGroupName);
		void	addParam(std::string keyword, std::string description);

		/*
			Any call may call this function to print a text to the console.
		*/
		void	writeLine(std::string line);

		/*
			Usefull for writing text to console when running in a tight loop. This way 
			console will get refreshed instantly. Old state gets returned.
		*/
		bool	setForceRedraw(bool forceRedraw);

	private:
		bool		active,
					forceRedraw;

		std::string	currentLine;

		int		startY,
				addY;

		std::list<std::string>				lines;
		std::list<std::string>::iterator	linesIter;		

		void doAction(std::string &execLine);
		void parseString(std::string &line);
		bool cutChar(char chr);

		// console background
		utils::CPrimitives::sQuadDescriptor quadDescriptor;

		/*
			Here we register classes that want to be console-enabled.

			String param is the param that will be sent to onAction(params).
			Param may be constructed from several keywords separated by ','.
		*/
		std::map<std::string, std::pair< std::string, control::CConsoleListener* > >			consoleListeners;
		std::map<std::string, std::pair< std::string, control::CConsoleListener* > >::iterator	clIter;

		control::CConsoleListener	*lastRegisteredListener;
		std::string					lastCommandGroupName;

		std::vector<std::string>	commandHistroy;
		int							commandHistoryPos;

		/*
			This way we can simply print commandy by group.
		*/
		std::map<std::string, std::vector<std::string> >			groups;
		std::map<std::string, std::vector<std::string> >::iterator	gIter;
	};
};

#endif // CONSOLE_H