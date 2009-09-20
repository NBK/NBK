#include "RoomDependancy.h"

namespace DK_GUI
{
	CRoomDependancy::CRoomDependancy()
	{
		room_selected=false;
	}

	CRoomDependancy::~CRoomDependancy()
	{
	}

	GLvoid CRoomDependancy::set_selected_room(GLvoid *object)
	{
		room_selected=true;
	}
}