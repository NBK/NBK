#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string>
#include "system.h"
#include <GL/gl.h>
#include <cml/cml.h>

namespace game_utils
{
	class CConversions
	{
	public:
		CConversions();
		~CConversions();

		static std::string		intToStr(GLint val);
		static std::string		floatToStr(GLfloat val);
		static std::string		vec3fToStr(cml::vector3f val);
		static std::string		vec2iToStr(cml::vector2i val);
		static std::string		vec3iToStr(cml::vector3i val);
		static std::string		vec3ubToStr(cml::vector3ub val);

		static GLint			strToInt(std::string val);
		static GLfloat			strToFloat(std::string val);
		static cml::vector3f	strToVec3f(std::string val);
		static cml::vector2f	strToVec2f(std::string val);

		static cml::vector2i	realToLogical(cml::vector3f val);
		static GLvoid			vec3fToFloat_p(cml::vector3f val, GLfloat *arr);
		static GLvoid			vec2fToFloat_p(cml::vector2f val, GLfloat *arr);
		//static GLvoid			swap(cml::vector3f &vec, GLint a, GLint b);
		static cml::vector3f	swap(cml::vector3f vec, GLint a, GLint b);
	};
};

#endif // CONVERSIONS_H