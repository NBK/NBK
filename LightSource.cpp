#include "settings.h"
#include "commons.h"
#include "LightSource.h"

using namespace cml;
using namespace std;
using namespace game_utils;
using namespace loaders;
using namespace geometry;

namespace game_objects
{
	namespace lighting
	{
		CLightSource::CLightSource(string name): CEntity()
		{
			this->name = name;
			// read light settings
			setAmbientColor(CV_GAME_MANAGER->getSettingsManager()->getSetting_vector3f("ST_"+name+"_COLOR_AMBIENT"));
			setDiffuseColor(CV_GAME_MANAGER->getSettingsManager()->getSetting_vector3f("ST_"+name+"_COLOR_DIFFUSE"));
			setSpecularColor(CV_GAME_MANAGER->getSettingsManager()->getSetting_vector3f("ST_"+name+"_COLOR_SPECULAR"));
			setQuadraticAttenuation(CV_GAME_MANAGER->getSettingsManager()->getSetting_Float("ST_"+name+"_Q_ATTENUATION"));
			setLinearAttenuation(CV_GAME_MANAGER->getSettingsManager()->getSetting_Float("ST_"+name+"_L_ATTENUATION"));
			setInitialAttenuation(CV_GAME_MANAGER->getSettingsManager()->getSetting_Float("ST_"+name+"_I_ATTENUATION"));
			setRadius(CV_GAME_MANAGER->getSettingsManager()->getSetting_Float("ST_"+name+"_RADIUS"));
		}

		CLightSource::CLightSource(CLightSource &lightSource): CEntity(lightSource)
		{
			memcpy(this->ambientColor,lightSource.ambientColor,sizeof(GLfloat)*4);
			memcpy(this->diffuseColor,lightSource.diffuseColor,sizeof(GLfloat)*4);
			memcpy(this->specularColor,lightSource.specularColor,sizeof(GLfloat)*4);
			this->quadraticAttenuation=lightSource.quadraticAttenuation;
			this->linearAttenuation=lightSource.linearAttenuation;
			this->initialAttenuation=lightSource.initialAttenuation;
			this->radius=lightSource.radius;
			this->name=lightSource.name;
		}

		CLightSource::~CLightSource()
		{
		}

		GLvoid CLightSource::setPosition(cml::vector3f position)
		{
			CEntity::setPosition(position);
		}

		GLvoid CLightSource::setQuadraticAttenuation(GLfloat attenuation)
		{
			this->quadraticAttenuation = attenuation;
		}
		
		GLvoid CLightSource::setLinearAttenuation(GLfloat attenuation)
		{
			this->linearAttenuation = attenuation;
		}
		
		GLvoid CLightSource::setInitialAttenuation(GLfloat attenuation)
		{
			this->initialAttenuation = attenuation;
		}

		GLvoid CLightSource::setAttenuation(GLint attIndex, GLfloat attenuation)
		{
			switch (attIndex)
			{
				case 0: 
				{
					setQuadraticAttenuation(attenuation);
					return;
				}
				case 1: 
				{
					setLinearAttenuation(attenuation);
					return;
				}
				case 2:
				{
					setInitialAttenuation(attenuation);
					return;
				}
			}		
		}

		GLvoid CLightSource::setRadius(GLfloat radius)
		{
			this->radius=radius;
		}

		GLvoid CLightSource::setAmbientColor(vector3f &color)
		{
			memcpy(ambientColor,&color[0],sizeof(GLfloat)*3);
			ambientColor[3]=1.0f;
		}

		GLvoid CLightSource::setDiffuseColor(vector3f &color)
		{
			memcpy(diffuseColor,&color[0],sizeof(GLfloat)*3);
			diffuseColor[3]=1.0f;
		}

		GLvoid CLightSource::setSpecularColor(vector3f &color)
		{
			memcpy(specularColor,&color[0],sizeof(GLfloat)*3);
			specularColor[3]=1.0f;
		}

		GLfloat *CLightSource::getAmbientColor()
		{
			return ambientColor;
		}

		GLfloat *CLightSource::getDiffuseColor()
		{
			return diffuseColor;
		}

		GLfloat *CLightSource::getSpecularColor()
		{
			return specularColor;
		}

		GLfloat	CLightSource::getAttenuation(GLint attIndex)
		{
			switch (attIndex)
			{
				case 0: 
				{
					return quadraticAttenuation;
				}
				case 1: 
				{
					return linearAttenuation;
				}
				case 2:
				{
					return initialAttenuation;
				}
				default:
				{
					return 0.0f;
				}
			}
		}

		GLfloat	CLightSource::getQuadraticAttenuation()
		{
			return quadraticAttenuation;
		}

		GLfloat CLightSource::getLinearAttenuation()
		{
			return linearAttenuation;
		}

		GLfloat CLightSource::getInitialAttenuation()
		{
			return initialAttenuation;
		}

		GLfloat CLightSource::getRadius()
		{
			return radius;
		}

		string CLightSource::getName()
		{
			return name;
		}
	};
};