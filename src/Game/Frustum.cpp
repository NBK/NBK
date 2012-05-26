#include "Frustum.h"

#include <math.h>
#include <cml/cml.h>
#include "utils.h"

using namespace control;
using namespace cml;
using namespace utils;

namespace geometry
{
	CFrustum::CFrustum()
	{
	}

	CFrustum::~CFrustum()
	{
	}

	GLvoid CFrustum::initialize(GLfloat fieldOfView, GLfloat ratio, GLfloat nearPlaneDistance, GLfloat farPlaneDistance)
	{
		this->fieldOfView=fieldOfView;
		this->ratio=ratio;
		this->nearPlaneDistance=nearPlaneDistance;
		this->farPlaneDistance=farPlaneDistance;

		GLfloat degTan = tan(sGeometryUtils::deg2rad(fieldOfView/2.0f));

		nearPlaneHeight = degTan*nearPlaneDistance;	
		nearPlaneWidth = nearPlaneHeight*ratio;

		farPlaneHeight = degTan*farPlaneDistance;	
		farPlaneWidth = farPlaneHeight*ratio;
	}

	GLvoid CFrustum::updateRatio(GLfloat ratio)
	{
		this->ratio=ratio;
		nearPlaneWidth = nearPlaneHeight*ratio;
		farPlaneWidth = farPlaneHeight*ratio;
	}

	GLvoid CFrustum::update(CCamera *camera)
	{
		// calculate 8 points that represent 6 planes of frustum

		vector3f p = camera->getPosition();
		vector3f up = camera->getUpVector();
		vector3f dir = camera->getDirection();
		vector3f right = cross(dir,up);
		vector3f l = camera->getTargetPoint();

		vector3f zAxis = normalize(p-l);

		vector3f npC = p-zAxis*nearPlaneDistance;
		vector3f fpC = p-zAxis*farPlaneDistance;

		// near plane
		npTL = up*nearPlaneHeight-right*nearPlaneWidth+npC;
		npTR = up*nearPlaneHeight+right*nearPlaneWidth+npC;

		npBR = -up*nearPlaneHeight+right*nearPlaneWidth+npC;
		npBL = -up*nearPlaneHeight-right*nearPlaneWidth+npC;

		// far plane
		fpTL = up*farPlaneHeight-right*farPlaneWidth+fpC;
		fpTR = up*farPlaneHeight+right*farPlaneWidth+fpC;

		fpBR = -up*farPlaneHeight+right*farPlaneWidth+fpC;
		fpBL = -up*farPlaneHeight-right*farPlaneWidth+fpC;

		// 6 planes
		planes[PN_TOP].set(fpTR,fpTL,npTL);
		planes[PN_BOTTOM].set(fpBL,fpBR,npBR);

		planes[PN_FRONT].set(npBR,npTR,npTL);
		planes[PN_BACK].set(fpTL,fpTR,fpBR);

		planes[PN_LEFT].set(npTL,fpTL,fpBL);
		planes[PN_RIGHT].set(fpBR,fpTR,npTR);
	}

	GLvoid CFrustum::draw()
	{
		glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glColor3f(1.0f,0.0f,0.0f);
		glLineWidth(5.0f);
		glBegin(GL_LINES);
		{
			// near 
			glVertex3fv(&npTL[0]);
			glVertex3fv(&npTR[0]);

			glVertex3fv(&npTR[0]);
			glVertex3fv(&npBR[0]);

			glVertex3fv(&npBR[0]);
			glVertex3fv(&npBL[0]);

			glVertex3fv(&npBL[0]);
			glVertex3fv(&npTL[0]);

			// far
			glVertex3fv(&fpTL[0]);
			glVertex3fv(&fpTR[0]);

			glVertex3fv(&fpTR[0]);
			glVertex3fv(&fpBR[0]);

			glVertex3fv(&fpBR[0]);
			glVertex3fv(&fpBL[0]);

			glVertex3fv(&fpBL[0]);
			glVertex3fv(&fpTL[0]);

			// connections
			glVertex3fv(&npTL[0]);
			glVertex3fv(&fpTL[0]);

			glVertex3fv(&npTR[0]);
			glVertex3fv(&fpTR[0]);

			glVertex3fv(&npBR[0]);
			glVertex3fv(&fpBR[0]);

			glVertex3fv(&npBL[0]);
			glVertex3fv(&fpBL[0]);
		}
        glEnd();
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
	}

	bool CFrustum::containsPoint(cml::vector3f point)
	{
		for (GLint i=0; i<6; i++)
		{
			if (planes[i].pointStatus(point)==PS_OUTSIDE)
			{
				return false;
			}
		}
		return true;
	}

	/*bool CFrustum::containsBBOX(sBoundingBox &bbox)
	{
		vector3f *bboxPoints = bbox.getPoints();

		for (GLint i=0; i<8; i++)
		{
			if (containsPoint(bboxPoints[i]))
			{
				return true;
			}
		}

		return false;
	}*/

	bool CFrustum::containsBBOX(sBoundingBox &bbox)
	{
		bool result = true;
		int out,in;

		vector3f *points = bbox.getPoints();

		for(int i=0; i < 6; i++) 
		{
			out=0;in=0;

			for (int k = 0; k < 8 && (in==0 || out==0); k++) 
			{						
				if (planes[i].pointStatus(points[k]) == PS_OUTSIDE)
				{
					out++;
				}
				else
				{
					in++;
				}
			}

			if (!in)
			{
				return false;
			}
			else if (out)
			{
				result = true;
			}
		}
		return(result);
	}

	bool CFrustum::containsBBOX(sBoundingBox *bbox)
	{
		return containsBBOX(*bbox);
	}
};