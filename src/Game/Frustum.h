#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "system.h"
#include <GL/gl.h>
#include "boundingBox.h"
#include "Camera.h"

namespace geometry
{
	class CFrustum
	{
	public:
		CFrustum();
		~CFrustum();

		// setup frustum
		GLvoid	initialize(GLfloat fieldOfView, GLfloat ratio, GLfloat nearPlaneDistance, GLfloat farPlaneDistance);
		GLvoid  updateRatio(GLfloat ratio);
		GLvoid	update(control::CCamera *camera);
		GLvoid	draw();

		bool	containsPoint(cml::vector3f point);
		bool	containsBBOX(sBoundingBox &bbox);
		bool	containsBBOX(sBoundingBox *bbox);

	private:
        
		GLfloat nearPlaneWidth,
				nearPlaneHeight,
				farPlaneWidth,
				farPlaneHeight,
				fieldOfView,
				ratio,
				nearPlaneDistance,
				farPlaneDistance;

		cml::vector3f	npTL,
						npTR,
						npBR,
						npBL,
						fpTL,
						fpTR,
						fpBR,
						fpBL;

		enum POINT_STATUS{PS_INSIDE=0, PS_OUTSIDE};

		struct sPlane
		{
			cml::vector3f	n;
			GLfloat			d;

			sPlane()
			{}

			inline GLvoid set(cml::vector3f &a, cml::vector3f &b, cml::vector3f &c)
			{
				n = normalize(cross(a-b,c-b));
				d = -dot(a,n);
			}			

			inline POINT_STATUS pointStatus(cml::vector3f &point)
			{
				return ((dot(point,n)+d)>=0.0f?PS_INSIDE:PS_OUTSIDE);
			}
		};

		enum PLANE_NAME{PN_TOP=0,PN_BOTTOM,PN_FRONT,PN_BACK,PN_LEFT,PN_RIGHT};
		sPlane planes[6];		
	};
};

#endif // FRUSTUM_H
