#ifndef ROOM_INFO_TEXTURE_H
#define ROOM_INFO_TEXTURE_H

#include "GUIBackground.h"
#include "DKTextureList.h"

using namespace GUI;

namespace DK_GUI
{
	class CDKRoomInfoTexture: public CGUIBackground
	{
	public:
		CDKRoomInfoTexture(CDKTextureList *texture_list);
		~CDKRoomInfoTexture();

		GLvoid set_selected(const char *class_name, char leading);

		virtual GLvoid draw();

		GLvoid set_blink(bool blink);

	private:
		CDKTextureList *texture_list;
		bool blink,sel_tex;		
		GLuint textures[2];
		GLfloat blink_speed, blink_time_out, blink_pos;
	};
}

#endif // ROOM_INFO_TEXTURE_H