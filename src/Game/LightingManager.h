#ifndef LIGHTING_MANAGER_H
#define LIGHTING_MANAGER_H

#include "Block.h"
#include "Manager.h"
#include "LightSource.h"
#include <cml/cml.h>
#include <vector>
#include "Frustum.h"
#include "ConsoleListener.h"

/*
	The lighting manager. The main lighting unit.
	Stores lights for every block. There can be
	at max MAX_LIGHTS affecting one block. Block
	lookup happens in O(1). Very little memory
	gets used 85*85*sizeof(CBlockLightData).

	Lighting type: (proposal) ?		
		- static lighting: torches, candles...: using lightmaps, bumps faked by 1 light positioned somewhere
		- fully dynamic lights: mouse, projectiles...: using ppl
*/

#define MAX_LIGHTS_PER_BLOCK	4

namespace game_utils
{
	namespace managers
	{

		// private class
		class CBlockLightData
		{			
		public:

			CBlockLightData();
			~CBlockLightData();

			GLvoid add(game_objects::lighting::CLightSource *newLightSource, game_objects::CBlock *targetBlock);

			GLubyte									getLightsCount();
			game_objects::lighting::CLightSource	*getLightSource(GLint index);

			GLvoid						reset();

		private:

			GLubyte									lightsCount;
			game_objects::lighting::CLightSource	*lightSources[MAX_LIGHTS_PER_BLOCK];
		};

		// light types
		#define MAX_LIGHT_TYPES				16
		#define COLOR_TYPE_AMBIENT			100
		#define COLOR_TYPE_DIFFUSE			101
		#define ATTENUATION_TYPE_QUADRATIC	0
		#define ATTENUATION_TYPE_LINEAR		1
		#define ATTENUATION_TYPE_INITIAL	2

		class CLightingManager: public CManager, public control::CConsoleListener
		{
		public:
			CLightingManager();
			virtual ~CLightingManager();

			// from CManager
			virtual bool	init();
			virtual bool	update();
			virtual bool	shutdown();	

			// from CConsoleListener
			virtual std::string onAction(std::string keyword, std::string params);

			/*
				Usefull for debug purposes. It draws a wireframe cube
				where the light should be. Non visible ones get excluded
				by the frustum. This could be also done by the RenderManager
				but this way it is more compact.
			*/
			GLvoid			drawLightSources(geometry::CFrustum *viewFrustum);

			/*
				Where ever we wanna add a light we need to call this function.
				Here is what happens:

				- block light type is determined. if the block is alone and has a torch -> then it has torches all around
				- neighbour blocks get affected by this light so we need to proccess them too
				- all lights get picked out from a light sources pool
				
			*/
			GLvoid			addLightSource(cml::vector2i position, GLint lightSourceType);
			GLvoid			addLightSource(cml::vector2i position, std::string lightSourceName);

			/*
				Usually called by RenderManager to obtain light info
				for currently proccessed block.
			*/
			CBlockLightData *getBlockLightData(cml::vector2i blockPos);

			/*
				Set a specified param for a light of a specified type.
			*/
			GLvoid setLightParam(GLint lightSourceType, GLint attenuationType, GLfloat val);
			GLvoid setLightParam(GLint lightSourceType, GLint colorType, cml::vector3f color);

			GLfloat getLightParam(GLint lightSourceType, GLint attenuationType);

			/*
				Converts lightTypeIndex to lightTypeName.
			*/
			std::string lightSourceTypeIndexToName(GLint lightSourceType);

			/*
				Converts lightTypeName to lightTypeIndex.
			*/
			GLint		lightSourceNameToTypeIndex(std::string lightSourceName);

			std::string getLightTypes();

		private:			
	
			/*
				Holds light info for every possible block.
			*/
			CBlockLightData blockLightData[85][85];

			/*
				Holds references of all light objects. So we can easily erase them.
			*/
			std::vector<game_objects::lighting::CLightSource*>				lightSources;
			std::vector<game_objects::lighting::CLightSource*>::iterator	lightSourcesIter;

			// for console
			GLint	visibleLightObjects;
			bool	drawBBOX;

			/*
				Mappings from light type index to light type name.
			*/
			std::map<GLint,std::string> lightIndexName;
		};
	};
};

#endif // LIGHTING_MANAGER_H