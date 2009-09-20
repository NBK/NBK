#ifndef ROOM_DEPENDANCY_H
#define ROOM_DEPENDANCY_H

#include <windows.h>
#include <gl/gl.h>

namespace DK_GUI
{
	class CRoomDependancy
	{
	public:
		CRoomDependancy();
		~CRoomDependancy();
		
		virtual GLvoid set_selected_room(GLvoid *object);

	protected:
		bool room_selected;
	};
}

#endif //ROOM_DEPENDANCY_H