#include "commons.h"
#include "HeroGate.h"
#include "perlin.h"

using namespace std;
using namespace cml;
using namespace loaders;
using namespace game_utils;
using namespace rendering;

namespace game_objects
{
	namespace block_objects
	{
		CHeroGate::CHeroGate(std::string name, cml::vector3f position)
			: CBlockObject(name,position,NULL)
		{
			setModel(CV_GAME_MANAGER->getResourceManager()->getModel(name));

			/* 
				This model is not animated when it's read, but we 
				animate it every frame therefore we must modify
				its VBO to make it reupload data every frame.
			*/

			if (model)
			{
				model->getVBO()->setBufferType(CVBO::BT_STREAM_DRAW);
			}

			time = 0.0f;
		}

		CHeroGate::~CHeroGate()
		{
		}

		CBR5Model *CHeroGate::getModel()
		{
			// before we return the model we deform the plane using perlin noise

			float	*vertices = model->getVertexCoordinates(),
					x=0.0f,y=0.0f;

			for (GLuint v=0; v<model->getVertexCount()*3; v+=3)
			{
				x = *(vertices+v+0);
				y = *(vertices+v+1);			

				// hero gates are allways on the XY axis so we modify the Z coordinate
				GLfloat details = 15.0f;
				GLfloat scale = 25.0f;
				*(vertices+v+2) = (GLfloat)PerlinNoise3D(x*details,y*details,time,2.0,2.0,1)/scale;
			}

			time += model->getAnimSpeed()*CV_GAME_MANAGER->getDeltaTime();

			return model;
		}
	};
};