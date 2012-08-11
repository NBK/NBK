#include "commons.h"
#include "Camera.h"
#include <fstream>
#include "utils.h"
#include "OGLUtils.h"

using namespace cml;
using namespace std;
using namespace utils;
using namespace game_objects;

namespace control
{
	#define D2R(deg) deg*3.1415926535f/180.0f

	CCamera::CCamera(): CEntity(), direction(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), normAll(0),
		posScale(1.0f), tmp_angle_x(0.0f), tmp_angle_y(0.0f), tmp_angle_z(0.0f)
	{
	}

	CCamera::~CCamera()
	{
	}

	GLvoid CCamera::transformView()
	{
		glLoadIdentity();
		vector3f target = mPosition+direction;
		gluLookAt(mPosition[0],mPosition[1],mPosition[2],target[0],target[1],target[2],up[0],up[1],up[2]);

		normAll++;
		if (normAll>1000)
		{
			normAll=0;
			normalize(direction);
			normalize(up);
		}
	}

	GLvoid CCamera::move(vector3f vec)
	{
		mPosition+=vec;
	}

	vector3f CCamera::rotateAroundArbitrary(const vector3f &v, vector3f &axis, GLfloat angle)
	{
		vector3f w;
		GLfloat c, s, t;

		normalize(axis);

		//calculate parameters of the rotation matrix
		c = cos(angle);
		s = sin(angle);
		t = 1 - c;

		//multiply v with rotation matrix
		w[0] = (t * axis[0] * axis[0] +          c) * v[0] 
			+ (t * axis[0] * axis[1] + s * axis[2]) * v[1] 
			+ (t * axis[0] * axis[2] - s * axis[1]) * v[2];
	        
		w[1] = (t * axis[0] * axis[1] - s * axis[2]) * v[0] 
			+ (t * axis[1] * axis[1] +          c) * v[1] 
			+ (t * axis[1] * axis[2] + s * axis[0]) * v[2];
	        
		w[2] = (t * axis[0] * axis[2] + s * axis[1]) * v[0] 
			+ (t * axis[1] * axis[2] - s * axis[0]) * v[1] 
			+ (t * axis[2] * axis[2] +          c) * v[2];

		w.normalize();
		w = w * length(v);
	    
		return w;
	}

	GLvoid CCamera::rotateX(GLfloat a)
	{
		tmp_angle_x+=a;
		a=D2R(a);		
		vector3f xaxis = cross(up,direction);
		direction=rotateAroundArbitrary(direction,xaxis,-a);
		up=rotateAroundArbitrary(up,xaxis,-a);
	}

	GLvoid CCamera::rotateY(GLfloat a)
	{
		tmp_angle_y+=a;
		a=D2R(a);		
		matrix33f_r roty
			( 	
				cos(a),0.0f,-sin(a),
				0.0f,1.0f,0.0f,
				sin(a),0.0f,cos(a)
			);

		direction=roty*direction;
		up=roty*up;
	}

	GLvoid CCamera::rotateZ(GLfloat a)
	{
		tmp_angle_z+=a;
		/*a=D2R(a);
		matrix33f_r rotz
			( 	
				cos(a),sin(a),0.0f,
				-sin(a),cos(a),0.0f,
				0.0f,0.0f,1.0f
			);

		direction=rotz*direction;
		up=rotz*up;*/
	}

	GLvoid CCamera::strafeLeft(GLfloat a)
	{
		mPosition+=cross(up,direction)*a;
	}

	GLvoid CCamera::moveForward(GLfloat a)
	{
		mPosition+=direction*a;
	}

	/*vector3f CCamera::getPosition()
	{
		return position;
	}*/

	vector3f CCamera::getDirection()
	{
		return direction;
	}

	vector3f CCamera::getUpVector()
	{
		return up;
	}

	vector3f CCamera::getTargetPoint()
	{
		return mPosition+direction;
	}

	GLvoid CCamera::setPosScale(GLfloat posScale)
	{
		this->posScale=posScale;
	}

	/*GLvoid CCamera::setPosition(vector3f position)
	{
		this->position=position*posScale;
	}*/

	GLfloat CCamera::getRotateX()
	{
		return tmp_angle_x;
	}

	GLfloat CCamera::getRotateY()
	{
		return tmp_angle_y;
	}

	GLfloat CCamera::getRotateZ()
	{
		return tmp_angle_z;
	}

	GLvoid CCamera::rotateAroundTargetOnY(cml::vector3f target, GLfloat angle)
	{
		vector3f oldPos = mPosition;
		this->setPosition(target);
		this->rotateY(angle);
		this->setPosition(-normalize(direction)*oldPos.length());
	}

	GLvoid CCamera::rotateAroundTargetOnX(cml::vector3f target, GLfloat angle)
	{
		vector3f oldPos = mPosition;
		this->setPosition(target);
		this->rotateX(angle);
		this->setPosition(-normalize(direction)*oldPos.length());
	}

	GLvoid CCamera::pushPosition()
	{
		tempPosition=mPosition;
	}

	GLvoid CCamera::popPosition()
	{
		mPosition=tempPosition;
	}

	GLvoid CCamera::setFollowPath(string pathFile)
	{
		// read contents of a file

		ifstream file;
		file.open(pathFile.c_str());

		string line = "";

		std::vector<vector3f> inPoints;

		while (file.good())
		{
			file >> line;

			GLfloat x,y;
			sscanf(line.c_str(),"%f,%f",&x,&y);

			inPoints.push_back(vector3f(x,y,0.0f));
		}

		file.close();

		// aproximate path
		path.clear();
		path=sGeometryUtils::generateBSplines(inPoints,inPoints.size()*300,2);
		pathPos=path.begin();
	}

	GLvoid CCamera::followPath(CDeltaTime *deltaTime)
	{
		vector3f dir = vector3f((*pathPos)[0],0.0f,(*pathPos)[2])-vector3f(mPosition[0],0.0f,mPosition[2]);

		GLfloat speed = 0.05f*deltaTime->getDelta();

		if (dir.length()<speed)
		{
			mPosition=*pathPos;
			pathPos++;
			if (pathPos==path.end())
			{
				path.clear();
				return;
			}
			direction = normalize(*pathPos-mPosition);
		}
		else
		{
			dir=dir.normalize();
			mPosition+=dir*speed;
		}
	}

	GLvoid CCamera::update(CDeltaTime *deltaTime)
	{
		if (path.size()>0)
		{
			if (pathPos==path.begin())
			{
				// calc direction
				mPosition = *pathPos;				
				pathPos++;
				direction = normalize(*pathPos-mPosition);
			}
			followPath(deltaTime);
		}
	}

	std::vector<vector3f> *CCamera::getPathPoints()
	{
		return &path;
	}
};