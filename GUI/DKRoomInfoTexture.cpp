#include "../commons.h"
#include "DKRoomInfoTexture.h"

using namespace game_utils;

namespace DK_GUI
{
	CDKRoomInfoTexture::CDKRoomInfoTexture(CDKTextureList *texture_list)
	:CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0)
	{
		this->texture_list=texture_list;
		blink=true;
		blink_speed=0.008f;
		blink_time_out=1.0f;
		blink_pos=0.0f;
	}

	CDKRoomInfoTexture::~CDKRoomInfoTexture()
	{
	}

	GLvoid CDKRoomInfoTexture::set_selected(const char *class_name, char leading)
	{
		char *info_texture_class_name=strdup(class_name);
		info_texture_class_name[0]=leading;
		info_texture_class_name[1]='E';
		textures[0]=texture_list->get_texture_by_name(info_texture_class_name);
		info_texture_class_name[1]='D';
		textures[1]=texture_list->get_texture_by_name(info_texture_class_name);
		delete info_texture_class_name;
		set_ready(true);

		blink=true;
		set_texture(textures[0]);
	}

	GLvoid CDKRoomInfoTexture::draw()
	{
		CGUIBackground::draw();

		if (blink)
		{
			blink_pos+=blink_speed*CV_GAME_MANAGER->getDeltaTime();
			if (blink_pos>=blink_time_out)
			{
				set_texture(textures[(GLint)(sel_tex=!sel_tex)]);
				blink_pos=0.0f;
			}
		}
	}

	GLvoid CDKRoomInfoTexture::set_blink(bool blink)
	{
		this->blink=blink;
		set_texture(textures[0]);
	}
}