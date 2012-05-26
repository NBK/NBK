#ifndef BBOX_H
#define BBOX_H

#include <cml/cml.h>

namespace geometry
{
	struct sBoundingBox
	{
		GLfloat minX,maxX;
		GLfloat minY,maxY;
		GLfloat minZ,maxZ;

		GLfloat width,height,depth;

		cml::vector3f A,B,C,D,E,F,G,H,points[8];

		cml::vector3f Nfnt, Nbck, Nlft, Nrgt, Ntop, Nbtm;
		cml::vector3f extents, center, min, max;

		sBoundingBox()
		{
			reset();
		}

		sBoundingBox(sBoundingBox &bbox)
		{
			minX = bbox.minX;
			minY = bbox.minY;
			minZ = bbox.minZ;

			maxX = bbox.maxX;
			maxY = bbox.maxY;
			maxZ = bbox.maxZ;

			width = bbox.width;
			height = bbox.height;
			depth = bbox.depth;

			A = bbox.A;
			B = bbox.B;
			C = bbox.C;
			D = bbox.D;
			E = bbox.E;
			F = bbox.F;
			G = bbox.G;
			H = bbox.H;

			for (GLint i=0; i<8; i++)
			{
				points[i]=bbox.points[i];
			}

			Nfnt = bbox.Nfnt;
			Nbck = bbox.Nbck;
			Nlft = bbox.Nlft;
			Nrgt = bbox.Nrgt;
			Ntop = bbox.Ntop;
			Nbtm = bbox.Nbtm;

			extents = bbox.extents;
			center = bbox.center;
			min = bbox.min;
			max = bbox.max;
		}

		GLvoid reset()
		{
			maxX=maxY=maxZ=-1000000.0f;
			minX=minY=minZ=1000000.0f;
		}

		sBoundingBox(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY, GLfloat minZ, GLfloat maxZ)
		{
			this->minX=minX;
			this->maxX=maxX;
			this->minY=minY;
			this->maxY=maxY;
			this->minZ=minZ;
			this->maxZ=maxZ;
		}

		GLvoid translate(cml::vector3f vec)
		{
			minX+=vec[0];
			maxX+=vec[0];

			minY+=vec[1];
			maxY+=vec[1];

			minZ+=vec[2];
			maxZ+=vec[2];

			calculateExtents();
			calculateVertices();
		}

		GLvoid update(GLfloat x, GLfloat y, GLfloat z)
		{
			maxX=x>maxX?x:maxX;
			maxY=y>maxY?y:maxY;
			maxZ=z>maxZ?z:maxZ;

			minX=x<minX?x:minX;
			minY=y<minY?y:minY;
			minZ=z<minZ?z:minZ;
		}

		GLvoid update(GLfloat *xyz)
		{
			update(xyz[0],xyz[1],xyz[2]);
		}

		GLvoid update(cml::vector3f vec)
		{
			update(vec[0],vec[1],vec[2]);
		}

		GLvoid set(sBoundingBox &bbox)
		{
			this->maxX=bbox.maxX;
			this->maxY=bbox.maxY;
			this->maxZ=bbox.maxZ;

			this->minX=bbox.minX;
			this->minY=bbox.minY;
			this->minZ=bbox.minZ;
		}

		GLvoid calculateExtents()
		{
			width = maxX-minX;
			height = maxY-minY;
			depth = maxZ-minZ;

			extents=cml::vector3f(width,height,depth);
			center=cml::vector3f((minX+maxX)/2.0f,(minY+maxY)/2.0f,(minZ+maxZ)/2.0f);
			min=cml::vector3f(minX,minY,minZ);
			max=cml::vector3f(maxX,maxY,maxZ);
		}

		GLvoid calculateVertices()
		{
			A=cml::vector3f(minX,maxY,maxZ);
			B=cml::vector3f(maxX,maxY,maxZ);
			C=cml::vector3f(maxX,maxY,minZ);
			D=cml::vector3f(minX,maxY,minZ);

			E=cml::vector3f(minX,minY,maxZ);
			F=cml::vector3f(maxX,minY,maxZ);
			G=cml::vector3f(maxX,minY,minZ);
			H=cml::vector3f(minX,minY,minZ);

			Nfnt=cml::vector3f(0.0f,0.0f,1.0f);
			Nbck=cml::vector3f(0.0f,0.0f,-1.0f);
			Nlft=cml::vector3f(-1.0f,0.0f,0.0f);
			Nrgt=cml::vector3f(1.0f,0.0f,0.0f);
			Ntop=cml::vector3f(0.0f,1.0f,0.0f);
			Nbtm=cml::vector3f(0.0f,-1.0f,0.0f);
		}

		// returns all 8 points
		cml::vector3f *getPoints()
		{
			points[0]=A;
			points[1]=B;
			points[2]=C;
			points[3]=D;
			points[4]=E;
			points[5]=F;
			points[6]=G;
			points[7]=H;
			return points;
		}

		bool contains(cml::vector3f &pnt)
		{
			return (pnt[0]>=minX && pnt[0]<=maxX && pnt[1]>=minY && pnt[1]<=maxY && pnt[2]>=minZ && pnt[2]<=maxZ);
		}

		GLvoid draw()
		{
			glLineWidth(3.0f);
			glBegin(GL_LINES);
			{
				glVertex3fv(&A[0]);
				glVertex3fv(&B[0]);

				glVertex3fv(&B[0]);
				glVertex3fv(&C[0]);

				glVertex3fv(&C[0]);
				glVertex3fv(&D[0]);

				glVertex3fv(&D[0]);
				glVertex3fv(&A[0]);
			}
			glEnd();
			glLineWidth(1.0f);
		}
	};
};

#endif // BBOX_H