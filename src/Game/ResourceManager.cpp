#include "commons.h"
#include "ResourceManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "LUA/LUAEffectLoader.h"

using namespace std;
using namespace lists;
using namespace loaders;
using namespace LUA_effects;

#define RLEFF "RLEFF"

namespace game_utils
{
	namespace managers
	{
		CResourceManager::CResourceManager(): CManager()
		{
		}

		CResourceManager::~CResourceManager()
		{
			for (mpIter = modelPool.begin(); mpIter != modelPool.end(); mpIter++)
			{
				delete mpIter->second;
			}

			for (effIter = effectPool.begin(); effIter != effectPool.end(); effIter++)
			{
				delete effIter->second;
			}
		}

		bool CResourceManager::init()
		{
			CV_GAME_MANAGER->getConsole()->registerClass(this,"RESOURCE MANAGER");
			CV_GAME_MANAGER->getConsole()->addParam(RLEFF,"Reloads all the effects.");

			GLOBAL_TEXTURE_LIST = new CDKTextureList(CV_WINDOW_HANDLE);

			GLOBAL_TEXTURE_LIST->add_texture("MINIMAP_TEXTURE","data/resources/GFX/mini_draw/claimed_and_room2.gif",true,GL_NEAREST);
			GLOBAL_TEXTURE_LIST->add_texture("FSM_BACKGROUND_TEXTURE","data/resources/GFX/GUI/fsm_bg.gif");
			GLOBAL_TEXTURE_LIST->add_texture("ROOM_EFFECT","data/resources/GFX/BB.gif");
			GLOBAL_TEXTURE_LIST->add_texture("LEVELS","data/resources/GFX/levels.gif");
			GLOBAL_TEXTURE_LIST->add_texture("MENU_BUTTON","data/resources/GFX/menu_button.gif",true);
			GLOBAL_TEXTURE_LIST->add_texture("MENU_BUTTON_EFFECT","data/resources/GFX/menu_button_effect.gif",true);
			GLOBAL_TEXTURE_LIST->add_texture("MAIN_MENU_BACKGROUND","data/resources/GFX/main_menu.gif");

			GLOBAL_TEXTURE_LIST->add_texture("ITEM_BUTTON_TEXTURE_SELL","data/resources/GFX/GUI/pic106.bmp");
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_NORMAL","data/resources/GFX/pointers/pic0.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_DIG","data/resources/GFX/pointers/pic1.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_SELL","data/resources/GFX/pointers/pic2.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_QUERY","data/resources/GFX/pointers/pic3.bmp",true);

			/* the room build mouse pointer textures */
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_TREASURE","data/resources/GFX/pointers/pic24.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_LAIR","data/resources/GFX/pointers/pic35.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_HATCHERY","data/resources/GFX/pointers/pic25.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_TRAIN","data/resources/GFX/pointers/pic29.bmp",true);

			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_LIBRARY","data/resources/GFX/pointers/pic26.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_BRIDGE","data/resources/GFX/pointers/pic36.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_GUARD","data/resources/GFX/pointers/pic37.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_WORKSHOP","data/resources/GFX/pointers/pic33.bmp",true);

			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_PRISON","data/resources/GFX/pointers/pic28.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_TORTURE","data/resources/GFX/pointers/pic27.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_BARRACKS","data/resources/GFX/pointers/pic30.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_TEMPLE","data/resources/GFX/pointers/pic32.bmp",true);

			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_ROOM_GRAVEYARD","data/resources/GFX/pointers/pic31.bmp",true);

			/* the door build mouse pointer textures */
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_DOORS_WOODEN","data/resources/GFX/pointers/pic10.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_DOORS_BRACED","data/resources/GFX/pointers/pic11.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_DOORS_IRON","data/resources/GFX/pointers/pic12.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_DOORS_MAGIC","data/resources/GFX/pointers/pic13.bmp",true);

			/* the trap build mouse pointer textures */
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_TRAP_BOULDER","data/resources/GFX/pointers/pic4.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_TRAP_ALARM","data/resources/GFX/pointers/pic8.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_TRAP_POISON","data/resources/GFX/pointers/pic6.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_TRAP_LIGHTNING","data/resources/GFX/pointers/pic7.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_TRAP_LAVA","data/resources/GFX/pointers/pic9.bmp",true);
			GLOBAL_TEXTURE_LIST->add_texture("POINTER_BUILD_TRAP_WORD_OF_POWER","data/resources/GFX/pointers/pic5.bmp",true);

			/* item button textures for building doors, rooms, traps */
			GLOBAL_TEXTURE_LIST->add_texture("BS_UNAVAILABLE","data/resources/GFX/GUI/pic23.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("BS_RESEARCH","data/resources/GFX/GUI/pic24.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("BS_AVAILABLE_NOT_USED","data/resources/GFX/GUI/pic23.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("BS_AVAILABLE_USED","data/resources/GFX/GUI/pic26.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("BS_AVAILABLE_OVER","data/resources/GFX/GUI/pic27.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture("INFO_PANEL","data/resources/GFX/GUI/pic25.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_SIMPLE_PANEL","data/resources/GFX/GUI/pic458.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_QUERY_UP","data/resources/GFX/GUI/pic475.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_QUERY_DOWN","data/resources/GFX/GUI/pic474.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_CHICKEN_DOWN","data/resources/GFX/GUI/pic345.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_CHICKEN_UP","data/resources/GFX/GUI/pic346.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_CHICKEN_ANIM1","data/resources/GFX/GUI/pic347.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_CHICKEN_ANIM2","data/resources/GFX/GUI/pic348.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_ROOM_COUNT_PLAYER0","data/resources/GFX/GUI/pic323.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_ROOM_COUNT_PLAYER1","data/resources/GFX/GUI/pic325.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_ROOM_COUNT_PLAYER2","data/resources/GFX/GUI/pic327.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_ROOM_COUNT_PLAYER3","data/resources/GFX/GUI/pic329.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_CREATURE_COUNT_PLAYER0","data/resources/GFX/GUI/pic322.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_CREATURE_COUNT_PLAYER1","data/resources/GFX/GUI/pic324.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_CREATURE_COUNT_PLAYER2","data/resources/GFX/GUI/pic326.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_CREATURE_COUNT_PLAYER3","data/resources/GFX/GUI/pic328.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_PRISON_DOWN","data/resources/GFX/GUI/pic349.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_PRISON_UP","data/resources/GFX/GUI/pic350.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_PRISON_ANIM1","data/resources/GFX/GUI/pic351.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_PRISON_ANIM2","data/resources/GFX/GUI/pic352.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_PAYDAY_PROGRESS_BAR_PANEL","data/resources/GFX/GUI/pic340.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_RESEARCH_PICTURE","data/resources/GFX/GUI/rooms/pic60.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_RESEARCH_PROGRESS_BAR_TEXTURE","data/resources/GFX/GUI/pic460.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_WORKSHOP_PICTURE","data/resources/GFX/GUI/rooms/pic74.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("QUERY_TAB_WORKSHOP_PROGRESS_BAR_TEXTURE","data/resources/GFX/GUI/pic461.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture("DOOR0","data/resources/GFX/GUI/doors/pic143.bmp",true,GL_NEAREST);
			GLOBAL_TEXTURE_LIST->add_texture("DOOR1","data/resources/GFX/GUI/doors/pic145.bmp",true,GL_NEAREST);
			GLOBAL_TEXTURE_LIST->add_texture("DOOR2","data/resources/GFX/GUI/doors/pic147.bmp",true,GL_NEAREST);
			GLOBAL_TEXTURE_LIST->add_texture("DOOR3","data/resources/GFX/GUI/doors/pic149.bmp",true,GL_NEAREST);

			GLOBAL_TEXTURE_LIST->add_texture("GP00","data/resources/GFX/GUI/pic235.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("GP01","data/resources/GFX/GUI/pic1.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("GP10","data/resources/GFX/GUI/pic2.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("GP11","data/resources/GFX/GUI/pic3.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("GP20","data/resources/GFX/GUI/pic4.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("GP21","data/resources/GFX/GUI/pic5.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("GP30","data/resources/GFX/GUI/pic20.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("GP31","data/resources/GFX/GUI/pic21.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP0E","data/resources/GFX/GUI/pic6.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP0D","data/resources/GFX/GUI/pic7.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP1E","data/resources/GFX/GUI/pic8.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP1D","data/resources/GFX/GUI/pic9.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP2E","data/resources/GFX/GUI/pic10.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP2D","data/resources/GFX/GUI/pic11.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP3E","data/resources/GFX/GUI/pic12.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP3D","data/resources/GFX/GUI/pic13.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP4E","data/resources/GFX/GUI/pic14.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture("TCP4D","data/resources/GFX/GUI/pic15.bmp",true,GL_NEAREST,false);

			/* the room textures for room buttons */
			GLOBAL_TEXTURE_LIST->add_texture("ROOM00_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic56.bmp",true,GL_NEAREST,false); // treasure
			GLOBAL_TEXTURE_LIST->add_texture("ROOM01_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic78.bmp",true,GL_NEAREST,false); // lair
			GLOBAL_TEXTURE_LIST->add_texture("ROOM02_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic58.bmp",true,GL_NEAREST,false); // hatchery
			GLOBAL_TEXTURE_LIST->add_texture("ROOM03_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic66.bmp",true,GL_NEAREST,false); // train

			GLOBAL_TEXTURE_LIST->add_texture("ROOM04_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic60.bmp",true,GL_NEAREST,false); // library
			GLOBAL_TEXTURE_LIST->add_texture("ROOM05_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic80.bmp",true,GL_NEAREST,false); // bridge
			GLOBAL_TEXTURE_LIST->add_texture("ROOM06_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic82.bmp",true,GL_NEAREST,false); // guard
			GLOBAL_TEXTURE_LIST->add_texture("ROOM07_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic74.bmp",true,GL_NEAREST,false); // workshop

			GLOBAL_TEXTURE_LIST->add_texture("ROOM08_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic64.bmp",true,GL_NEAREST,false); // prison
			GLOBAL_TEXTURE_LIST->add_texture("ROOM09_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic62.bmp",true,GL_NEAREST,false); // torture
			GLOBAL_TEXTURE_LIST->add_texture("ROOM10_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic68.bmp",true,GL_NEAREST,false); // barracks
			GLOBAL_TEXTURE_LIST->add_texture("ROOM11_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic72.bmp",true,GL_NEAREST,false); // temple

			GLOBAL_TEXTURE_LIST->add_texture("ROOM12_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/rooms/pic70.bmp",true,GL_NEAREST,false); // graveyard
			GLOBAL_TEXTURE_LIST->add_texture("ROOM13_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/pic15.bmp",true,GL_NEAREST,false); // scavenger (not supported!!)
			GLOBAL_TEXTURE_LIST->add_texture("ROOM14_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/pic15.bmp",true,GL_NEAREST,false); // empty
			GLOBAL_TEXTURE_LIST->add_texture("ROOM15_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/pic106.bmp",true,GL_NEAREST,false); // sell

			/* the textures for room info ENABLED*/
			GLOBAL_TEXTURE_LIST->add_texture("IE00","data/resources/GFX/GUI/rooms/pic28.bmp",true,GL_NEAREST,false); // treasure
			GLOBAL_TEXTURE_LIST->add_texture("IE01","data/resources/GFX/GUI/rooms/pic50.bmp",true,GL_NEAREST,false); // lair
			GLOBAL_TEXTURE_LIST->add_texture("IE02","data/resources/GFX/GUI/rooms/pic30.bmp",true,GL_NEAREST,false); // hatchery
			GLOBAL_TEXTURE_LIST->add_texture("IE03","data/resources/GFX/GUI/rooms/pic38.bmp",true,GL_NEAREST,false); // train

			GLOBAL_TEXTURE_LIST->add_texture("IE04","data/resources/GFX/GUI/rooms/pic32.bmp",true,GL_NEAREST,false); // library
			GLOBAL_TEXTURE_LIST->add_texture("IE05","data/resources/GFX/GUI/rooms/pic52.bmp",true,GL_NEAREST,false); // bridge
			GLOBAL_TEXTURE_LIST->add_texture("IE06","data/resources/GFX/GUI/rooms/pic54.bmp",true,GL_NEAREST,false); // guard
			GLOBAL_TEXTURE_LIST->add_texture("IE07","data/resources/GFX/GUI/rooms/pic46.bmp",true,GL_NEAREST,false); // workshop

			GLOBAL_TEXTURE_LIST->add_texture("IE08","data/resources/GFX/GUI/rooms/pic36.bmp",true,GL_NEAREST,false); // prison
			GLOBAL_TEXTURE_LIST->add_texture("IE09","data/resources/GFX/GUI/rooms/pic34.bmp",true,GL_NEAREST,false); // torture
			GLOBAL_TEXTURE_LIST->add_texture("IE10","data/resources/GFX/GUI/rooms/pic40.bmp",true,GL_NEAREST,false); // barracks
			GLOBAL_TEXTURE_LIST->add_texture("IE11","data/resources/GFX/GUI/rooms/pic44.bmp",true,GL_NEAREST,false); // temple

			GLOBAL_TEXTURE_LIST->add_texture("IE12","data/resources/GFX/GUI/rooms/pic42.bmp",true,GL_NEAREST,false); // graveyard
			GLOBAL_TEXTURE_LIST->add_texture("IE13","data/resources/GFX/GUI/rooms/pic48.bmp",true,GL_NEAREST,false); // scavenger (not supported!!)
			GLOBAL_TEXTURE_LIST->add_texture("IE14","data/resources/GFX/GUI/pic15.bmp",true,GL_NEAREST,false); // empty
			GLOBAL_TEXTURE_LIST->add_texture("IE15","data/resources/GFX/GUI/pic106.bmp",true,GL_NEAREST,false); // sell

			/* the textures for room info DISABLED */
			GLOBAL_TEXTURE_LIST->add_texture("ID00","data/resources/GFX/GUI/rooms/pic29.bmp",true,GL_NEAREST,false); // treasure
			GLOBAL_TEXTURE_LIST->add_texture("ID01","data/resources/GFX/GUI/rooms/pic51.bmp",true,GL_NEAREST,false); // lair
			GLOBAL_TEXTURE_LIST->add_texture("ID02","data/resources/GFX/GUI/rooms/pic31.bmp",true,GL_NEAREST,false); // hatchery
			GLOBAL_TEXTURE_LIST->add_texture("ID03","data/resources/GFX/GUI/rooms/pic39.bmp",true,GL_NEAREST,false); // train

			GLOBAL_TEXTURE_LIST->add_texture("ID04","data/resources/GFX/GUI/rooms/pic33.bmp",true,GL_NEAREST,false); // library
			GLOBAL_TEXTURE_LIST->add_texture("ID05","data/resources/GFX/GUI/rooms/pic53.bmp",true,GL_NEAREST,false); // bridge
			GLOBAL_TEXTURE_LIST->add_texture("ID06","data/resources/GFX/GUI/rooms/pic55.bmp",true,GL_NEAREST,false); // guard
			GLOBAL_TEXTURE_LIST->add_texture("ID07","data/resources/GFX/GUI/rooms/pic47.bmp",true,GL_NEAREST,false); // workshop

			GLOBAL_TEXTURE_LIST->add_texture("ID08","data/resources/GFX/GUI/rooms/pic37.bmp",true,GL_NEAREST,false); // prison
			GLOBAL_TEXTURE_LIST->add_texture("ID09","data/resources/GFX/GUI/rooms/pic35.bmp",true,GL_NEAREST,false); // torture
			GLOBAL_TEXTURE_LIST->add_texture("ID10","data/resources/GFX/GUI/rooms/pic41.bmp",true,GL_NEAREST,false); // barracks
			GLOBAL_TEXTURE_LIST->add_texture("ID11","data/resources/GFX/GUI/rooms/pic45.bmp",true,GL_NEAREST,false); // temple

			GLOBAL_TEXTURE_LIST->add_texture("ID12","data/resources/GFX/GUI/rooms/pic43.bmp",true,GL_NEAREST,false); // graveyard
			GLOBAL_TEXTURE_LIST->add_texture("ID13","data/resources/GFX/GUI/rooms/pic49.bmp",true,GL_NEAREST,false); // scavenger (not supported!!)
			GLOBAL_TEXTURE_LIST->add_texture("ID14","data/resources/GFX/GUI/pic15.bmp",true,GL_NEAREST,false); // empty
			GLOBAL_TEXTURE_LIST->add_texture("ID15","data/resources/GFX/GUI/pic106.bmp",true,GL_NEAREST,false); // sell

			/* the gfx for room data displayer */
			GLOBAL_TEXTURE_LIST->add_texture("BUILD_TAB_ROOM_DATA_PROGRESS_BAR_TEXTURE","data/resources/GFX/GUI/pic22.bmp",true,GL_NEAREST,false);

			/* the trap textures for trap buttons */
			GLOBAL_TEXTURE_LIST->add_texture("TRAP0_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/traps/pic151.bmp",true,GL_NEAREST,false); // boulder
			GLOBAL_TEXTURE_LIST->add_texture("TRAP1_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/traps/pic153.bmp",true,GL_NEAREST,false); // alarm
			GLOBAL_TEXTURE_LIST->add_texture("TRAP2_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/traps/pic155.bmp",true,GL_NEAREST,false); // gas
			GLOBAL_TEXTURE_LIST->add_texture("TRAP3_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/traps/pic157.bmp",true,GL_NEAREST,false); // lightning
			GLOBAL_TEXTURE_LIST->add_texture("TRAP4_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/traps/pic159.bmp",true,GL_NEAREST,false); // word of power
			GLOBAL_TEXTURE_LIST->add_texture("TRAP5_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/traps/pic161.bmp",true,GL_NEAREST,false); // lava

			/* the textures for trap info ENABLED*/
			GLOBAL_TEXTURE_LIST->add_texture("TE00","data/resources/GFX/GUI/traps/pic129.bmp",true,GL_NEAREST,false); // boulder
			GLOBAL_TEXTURE_LIST->add_texture("TE01","data/resources/GFX/GUI/traps/pic131.bmp",true,GL_NEAREST,false); // alarm
			GLOBAL_TEXTURE_LIST->add_texture("TE02","data/resources/GFX/GUI/traps/pic133.bmp",true,GL_NEAREST,false); // poison
			GLOBAL_TEXTURE_LIST->add_texture("TE03","data/resources/GFX/GUI/traps/pic135.bmp",true,GL_NEAREST,false); // lightning
			GLOBAL_TEXTURE_LIST->add_texture("TE04","data/resources/GFX/GUI/traps/pic137.bmp",true,GL_NEAREST,false); // word of power
			GLOBAL_TEXTURE_LIST->add_texture("TE05","data/resources/GFX/GUI/traps/pic139.bmp",true,GL_NEAREST,false); // lava

			/* the textures for trap info DISABLED*/
			GLOBAL_TEXTURE_LIST->add_texture("TD00","data/resources/GFX/GUI/traps/pic130.bmp",true,GL_NEAREST,false); // boulder
			GLOBAL_TEXTURE_LIST->add_texture("TD01","data/resources/GFX/GUI/traps/pic132.bmp",true,GL_NEAREST,false); // alarm
			GLOBAL_TEXTURE_LIST->add_texture("TD02","data/resources/GFX/GUI/traps/pic134.bmp",true,GL_NEAREST,false); // poison
			GLOBAL_TEXTURE_LIST->add_texture("TD03","data/resources/GFX/GUI/traps/pic136.bmp",true,GL_NEAREST,false); // lightning
			GLOBAL_TEXTURE_LIST->add_texture("TD04","data/resources/GFX/GUI/traps/pic138.bmp",true,GL_NEAREST,false); // word of power
			GLOBAL_TEXTURE_LIST->add_texture("TD05","data/resources/GFX/GUI/traps/pic140.bmp",true,GL_NEAREST,false); // lava

			/* the door textures for door buttons */
			GLOBAL_TEXTURE_LIST->add_texture("DOOR0_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/doors/pic165.bmp",true,GL_NEAREST,false); // wooden
			GLOBAL_TEXTURE_LIST->add_texture("DOOR1_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/doors/pic167.bmp",true,GL_NEAREST,false); // braced
			GLOBAL_TEXTURE_LIST->add_texture("DOOR2_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/doors/pic169.bmp",true,GL_NEAREST,false); // iron
			GLOBAL_TEXTURE_LIST->add_texture("DOOR3_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/doors/pic171.bmp",true,GL_NEAREST,false); // magic

			/* the textures for door info ENABLED*/
			GLOBAL_TEXTURE_LIST->add_texture("DE00","data/resources/GFX/GUI/doors/pic143.bmp",true,GL_NEAREST,false); // wooden
			GLOBAL_TEXTURE_LIST->add_texture("DE01","data/resources/GFX/GUI/doors/pic145.bmp",true,GL_NEAREST,false); // braced
			GLOBAL_TEXTURE_LIST->add_texture("DE02","data/resources/GFX/GUI/doors/pic147.bmp",true,GL_NEAREST,false); // iron
			GLOBAL_TEXTURE_LIST->add_texture("DE03","data/resources/GFX/GUI/doors/pic149.bmp",true,GL_NEAREST,false); // magic

			/* the textures for door info DISABLED*/
			GLOBAL_TEXTURE_LIST->add_texture("DD00","data/resources/GFX/GUI/doors/pic144.bmp",true,GL_NEAREST,false); // wooden
			GLOBAL_TEXTURE_LIST->add_texture("DD01","data/resources/GFX/GUI/doors/pic146.bmp",true,GL_NEAREST,false); // braced
			GLOBAL_TEXTURE_LIST->add_texture("DD02","data/resources/GFX/GUI/doors/pic148.bmp",true,GL_NEAREST,false); // iron
			GLOBAL_TEXTURE_LIST->add_texture("DD03","data/resources/GFX/GUI/doors/pic150.bmp",true,GL_NEAREST,false); // magic

			/* the room textures for spell buttons */
			GLOBAL_TEXTURE_LIST->add_texture("SPELL00_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic113.bmp",true,GL_NEAREST,false); // possess
			GLOBAL_TEXTURE_LIST->add_texture("SPELL01_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic117.bmp",true,GL_NEAREST,false); // create imp
			GLOBAL_TEXTURE_LIST->add_texture("SPELL02_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic107.bmp",true,GL_NEAREST,false); // sight of evil
			GLOBAL_TEXTURE_LIST->add_texture("SPELL03_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic121.bmp",true,GL_NEAREST,false); // speed monster

			GLOBAL_TEXTURE_LIST->add_texture("SPELL04_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic451.bmp",true,GL_NEAREST,false); // must obey
			GLOBAL_TEXTURE_LIST->add_texture("SPELL05_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic115.bmp",true,GL_NEAREST,false); // call to arms
			GLOBAL_TEXTURE_LIST->add_texture("SPELL06_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic127.bmp",true,GL_NEAREST,false); // conceal monster
			GLOBAL_TEXTURE_LIST->add_texture("SPELL07_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic111.bmp",true,GL_NEAREST,false); // hold audience

			GLOBAL_TEXTURE_LIST->add_texture("SPELL08_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic119.bmp",true,GL_NEAREST,false); // cave in
			GLOBAL_TEXTURE_LIST->add_texture("SPELL09_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic109.bmp",true,GL_NEAREST,false); // heal
			GLOBAL_TEXTURE_LIST->add_texture("SPELL10_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic123.bmp",true,GL_NEAREST,false); // lightning
			GLOBAL_TEXTURE_LIST->add_texture("SPELL11_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic125.bmp",true,GL_NEAREST,false); // protect creature

			GLOBAL_TEXTURE_LIST->add_texture("SPELL12_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic313.bmp",true,GL_NEAREST,false); // chicken
			GLOBAL_TEXTURE_LIST->add_texture("SPELL13_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic318.bmp",true,GL_NEAREST,false); // disease
			GLOBAL_TEXTURE_LIST->add_texture("SPELL14_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic320.bmp",true,GL_NEAREST,false); // armageddon
			GLOBAL_TEXTURE_LIST->add_texture("SPELL15_BUTTON_TOP_TEXTURE","data/resources/GFX/GUI/spells/pic316.bmp",true,GL_NEAREST,false); // destroy wall

			/* the textures for spell info ENABLED*/
			GLOBAL_TEXTURE_LIST->add_texture("SE00","data/resources/GFX/GUI/spells/pic90.bmp",true,GL_NEAREST,false); // possess
			GLOBAL_TEXTURE_LIST->add_texture("SE01","data/resources/GFX/GUI/spells/pic94.bmp",true,GL_NEAREST,false); // create imp
			GLOBAL_TEXTURE_LIST->add_texture("SE02","data/resources/GFX/GUI/spells/pic84.bmp",true,GL_NEAREST,false); // sight of evil
			GLOBAL_TEXTURE_LIST->add_texture("SE03","data/resources/GFX/GUI/spells/pic98.bmp",true,GL_NEAREST,false); // speed monster

			GLOBAL_TEXTURE_LIST->add_texture("SE04","data/resources/GFX/GUI/spells/pic393.bmp",true,GL_NEAREST,false); // must obey
			GLOBAL_TEXTURE_LIST->add_texture("SE05","data/resources/GFX/GUI/spells/pic92.bmp",true,GL_NEAREST,false); // call to arms
			GLOBAL_TEXTURE_LIST->add_texture("SE06","data/resources/GFX/GUI/spells/pic104.bmp",true,GL_NEAREST,false); // conceal monster
			GLOBAL_TEXTURE_LIST->add_texture("SE07","data/resources/GFX/GUI/spells/pic88.bmp",true,GL_NEAREST,false); // hold audience

			GLOBAL_TEXTURE_LIST->add_texture("SE08","data/resources/GFX/GUI/spells/pic96.bmp",true,GL_NEAREST,false); // cave in
			GLOBAL_TEXTURE_LIST->add_texture("SE09","data/resources/GFX/GUI/spells/pic86.bmp",true,GL_NEAREST,false); // heal
			GLOBAL_TEXTURE_LIST->add_texture("SE10","data/resources/GFX/GUI/spells/pic100.bmp",true,GL_NEAREST,false); // lightning
			GLOBAL_TEXTURE_LIST->add_texture("SE11","data/resources/GFX/GUI/spells/pic102.bmp",true,GL_NEAREST,false); // protect creature

			GLOBAL_TEXTURE_LIST->add_texture("SE12","data/resources/GFX/GUI/spells/pic305.bmp",true,GL_NEAREST,false); // chicken
			GLOBAL_TEXTURE_LIST->add_texture("SE13","data/resources/GFX/GUI/spells/pic309.bmp",true,GL_NEAREST,false); // disease
			GLOBAL_TEXTURE_LIST->add_texture("SE14","data/resources/GFX/GUI/spells/pic311.bmp",true,GL_NEAREST,false); // armageddon
			GLOBAL_TEXTURE_LIST->add_texture("SE15","data/resources/GFX/GUI/spells/pic307.bmp",true,GL_NEAREST,false); // destroy wall

			/* the textures for spell info DISABLED */
			GLOBAL_TEXTURE_LIST->add_texture("SD00","data/resources/GFX/GUI/spells/pic91.bmp",true,GL_NEAREST,false); // possess
			GLOBAL_TEXTURE_LIST->add_texture("SD01","data/resources/GFX/GUI/spells/pic95.bmp",true,GL_NEAREST,false); // create imp
			GLOBAL_TEXTURE_LIST->add_texture("SD02","data/resources/GFX/GUI/spells/pic85.bmp",true,GL_NEAREST,false); // sight of evil
			GLOBAL_TEXTURE_LIST->add_texture("SD03","data/resources/GFX/GUI/spells/pic99.bmp",true,GL_NEAREST,false); // speed monster

			GLOBAL_TEXTURE_LIST->add_texture("SD04","data/resources/GFX/GUI/spells/pic394.bmp",true,GL_NEAREST,false); // must obey
			GLOBAL_TEXTURE_LIST->add_texture("SD05","data/resources/GFX/GUI/spells/pic93.bmp",true,GL_NEAREST,false); // call to arms
			GLOBAL_TEXTURE_LIST->add_texture("SD06","data/resources/GFX/GUI/spells/pic105.bmp",true,GL_NEAREST,false); // conceal monster
			GLOBAL_TEXTURE_LIST->add_texture("SD07","data/resources/GFX/GUI/spells/pic89.bmp",true,GL_NEAREST,false); // hold audience

			GLOBAL_TEXTURE_LIST->add_texture("SD08","data/resources/GFX/GUI/spells/pic97.bmp",true,GL_NEAREST,false); // cave in
			GLOBAL_TEXTURE_LIST->add_texture("SD09","data/resources/GFX/GUI/spells/pic87.bmp",true,GL_NEAREST,false); // heal
			GLOBAL_TEXTURE_LIST->add_texture("SD10","data/resources/GFX/GUI/spells/pic101.bmp",true,GL_NEAREST,false); // lightning
			GLOBAL_TEXTURE_LIST->add_texture("SD11","data/resources/GFX/GUI/spells/pic103.bmp",true,GL_NEAREST,false); // protect creature

			GLOBAL_TEXTURE_LIST->add_texture("SD12","data/resources/GFX/GUI/spells/pic306.bmp",true,GL_NEAREST,false); // chicken
			GLOBAL_TEXTURE_LIST->add_texture("SD13","data/resources/GFX/GUI/spells/pic310.bmp",true,GL_NEAREST,false); // disease
			GLOBAL_TEXTURE_LIST->add_texture("SD14","data/resources/GFX/GUI/spells/pic312.bmp",true,GL_NEAREST,false); // armageddon
			GLOBAL_TEXTURE_LIST->add_texture("SD15","data/resources/GFX/GUI/spells/pic308.bmp",true,GL_NEAREST,false); // destroy wall

			/* spell cursor animations */
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_POSSESS_MONSTER0,"data/resources/GFX/pointers/pic95.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_POSSESS_MONSTER1,"data/resources/GFX/pointers/pic96.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_POSSESS_MONSTER2,"data/resources/GFX/pointers/pic97.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_POSSESS_MONSTER3,"data/resources/GFX/pointers/pic98.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_POSSESS_MONSTER4,"data/resources/GFX/pointers/pic99.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_POSSESS_MONSTER5,"data/resources/GFX/pointers/pic100.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_POSSESS_MONSTER6,"data/resources/GFX/pointers/pic101.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_POSSESS_MONSTER7,"data/resources/GFX/pointers/pic102.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CALL_TO_ARMS0,"data/resources/GFX/pointers/pic39.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CALL_TO_ARMS1,"data/resources/GFX/pointers/pic40.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CALL_TO_ARMS2,"data/resources/GFX/pointers/pic41.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CALL_TO_ARMS3,"data/resources/GFX/pointers/pic42.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CALL_TO_ARMS4,"data/resources/GFX/pointers/pic43.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CALL_TO_ARMS5,"data/resources/GFX/pointers/pic44.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CALL_TO_ARMS6,"data/resources/GFX/pointers/pic45.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CALL_TO_ARMS7,"data/resources/GFX/pointers/pic46.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CONCEAL_MONSTER0,"data/resources/GFX/pointers/pic47.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CONCEAL_MONSTER1,"data/resources/GFX/pointers/pic48.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CONCEAL_MONSTER2,"data/resources/GFX/pointers/pic49.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CONCEAL_MONSTER3,"data/resources/GFX/pointers/pic50.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CONCEAL_MONSTER4,"data/resources/GFX/pointers/pic51.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CONCEAL_MONSTER5,"data/resources/GFX/pointers/pic52.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CONCEAL_MONSTER6,"data/resources/GFX/pointers/pic53.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CONCEAL_MONSTER7,"data/resources/GFX/pointers/pic54.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CHICKEN0,"data/resources/GFX/pointers/pic55.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CHICKEN1,"data/resources/GFX/pointers/pic56.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CHICKEN2,"data/resources/GFX/pointers/pic57.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CHICKEN3,"data/resources/GFX/pointers/pic58.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CHICKEN4,"data/resources/GFX/pointers/pic59.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CHICKEN5,"data/resources/GFX/pointers/pic60.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CHICKEN6,"data/resources/GFX/pointers/pic61.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CHICKEN7,"data/resources/GFX/pointers/pic62.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DISEASE0,"data/resources/GFX/pointers/pic63.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DISEASE1,"data/resources/GFX/pointers/pic64.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DISEASE2,"data/resources/GFX/pointers/pic65.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DISEASE3,"data/resources/GFX/pointers/pic66.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DISEASE4,"data/resources/GFX/pointers/pic67.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DISEASE5,"data/resources/GFX/pointers/pic68.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DISEASE6,"data/resources/GFX/pointers/pic69.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DISEASE7,"data/resources/GFX/pointers/pic70.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DESTROY_WALL0,"data/resources/GFX/pointers/pic71.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DESTROY_WALL1,"data/resources/GFX/pointers/pic72.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DESTROY_WALL2,"data/resources/GFX/pointers/pic73.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DESTROY_WALL3,"data/resources/GFX/pointers/pic74.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DESTROY_WALL4,"data/resources/GFX/pointers/pic75.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DESTROY_WALL5,"data/resources/GFX/pointers/pic76.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DESTROY_WALL6,"data/resources/GFX/pointers/pic77.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_DESTROY_WALL7,"data/resources/GFX/pointers/pic78.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SUMMON_IMP0,"data/resources/GFX/pointers/pic79.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SUMMON_IMP1,"data/resources/GFX/pointers/pic80.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SUMMON_IMP2,"data/resources/GFX/pointers/pic81.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SUMMON_IMP3,"data/resources/GFX/pointers/pic82.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SUMMON_IMP4,"data/resources/GFX/pointers/pic83.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SUMMON_IMP5,"data/resources/GFX/pointers/pic84.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SUMMON_IMP6,"data/resources/GFX/pointers/pic85.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SUMMON_IMP7,"data/resources/GFX/pointers/pic86.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_LIGHTNING0,"data/resources/GFX/pointers/pic87.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_LIGHTNING1,"data/resources/GFX/pointers/pic88.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_LIGHTNING2,"data/resources/GFX/pointers/pic89.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_LIGHTNING3,"data/resources/GFX/pointers/pic90.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_LIGHTNING4,"data/resources/GFX/pointers/pic91.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_LIGHTNING5,"data/resources/GFX/pointers/pic92.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_LIGHTNING6,"data/resources/GFX/pointers/pic93.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_LIGHTNING7,"data/resources/GFX/pointers/pic94.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_HEAL0,"data/resources/GFX/pointers/pic103.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_HEAL1,"data/resources/GFX/pointers/pic104.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_HEAL2,"data/resources/GFX/pointers/pic105.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_HEAL3,"data/resources/GFX/pointers/pic106.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_HEAL4,"data/resources/GFX/pointers/pic107.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_HEAL5,"data/resources/GFX/pointers/pic108.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_HEAL6,"data/resources/GFX/pointers/pic109.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_HEAL7,"data/resources/GFX/pointers/pic110.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_PROTECT_MONSTER0,"data/resources/GFX/pointers/pic111.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_PROTECT_MONSTER1,"data/resources/GFX/pointers/pic112.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_PROTECT_MONSTER2,"data/resources/GFX/pointers/pic113.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_PROTECT_MONSTER3,"data/resources/GFX/pointers/pic114.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_PROTECT_MONSTER4,"data/resources/GFX/pointers/pic115.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_PROTECT_MONSTER5,"data/resources/GFX/pointers/pic116.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_PROTECT_MONSTER6,"data/resources/GFX/pointers/pic117.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_PROTECT_MONSTER7,"data/resources/GFX/pointers/pic118.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CAVE_IN0,"data/resources/GFX/pointers/pic119.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CAVE_IN1,"data/resources/GFX/pointers/pic120.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CAVE_IN2,"data/resources/GFX/pointers/pic121.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CAVE_IN3,"data/resources/GFX/pointers/pic122.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CAVE_IN4,"data/resources/GFX/pointers/pic123.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CAVE_IN5,"data/resources/GFX/pointers/pic124.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CAVE_IN6,"data/resources/GFX/pointers/pic125.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_CAVE_IN7,"data/resources/GFX/pointers/pic126.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SPEED_MONSTER0,"data/resources/GFX/pointers/pic127.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SPEED_MONSTER1,"data/resources/GFX/pointers/pic128.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SPEED_MONSTER2,"data/resources/GFX/pointers/pic129.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SPEED_MONSTER3,"data/resources/GFX/pointers/pic130.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SPEED_MONSTER4,"data/resources/GFX/pointers/pic131.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SPEED_MONSTER5,"data/resources/GFX/pointers/pic132.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SPEED_MONSTER6,"data/resources/GFX/pointers/pic133.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SPEED_MONSTER7,"data/resources/GFX/pointers/pic134.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SIGHT_OF_EVIL0,"data/resources/GFX/pointers/pic135.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SIGHT_OF_EVIL1,"data/resources/GFX/pointers/pic136.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SIGHT_OF_EVIL2,"data/resources/GFX/pointers/pic137.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SIGHT_OF_EVIL3,"data/resources/GFX/pointers/pic138.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SIGHT_OF_EVIL4,"data/resources/GFX/pointers/pic139.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SIGHT_OF_EVIL5,"data/resources/GFX/pointers/pic140.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SIGHT_OF_EVIL6,"data/resources/GFX/pointers/pic141.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(ANIMATED_CURSOR_TEXTURE_SIGHT_OF_EVIL7,"data/resources/GFX/pointers/pic142.bmp",true,GL_NEAREST,false);

			/* the money textures */
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_0,"data/resources/GFX/GUI/pic70.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_1,"data/resources/GFX/GUI/pic71.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_2,"data/resources/GFX/GUI/pic72.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_3,"data/resources/GFX/GUI/pic73.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_4,"data/resources/GFX/GUI/pic74.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_5,"data/resources/GFX/GUI/pic75.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_6,"data/resources/GFX/GUI/pic76.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_7,"data/resources/GFX/GUI/pic77.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_8,"data/resources/GFX/GUI/pic78.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(NUMBER_TEXTURE_9,"data/resources/GFX/GUI/pic79.bmp",true,GL_NEAREST,false);

			/* the minimap zoom in/out */
			GLOBAL_TEXTURE_LIST->add_texture(MINIMAP_ZOOM_IN_TEXTURE_NORMAL,"data/resources/GFX/GUI/pic237.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(MINIMAP_ZOOM_IN_TEXTURE_PRESSED,"data/resources/GFX/GUI/pic236.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(MINIMAP_ZOOM_OUT_TEXTURE_NORMAL,"data/resources/GFX/GUI/pic239.bmp",true,GL_NEAREST,false);
			GLOBAL_TEXTURE_LIST->add_texture(MINIMAP_ZOOM_OUT_TEXTURE_PRESSED,"data/resources/GFX/GUI/pic238.bmp",true,GL_NEAREST,false);

			GLOBAL_TEXTURE_LIST->build_textures();

			bool result = loadTexturesFromList();

			creature_txt_reader = new CCreatureTxtReader();
			GLOBAL_CREATURE_TXT_READER=creature_txt_reader;

			CSettingsManager sm;

			// read models for model pool from models.conf
			sm.init(CV_CONFIG_MODELS);

			map<string, string> *settings = sm.getSettings();

			char name[256];
			GLfloat scale = 1.0f,
					animSpeed = 1.0f;

			for (map<string, string>::iterator sIter = settings->begin(); sIter != settings->end(); sIter++)
			{
				CBR5Model *model = new CBR5Model();

				string s = sIter->second;

				sscanf(sIter->second.c_str(),"%s %f %f",name,&scale,&animSpeed);

				string modelFileName = CV_RESOURCES_DIRECTORY+string(name);

				model->loadFromFile(modelFileName);
				model->scaleToMaxY(scale);
				model->setAnimSpeed(animSpeed);
				model->setInterpolate(true);

				// update the texture name
				string tNamePath = string(model->getTextureName());
				string tName = tNamePath.substr(tNamePath.find_last_of("/"));
				string mPath = modelFileName.substr(0,modelFileName.find_last_of("/"));
				model->setTextureName(mPath+tName);

				modelPool[sIter->first] = new CObjectPool<CBR5Model>(true);
				modelPool[sIter->first]->addObjects(model,2);
			}
			sm.shutdown();

			// read effects for effect pool from effects.conf
			sm.init(CV_CONFIG_EFFECTS);

			settings = sm.getSettings();

			for (map<string, string>::iterator sIter = settings->begin(); sIter != settings->end(); sIter++)
			{
				string s = sIter->second;

				sscanf(sIter->second.c_str(),"%s",name);

				string effectFileName = CV_RESOURCES_DIRECTORY+string(name);

				CLUAEffect *effect = CLUAEffectLoader::loadFromFile(effectFileName);
				effect->init();

				effectPool[sIter->first] = new CObjectPool<CLUAEffect>(true);
				effectPool[sIter->first]->addObjects(effect,2);
			}
			sm.shutdown();

			return result;
		}

		CBR5Model *CResourceManager::getModel(string modelName)
		{
			if (modelPool.find(modelName)==modelPool.end())
			{
				CV_GAME_MANAGER->getConsole()->writeLine("Trying to access non-existing resource: "+modelName);
				return NULL;
			}
			return modelPool[modelName]->popObject();
		}

		GLvoid CResourceManager::returnModel(string modelName, CBR5Model *model)
		{
			modelPool[modelName]->pushObject(model);
		}

		CLUAEffect *CResourceManager::getEffect(string effectName)
		{
			if (effectPool.find(effectName)==effectPool.end())
			{
				CV_GAME_MANAGER->getConsole()->writeLine("Trying to access non-existing resource: "+effectName);
				return NULL;
			}
			return effectPool[effectName]->popObject();
		}

		GLvoid CResourceManager::returnEffect(string effectName, CLUAEffect *effect)
		{
			effectPool[effectName]->pushObject(effect);
		}

		bool CResourceManager::update()
		{
			return true;
		}

		bool CResourceManager::shutdown()
		{
			delete GLOBAL_TEXTURE_LIST;
			return true;
		}

		GLvoid CResourceManager::loadSelectedTexture(GLint textureIndex)
		{
			CV_TEXTURE_LIST->buildTexture(textureIndex);
		}

		bool CResourceManager::loadTexturesFromList()
		{
			ifstream iFile;
			iFile.open((CV_RESOURCES_DIRECTORY+CV_CONFIG_TEXTURES).c_str());

			if (!iFile)
			{
				return false;
			}

			string line="";

			while ( !iFile.eof() )
			{
				getline (iFile,line);

				if (line.length()!=0 && line.at(0)!='#')
				{
					pair<GLint, CTextureList::sTextureData> parsedResult = extractTextureInfo(line);

					// update texture location since we are on relative locations
					parsedResult.second.fileName=CV_RESOURCES_DIRECTORY+parsedResult.second.fileName;

					// add textures
					CV_TEXTURE_LIST->addTexture(parsedResult.first,parsedResult.second);
				}
			}

			iFile.close();

			// build them
			//CV_TEXTURE_LIST->buildTextures();

			return true;

		}

		pair<GLint, CTextureList::sTextureData> CResourceManager::extractTextureInfo(string line)
		{
			GLint index;
			char name[MAX_PATH];
			sscanf(line.c_str(),"%d %s",&index,(char *)&name);
			return pair<GLint, CTextureList::sTextureData>(index, CTextureList::sTextureData(name));
		}

		GLvoid CResourceManager::initEffects()
		{
			for (effIter = effectPool.begin(); effIter != effectPool.end(); effIter++)
			{
				std::vector<CLUAEffect*> *allEffects = effIter->second->getObjectList();

				for (GLuint e=0; e<allEffects->size(); e++)
				{
					(*allEffects)[e]->init();
				}
			}
		}

		string CResourceManager::onAction(string keyword, string params)
		{
			if (keyword==RLEFF)
			{
				// reload all the effects

				initEffects();

				return "Game effects reloaded!";
			}

			return "<>";
		}
	};
};