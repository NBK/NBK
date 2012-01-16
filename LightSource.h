#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Entity.h"
#include <cml/cml.h>
#include <string>

/*
	Handles any/every type of light object:
		- torches
		- candles
		- heart flames
		- dynamic lights eg: imp claiming, missiles flying and/or exploding....
		...

	TODO: connect to light particle effect.
*/

namespace game_objects
{
	namespace lighting
	{
		class CLightSource: public CEntity
		{
		public:
			CLightSource(std::string name);
			CLightSource(CLightSource &lightSource);
			~CLightSource();

			GLvoid setPosition(cml::vector3f position);		
			GLvoid setAmbientColor(cml::vector3f color);
			GLvoid setDiffuseColor(cml::vector3f color);
			GLvoid setSpecularColor(cml::vector3f color);
			GLvoid setQuadraticAttenuation(GLfloat attenuation);
			GLvoid setLinearAttenuation(GLfloat attenuation);
			GLvoid setInitialAttenuation(GLfloat attenuation);
			GLvoid setAttenuation(GLint attIndex, GLfloat attenuation);
			GLvoid setRadius(GLfloat radius);

			GLfloat						*getAmbientColor();
			GLfloat						*getDiffuseColor();
			GLfloat						*getSpecularColor();
			GLfloat						getQuadraticAttenuation();
			GLfloat						getAttenuation(GLint attIndex);
			GLfloat						getLinearAttenuation();
			GLfloat						getInitialAttenuation();
			GLfloat						getRadius();
			std::string					getName();

		private:

			GLfloat ambientColor[4],
					diffuseColor[4],
					specularColor[4],
					radius,
					quadraticAttenuation,
					linearAttenuation,
					initialAttenuation;

			std::string name;
		};
	};
};

#endif // LIGHT_SOURCE_H