#include "commons.h"
#include "Conversions.h"
#include <iostream>
#include <sstream>
#include "utils.h"

using namespace std;
using namespace cml;
using namespace utils;

namespace game_utils
{
	CConversions::CConversions()
	{
	}

	CConversions::~CConversions()
	{
	}

	string CConversions::intToStr(GLint val)
	{
		std::ostringstream ostr;
		ostr << val;
		return ostr.str();
	}

	string CConversions::floatToStr(GLfloat val)
	{
		std::ostringstream ostr;
		ostr << val;
		return ostr.str();
	}
	
	GLint CConversions::strToInt(std::string val)
	{
		return atoi(val.c_str());
	}
	
	GLfloat CConversions::strToFloat(std::string val)
	{
		return (GLfloat)atof(val.c_str());
	}

	string CConversions::vec3fToStr(vector3f val)
	{
		//return floatToStr(val[0])+","+floatToStr(val[1])+","+floatToStr(val[2]);
		std::ostringstream ostr;
		ostr << val[0] << "," << val[1] << "," << val[2];
		return ostr.str();
	}

	string CConversions::vec2iToStr(vector2i val)
	{
		std::ostringstream ostr;
		ostr << val[0] << "," << val[1];
		return ostr.str();
	}

	string CConversions::vec3iToStr(vector3i val)
	{
		std::ostringstream ostr;
		ostr << val[0] << "," << val[1] << "," << val[2];
		return ostr.str();
	}

	string CConversions::vec3ubToStr(vector3ub val)
	{
		std::ostringstream ostr;
		ostr << (unsigned int)val[0] << "," << (unsigned int)val[1] << "," << (unsigned int)val[2];
		return ostr.str();
	}

	vector2i CConversions::realToLogical(vector3f val)
	{
		return vector2i((GLint)(val[0]/CV_BLOCK_WIDTH),(GLint)(val[2]/CV_BLOCK_DEPTH));
	}

	GLvoid CConversions::vec3fToFloat_p(cml::vector3f val, GLfloat *arr)
	{
		memcpy(arr,&val[0],sizeof(GLfloat)*3);
	}

	GLvoid CConversions::vec2fToFloat_p(cml::vector2f val, GLfloat *arr)
	{
		memcpy(arr,&val[0],sizeof(GLfloat)*2);
	}

	vector3f CConversions::strToVec3f(std::string val)
	{
		std::vector<string> vals;
		sStringUtils::tokenizeString(vals,val,",");
		return (vals.size()!=3?vector3f(-1.0f,-1.0f,-1.0f):vector3f(strToFloat(vals[0]),strToFloat(vals[1]),strToFloat(vals[2])));
	}

	vector2f CConversions::strToVec2f(std::string val)
	{
		std::vector<string> vals;
		sStringUtils::tokenizeString(vals,val,",");
		return (vals.size()!=2?vector2f(-1.0f,-1.0f):vector2f(strToFloat(vals[0]),strToFloat(vals[1])));
	}

	cml::vector3f CConversions::swap(cml::vector3f vec, GLint a, GLint b)
	{
		GLfloat tmp = vec[a];
		vec[a]=vec[b];
		vec[b]=tmp;
		return vec;
	}
};